#ifndef COMMON_H
#define COMMON_H

#include<linux/module.h>
#include<linux/version.h>
#include<linux/init.h>
#include<linux/stat.h>
#include<linux/proc_fs.h>
#include<asm/uaccess.h>

#define NAME_DIR "test_proc_dir"
#define NAME "test_module"
#define MSG_LENGTH 160

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHELIKHOV");

static int __init module_proc_init(void);	
static void __exit module_proc_exit(void);

module_init(module_proc_init);	
module_exit(module_proc_exit);	

#endif
