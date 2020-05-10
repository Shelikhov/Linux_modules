#include"module_proc.h"
#include"read_write_fops.c"

static const struct file_operations file_ops = {
	.owner = THIS_MODULE,
	.read = read_buffer,
	.write = write_buffer
};

static int __init module_proc_init(void){
	printk(KERN_INFO "%s|INFO|Loading module.\n", NAME);
	struct proc_dir_entry *proc_struct;
	proc_struct = proc_create(NAME, S_IFREG | S_IRUGO | S_IWUGO, NULL, &file_ops);
	
	if(proc_struct == NULL){
		printk(KERN_ERR "%s|ERROR|Creating proc directory failed!\n", NAME);
		return -ENOENT;
	}else
		printk(KERN_INFO "%s|INFO|Creating proc directory success.\n", NAME);
	return 0;
}

static void __exit module_proc_exit(void){
	remove_proc_entry(NAME, NULL);
	printk(KERN_INFO "%s|INFO|Removing /proc/%s device.\n", NAME, NAME);
	printk(KERN_INFO "%s|INFO|Unloading module.\n", NAME);
}
