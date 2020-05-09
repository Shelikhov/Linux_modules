#include<linux/module.h>
#include<linux/vmalloc.h>
#include<linux/blkdev.h>
#include<linux/genhd.h>
#include<linux/errno.h>
#include<linux/hdreg.h>
#include<linux/version.h>
#include<linux/init.h>

MODULE_AUTHOR("SHELIKHOV");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.5");

#define DEVICE_NAME "block_device"
#define KERNEL_SECTOR_SIZE 512
#define MINORS 16 //Partitions of the device.

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
#define blk_fs_request(rq) ((rq)->cmd_type == 	REQ_TYPE_FS)
#endif

static int major = 0;

static int size_MB = 5;//device size into MB

static int logic_blk_size = KERNEL_SECTOR_SIZE;//Size of one partition.

static int sector_count = 0;//Count of parttions.

