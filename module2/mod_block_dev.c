#include"mod_block_dev.h"

static int device_count = 3;

struct struct_device{
	int size;
	u8 *data;
	spinlock_t lock;
	struct request_queue *queue;
	struct gendisk *struct_gendisk;/*To display device into /dev.
					***We need: 
					***===to create (alloc_disk()); 
					***===to fill out; 
					***===to register (add_disk()) 
					***this structure into the kernel.
					***This struct is description of each disk in the kernel.*/
};

static struct struct_device *device = NULL;

static struct block_device_operations blk_dev_ops = {
	.owner = THIS_MODULE
};
/*
static int transfer_data(struct struct_device *device, unsigned long sector, unsigned long cnt_sectors, char *buffer, int write){
        unsigned long cnt_bytes = cnt_sectors * KERNEL_SECTOR_SIZE;
	unsigned long offset = sector * KERNEL_SECTOR_SIZE;
        if(write)
                memcpy(device->data + offset, buffer, cnt_bytes);
        else
                memcpy(buffer, device->data + offset, cnt_bytes);
        return 0;
}

static int process_bio_segment(struct struct_device *device, struct bio * bio){
	int result;
	struct bvec_iter i;
	struct bio_vec *bvec;
	size_t sector = bio_offset(bio);
	bio_for_each_segment(*bvec, bio, i){
		char *buffer;
		sector_t cnt_sectors;
	#if LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0)
		buffer = __bio_kmap_atomic(bio, i, KM_USER0);
	#else
		buffer = __bio_kmap_atomic(bio, i);
	#endif
		cnt_sectors = bio_sectors(bio);
		result = transfer_data(device, sector, cnt_sectors, buffer, bio_data_dir(bio) == WRITE);
		if(result != 0)
			return result;
		sector += cnt_sectors;
	#if LINUX_VERSION_CODE < KERNEL_VERSION(3,11,0)
		__bio_kunmap_atomic(bio, KM_USER0);
	#else
		__bio_kunmap_atomic(bio);
	#endif
	} 
	return 0;
}

static int process_bio(struct struct_device *device, struct request *req){
	struct bio *bio;
	int cnt_sectors = 0;
	printk(KERN_INFO "%s|INFO|Process bio.\n", DEVICE_NAME);
	__rq_for_each_bio(bio, req){
		process_bio_segment(device, bio);
		cnt_sectors += bio_cur_bytes(bio) / KERNEL_SECTOR_SIZE;
	}
	return cnt_sectors;
}
*/
static void process_request(struct request_queue *q){//Processing requests from the queue.
	struct request *req;
	int sectors;//Count of sectors for data exchange. 
	printk(KERN_INFO "%s|INFO|Processing request.\n", DEVICE_NAME);
	req = blk_fetch_request(q);
/*	while(req){
		struct struct_device *device = req->rq_disk->private_data;
		if(!blk_fs_request(req)){//Request doesn't valid.
			printk(KERN_ERR "%s|ERROR|No valid request.\n", DEVICE_NAME);
			__blk_end_request_all(req, -EIO);
			req = blk_fetch_request(q);
			continue;
		}
		sectors = process_bio(device, req);
		if(!__blk_end_request_cur(req, 0))
			req = blk_fetch_request(q);
	}
*/}

static void setup_device(struct struct_device *device, int num){
	memset(device, 0, sizeof(struct struct_device));
	device->size = size_MB * 1024 * 1024;
	device->data = vmalloc(device->size);
	if(device->data == NULL){
		printk(KERN_ERR "%s|ERROR|To allocate memory for device %d.\n", DEVICE_NAME, num);
		return;
	}else
		printk(KERN_ERR "%s|INFO|To allocate memory for device %d success.\n", DEVICE_NAME, num);

	spin_lock_init(&device->lock);

/*Initializing the queue for processing requests.*/
	device->queue = blk_init_queue(process_request, &device->lock);
	if(device->queue == NULL){
		if(device->data)
			vfree(device->data);
	}else
		printk(KERN_ERR "%s|INFO|Device %d queue initialization success.\n", DEVICE_NAME, num);

	blk_queue_logical_block_size(device->queue, logic_blk_size);
	device->queue->queuedata = device;

/*To create device into /dev.*/
	device->struct_gendisk = alloc_disk(MINORS);//CREATE
	if(!device->struct_gendisk){
		printk(KERN_ERR "%s|ERROR|alloc_disk failure.\n", DEVICE_NAME);
		if(device->data)
			vfree(device->data);
	}else
		printk(KERN_INFO "%s|INFO|Struct gendisk creating for device %d success.\n", DEVICE_NAME, num);
	device->struct_gendisk->major = major;//FILL OUT
	device->struct_gendisk->minors = MINORS;
	device->struct_gendisk->first_minor = num * MINORS;
	device->struct_gendisk->fops = &blk_dev_ops;
	device->struct_gendisk->queue = device->queue;
	device->struct_gendisk->private_data = device;
	sprintf(device->struct_gendisk->disk_name, "32", DEVICE_NAME"%c", num + 'a');
	set_capacity(device->struct_gendisk, sector_count * (logic_blk_size / KERNEL_SECTOR_SIZE));
//	add_disk(device->struct_gendisk);
	return;
}

static int __init module4_init(void){
	sector_count = size_MB * 1024 * 1024 / logic_blk_size;
	major = register_blkdev(major, DEVICE_NAME);

	if(major <= 0){
		printk(KERN_ERR "%s|ERROR|Device registration.\n", DEVICE_NAME);
		return -EBUSY;
	}else
		printk(KERN_INFO "%s|INFO|Device registration success.\n", DEVICE_NAME);

	device = kmalloc(device_count * sizeof(struct struct_device), GFP_KERNEL);
	if(device == NULL){
		printk(KERN_ERR "%s|ERROR|To allocate memory for device.\n", DEVICE_NAME);
		unregister_blkdev(major, DEVICE_NAME);
		return -ENOMEM;
	}

	int i;
	for(i = 0; i < device_count; i++)
		setup_device(device + i, i);
	return 0;
}

static void module4_exit(void){
	int i;
	for(i = 0; i < device_count; i++){
		struct struct_device *dev = device + i;
		if(dev->struct_gendisk){
			del_gendisk(dev->struct_gendisk);
			put_disk(dev->struct_gendisk);
			printk(KERN_INFO "%s|INFO|Delete struct gendisk for device %d success.\n", DEVICE_NAME, i);
		}
		if(dev->queue)
			blk_cleanup_queue(dev->queue);
			printk(KERN_INFO "%s|INFO|Cleanup queue for device %d success.\n", DEVICE_NAME, i);
		if(dev->data)
			vfree(dev->data);
			printk(KERN_INFO "%s|INFO|Free memory for device %d success.\n", DEVICE_NAME, i);
	}
	unregister_blkdev(major, DEVICE_NAME);
	kfree(device);
	printk(KERN_INFO "%s|INFO|Device unloading success.\n", DEVICE_NAME);
}

module_init(module4_init);
module_exit(module4_exit);
