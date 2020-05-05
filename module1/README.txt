A simple module, from which we can only read data.

Commands:

$sudo make
$sudo insmod module1.ko //To download module.
$dmesg | tail -n1
$cat /proc/devices | grep module1 //To look major number.
$sudo mknod -m0666 /dev/module1 c (major) 0
$cat /dev/module1
$dmesg | tail -n1
$sudo rm /dev/module1
$sudo rmmod module1 //To upload module.
$dmesg | tail -n1
