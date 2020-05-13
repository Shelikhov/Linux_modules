Simple interface module.



net_module.c:


alloc_netdev          - kernel function for dynamic allocation memory for struct net_device.

ether_setup           - fill in the fields of the device interface with the ethernet generic values.

register_netdev       - for registration interface into the kernel.

open_net_module       - allows transmit

start_xmit_net_module - provides hardware transmittion retrieved packets to net, 
			then removes socket buffer.



Testing module:

$sudo make

$sudo insmod net_module.ko

$dmesg --ctime | tail -n5
[DATE TIME] testInterface|INFO|Loading module.
[DATE TIME] testInterface|INFO|Setup device.
[DATE TIME] testInterface|INFO|Registration success.
[DATE TIME] IPv6: ADDRCONF(NETDEV_UP): testInterface0: link is not ready
[DATE TIME] testInterface|INFO|Interface opened.

$ip link show dev testInterface0
4: testInterface0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UNKNOWN mode DEFAULT group default qlen 1000
    link/ether 00:01:02:03:04:05 brd ff:ff:ff:ff:ff:ff

$sudo ifconfig testInterface0 10.0.1.17//for example

$ifconfig
testInterface0 Link encap:Ethernet  HWaddr 00:01:02:03:04:05  
          inet addr:10.0.1.17  Bcast:10.255.255.255  Mask:255.0.0.0
          inet6 addr: fe80::201:2ff:fe03:405/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

$ping 10.0.1.17
PING 10.0.1.17 (10.0.1.17) 56(84) bytes of data.
64 bytes from 10.0.1.17: icmp_seq=1 ttl=64 time=0.042 ms
64 bytes from 10.0.1.17: icmp_seq=2 ttl=64 time=0.049 ms
64 bytes from 10.0.1.17: icmp_seq=3 ttl=64 time=0.049 ms
64 bytes from 10.0.1.17: icmp_seq=4 ttl=64 time=0.048 ms
64 bytes from 10.0.1.17: icmp_seq=5 ttl=64 time=0.052 ms
^C
--- 10.0.1.17 ping statistics ---
5 packets transmitted, 5 received, 0% packet loss, time 3996ms
rtt min/avg/max/mdev = 0.042/0.048/0.052/0.003 ms

$sudo rmmod net_module

$dmesg --ctime | tail -n2
[DATE TIME] testInterface|INFO|Interface closed.
[DATE TIME] testInterface|INFO|Unregistration interface.
