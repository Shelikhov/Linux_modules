#include"module_proc.h"
#include"read_write_fops.c"

static const struct file_operations file_ops = {
	.owner = THIS_MODULE,
	.read = read_buffer,
	.write = write_buffer
};

static struct proc_dir_entry *proc_dir;

static int __init module_proc_init(void){
        printk(KERN_INFO "%s|INFO|Loading module.\n", NAME);
        struct proc_dir_entry *proc_dev;

        proc_dir = proc_mkdir(NAME_DIR, NULL);//NULL is parent
        if(proc_dir == NULL){
                printk(KERN_ERR "%s|ERROR|Creating directory in the /proc failed!\n", NAME_DIR);
                return -ENOENT;
        }else
                printk(KERN_INFO "%s|INFO|Creating directory in the /proc success.\n", NAME_DIR);

        proc_dev = proc_create(NAME, S_IFREG | S_IRUGO | S_IWUGO, proc_dir, &file_ops);
        if(proc_dev == NULL){
                printk(KERN_ERR "%s|ERROR|Creating file in the /proc/%s failed!\n", NAME, NAME_DIR);
                remove_proc_entry(NAME_DIR, NULL);
                return -ENOENT;
        }else
                printk(KERN_INFO "%s|INFO|Creating file in the /proc/%s success.\n", NAME, NAME_DIR);
        return 0;
}

static void __exit module_proc_exit(void){
        remove_proc_entry(NAME, proc_dir);
        remove_proc_entry(NAME_DIR, NULL);
        printk(KERN_INFO "%s|INFO|Removing /proc/%s/%s device.\n", NAME, NAME_DIR, NAME);
        printk(KERN_INFO "%s|INFO|Unloading module.\n", NAME);
}
