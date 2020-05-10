Creating interface in the /proc.

module_proc.h:

NAME is the name of the device, which will be created in the /proc directory.
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

$dmesg --ctime | tail -n2
[DATE TIME] NAME|INFO|Loading module.
[DATE TIME] NAME|INFO|Creating proc directory success.

$ls -l /proc/(NAME)
-rw-rw-rw- 1 root root 0 DATE TIME /proc/NAME

$cat /proc/(NAME)
Initial text.

$dmesg --ctime | tail -n1
[DATE TIME] NAME|INFO|Read from the device 14 bytes.

$dd if=/dev/random of=/proc/NAME bs=5 count=1
1+0 records in
1+0 records out
5 bytes copied, 0,000247583 s, 20,2 kB/s

$dmesg --ctime | tail -n1
[DATE TIME] NAME|INFO|Write to the device 5 bytes.

$cat /proc/(NAME)
anything random text from /dev/random

$sudo rmmod module_proc

$ls -l /proc/module_proc
ls: cannot access '/proc/module_proc': No such file or directory

$dmesg --ctime | tail -n2
[DATE TIME] NAME|INFO|Removing /proc/NAME device.
[DATE TIME] NAME|INFO|Unloading module.
