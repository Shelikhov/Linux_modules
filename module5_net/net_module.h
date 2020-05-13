#include<linux/module.h>
#include<linux/version.h>
#include<linux/netdevice.h>
#include<linux/etherdevice.h>

MODULE_LICENSE("GPL");

#define NET_NAME "testInterface"

static struct net_device *device;

static int __init net_module_init(void);
static void __exit net_module_exit(void);

module_init(net_module_init);
module_exit(net_module_exit);
