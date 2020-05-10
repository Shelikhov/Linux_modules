Creating interface in the /proc.

module_proc.h:

NAME_DIR is a name of the directory, which will be created in the /proc.
NAME is a name of the device, which will be created in the NAME_DIR.
MSG_LENGTH is a size of the /proc/NAME buffer.



module_proc.c:

module_proc_init - for loading module to the kernel.
module_proc_exit - for unloading module to the kernel.



read_write_fops.c:

read_buffer - for reading from the device /proc/NAME.
write_buffer - for writing to the device /proc/NAME.



TESTING THE MODULE:

$sudo make

$sudo insmod module_proc.ko

$dmesg --ctime | tail -n3
[DATE TIME] NAME|INFO|Loading module.
[DATE TIME] NAME_DIR|INFO|Creating directory in the /proc success.
[DATE TIME] NAME|INFO|Creating file in the /proc/NAME_DIR success.

$ls -l /proc/(NAME_DIR)/(NAME)
-rw-rw-rw- 1 root root 0 DATE TIME /proc/NAME_DIR/NAME

$cat /proc/(NAME_DIR)/(NAME)
Initial text.

$dmesg --ctime | tail -n1
[DATE TIME] NAME|INFO|Read from the device 14 bytes.

$dd if=/dev/random of=/proc/(NAME_DIR)/(NAME) bs=5 count=1
1+0 records in
1+0 records out
5 bytes copied, 0,000247583 s, 20,2 kB/s

$dmesg --ctime | tail -n1
[DATE TIME] NAME|INFO|Write to the device 5 bytes.

$cat /proc/(NAME_DIR)/(NAME)
anything random text from /dev/random

$sudo rmmod module_proc

$ls -l /proc/(NAME_DIR)/(NAME)
ls: cannot access '/proc/(NAME_DIR)/(NAME)': No such file or directory

$dmesg --ctime | tail -n2
[DATE TIME] NAME|INFO|Removing /proc/(NAME_DIR)/(NAME) device.
[DATE TIME] NAME|INFO|Unloading module.
