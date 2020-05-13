#include"net_module.h"

static int open_net_module(struct net_device *dev){
	printk(KERN_INFO "%s|INFO|Interface opened.\n", NET_NAME);
	netif_start_queue(dev);
	return 0;
}

static int close_net_module(struct net_device *dev){
	printk(KERN_INFO "%s|INFO|Interface closed.\n", NET_NAME);
	netif_stop_queue(dev);
	return 0;
}

static int start_xmit_net_module(struct sk_buff *skb, struct net_device *dev){
	dev_kfree_skb(skb);
	return 0;
}

static struct net_device_ops device_ops = {
	.ndo_open = open_net_module,
	.ndo_stop = close_net_module,
	.ndo_start_xmit = start_xmit_net_module,
};

static void setup_net_module(struct net_device *dev){//setting the mac address for interface
	printk(KERN_INFO "%s|INFO|Setup device.\n", NET_NAME);
	int i;
	for(i = 0; i < ETH_ALEN; ++i)//ETH_ALEN octets in one ethernet addr (6).
		dev->dev_addr[i] = i;
	ether_setup(dev);
	dev->netdev_ops = &device_ops;
}

static int __init net_module_init(void){
	printk(KERN_INFO "%s|INFO|Loading module.\n", NET_NAME);

	char interface_name[20];
	sprintf(interface_name, "%s%s", NET_NAME, "%d");
	device = alloc_netdev(0, interface_name, 0, setup_net_module);

	if(register_netdev(device)){
		printk(KERN_ERR "%s|ERROR|Registration failed.\n", NET_NAME);
		free_netdev(device);
		return -1;
	}

	printk(KERN_INFO "%s|INFO|Registration success.\n", NET_NAME);
	return 0;
}

static void __exit net_module_exit(void){
	unregister_netdev(device);
	free_netdev(device);
	printk(KERN_INFO "%s|INFO|Unregistration interface.\n", NET_NAME);
}
