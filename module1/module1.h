#include<linux/module.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHELIKHOV");
MODULE_VERSION("6.3");

#define MODULE_NAME "module1"
#define MESG_LENGTH 1024

static int __init module1_init(void);
static void __exit module1_exit(void);

module_init(module1_init);
module_exit(module1_exit);
