#include<linux/cdev.h>
#include"module1.h"

static int major = 0;
static char *module_content = "Module content!\n";

static struct cdev mod_cdev;
static int device_open = 0;

static int module1_open(struct inode *inode, struct file *file){
	if(device_open)
		printk(KERN_INFO "Device already opened!\n");
	device_open++;
	return 0;
}

static int module1_close(struct inode *inode, struct file *file){
	device_open--;
	return 0;
}

static ssize_t module1_read(struct file *file, char *user_buffer, size_t length, loff_t *pos){
	int count = strlen(module_content);
	if(length < count)
		return -EINVAL;
	if(*pos != 0){
		printk(KERN_INFO "Read from module: EOF.\n");
		return 0;
	}
	if(copy_to_user(user_buffer, module_content, count))
		return -EINVAL;
	*pos = count;
	printk(KERN_INFO "Read from module: %d.\n", count);
	return count;
}

static const struct file_operations device_opts = {
	.owner = THIS_MODULE,
	.open = module1_open,
	.release = module1_close,
	.read = module1_read
};

static int __init module1_init(void){
	dev_t dev_num;
	int result;
	result = alloc_chrdev_region(&dev_num, 0, 1, MODULE_NAME);
	major = MAJOR(dev_num);
	if(result < 0){
		printk(KERN_ERR "Error during allocate device number!\n");
		return result;
	}
	cdev_init(&mod_cdev, &device_opts);
	mod_cdev.owner = THIS_MODULE;
	result = cdev_add(&mod_cdev, dev_num, 1);
	if(result < 0){
		printk(KERN_ERR "Error during adding device to kernel!\n");
		return result;
	}
	printk(KERN_INFO "module1 installed, major %d, minor %d.\n", MAJOR(dev_num), MINOR(dev_num));
	return 0;
}

static void __exit module1_exit(void){
	cdev_del(&mod_cdev);
	unregister_chrdev_region(MKDEV(major, 0), 1);
	printk(KERN_INFO "module1 deleted!\n");
}
