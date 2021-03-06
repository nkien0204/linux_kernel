#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>

static struct net_device *my_dev;

static int veth_dev_init(struct net_device *dev);
static int veth_open(struct net_device *dev);
static int veth_close(struct net_device *dev);
static int veth_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd);

static struct veth_priv {
  struct net_device_stats stats;
  int status;
  struct snull_packet *ppool;
  struct snull_packet *rx_queue;
  int rx_int_enabled;
  int tx_packetlen;
  u8 *tx_packetdata;
  struct sk_buff *skb;
  spinlock_t lock;
};

static int veth_open(struct net_device *dev) {
  memcpy(dev->dev_addr, "\0ABCD0", ETH_ALEN);
  netif_start_queue(dev);
  return 0;
}

static int veth_close(struct net_device *dev) {
  netif_stop_queue(dev);
  return 0;
}

static netdev_tx_t veth_xmit(struct sk_buff *skb, struct net_device *dev) {
  return NETDEV_TX_OK;
}

static struct net_device_ops veth_ops =
{
 // .ndo_init = veth_dev_init,
  .ndo_open = veth_open,
  .ndo_stop = veth_close,
 // .ndo_do_ioctl = veth_ioctl,
  .ndo_start_xmit = veth_xmit
};

static int __init veth_init(void)
{
  my_dev = alloc_netdev(sizeof(struct veth_priv), "my_dev", 1, ether_setup);
  if (my_dev == NULL)
    return -ENOMEM;

  my_dev->netdev_ops = &veth_ops;

  register_netdev(my_dev);
  return 0;
}

static void __exit veth_exit(void)
{
  unregister_netdev(my_dev);
  free_netdev(my_dev);
}

module_init(veth_init);
module_exit(veth_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kien Nguyen");
MODULE_DESCRIPTION("Dummy loopback interface");
MODULE_VERSION("1.0");
