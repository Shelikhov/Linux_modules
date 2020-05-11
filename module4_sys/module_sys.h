#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/parport.h>
#include<linux/pci.h>
#include<linux/version.h>
#include<asm/uaccess.h>

#define MSG_LENGTH 160
#define FILE_NAME "test_sys_file"
#define CLASS_NAME "test_sys_class"

MODULE_LICENSE("GPL");

static char buffer[MSG_LENGTH + 1] = "Initial text.";

static int __init sys_module_init(void);
static void __exit sys_module_cleanup(void);

module_init(sys_module_init);
module_exit(sys_module_cleanup);
