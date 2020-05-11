Creating class in the /sys/class.


module_sys.c

sys_module_init - for initialization and loading module.
		  A struct class (struct kobject) is being created, 
		  then attribute record (CLASS_ATTR) linked 
		  to a previously created class.

sys_module_exit - for removing file (FILE_NAME) and 
		  destroying class (module_class).

show_buffer     - to read information from /sys/class/CLASS_NAME/FILE_NAME

store_buffer    - to write information to /sys/class/CLASS_NAME/FILE_NAME


Commands for testing module:

$sudo make

$sudo insmod module_sys.ko

$dmesg --ctime | tail -n2
[DATE TIME] test_sys_class|INFO|Module loading.
[DATE TIME] test_sys_file|INFO|File creating.

$cat /sys/class/CLASS_NAME/FILE_NAME
Initial text.

$sudo dd if=/dev/random of=/sys/class/CLASS_NAME/FILE_NAME bs=5 count=1
f=/s/random of=/sys/class/test_sys_class/test_file bs=5 count=1
1+0 records in
1+0 records out
5 bytes copied, 0,000434236 s, 11,5 kB/s

$dmesg --ctime | tail -n2
[DATE TIME] test_sys_file|INFO|Read from the module 13 bytes.
[DATE TIME] test_sys_file|INFO|Write to the module 5 bytes.

$sudo rmmod module_sys

$dmesg --ctime | tail -n2
[DATE TIME] test_sys_file|INFO|File deletion.
[DATE TIME] test_sys_file|INFO|Unloading module.

