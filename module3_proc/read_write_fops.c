#include"module_proc.h"

static char *get_buffer(void){
	static char msg[MSG_LENGTH + 1] = "Initial text.\n";
	return msg;
}

static ssize_t read_buffer(struct file *file, char *buffer, size_t count, loff_t *pos){
	char *msg = get_buffer();
	int result;

	if(*pos >= strlen(msg)){
		*pos = 0;
		return 0;
	}

	if(count > strlen(msg) - *pos)
		count = strlen(msg) - *pos;
	result = copy_to_user((void*)buffer, msg + *pos, count);
	*pos += count;
	
	printk(KERN_INFO "%s|INFO|Read from the device %d bytes.\n", NAME, count);
	return count;
}

static ssize_t write_buffer(struct file *file, const char *buffer, size_t count, loff_t *pos){
	char *buf = get_buffer();
	char msg[MSG_LENGTH], *p_msg = msg;
	size_t len = count > MSG_LENGTH ? MSG_LENGTH : count;
	copy_from_user(p_msg, (void*)buffer, len);
	msg[len] = '\n';
	strcpy(buf, msg);
	buf[MSG_LENGTH] = '\0';
	printk(KERN_INFO "%s|INFO|Write to the device %d bytes.\n", NAME, len);
	return len;
}
