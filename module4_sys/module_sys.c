#include"module_sys.h"

static ssize_t show_buffer(struct class *class, struct class_attribute *attr, char *buf){
	strcpy(buf, buffer);
	printk(KERN_INFO "%s|INFO|Read from the module %d bytes.\n", FILE_NAME, strlen(buf));
	return strlen(buf);
}

static ssize_t store_buffer(struct class *class, struct class_attribute *attr, const char *buf, size_t cnt){
	strncpy(buffer, buf, cnt);
	buffer[cnt] = '\0';
	printk(KERN_INFO "%s|INFO|Write to the module %d bytes.\n", FILE_NAME, cnt);
	return cnt;
}

CLASS_ATTR(test_file, (S_IWUSR | S_IRUGO), &show_buffer, &store_buffer);

static struct class *module_class;

static int __init sys_module_init(void){
	printk(KERN_INFO "%s|INFO|Module loading.\n", CLASS_NAME);
	module_class = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(module_class))
		printk(KERN_ERR "%s|ERROR|Class creating.\n", CLASS_NAME);

	class_create_file(module_class, &class_attr_test_file);
	printk(KERN_INFO "%s|INFO|File creating.\n", FILE_NAME);
	return 0;	
}

static void __exit sys_module_cleanup(void){
	class_remove_file(module_class, &class_attr_test_file);
	class_destroy(module_class);
	printk(KERN_INFO "%s|INFO|File deletion.\n", FILE_NAME);
	printk(KERN_INFO "%s|INFO|Unloading module.\n", FILE_NAME);
	return;
}
