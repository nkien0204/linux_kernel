#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

struct fake_device {
  char data[100];
  struct semaphore sem;
} virtual_device;

struct cdev *mcdev;
int major_number; //store major number from dev_t
int ret;
dev_t dev_num;  //major number that kernel give

#define DEVICE_NAME "fakedevice"

struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = device_open,
  .release = device_close,
  .write = device_write,
  .read = device_read
};

int device_open(struct inode *inode, struct file *filp) {
  //allow one process to open this device
  if (down_interruptible(&virtual_device.sem) != 0) {
    printk(KERN_ALERT "fakedevice: could not lock the device during open");
    return -1;
  }
  printk(KERN_INFO "fakedevice: opened device");
  return 0;
}

ssize_t device_read(struct file *filp, char *buf_store_data, size_t buf_count, loff_t *cur_offset) {
  printk(KERN_INFO "fakedevice: reading from device");
  ret = copy_to_user(buf_store_data, virtual_device.data, buf_count);
  return ret;
}

ssize_t device_write(struct file *filp, const char *buf_store_data, size_t buf_count, loff_t *cur_offset) {
  printk(KERN_INFO "fakedevice: reading from device");
  ret = copy_from_user(virtual_device.data, buf_store_data, buf_count);
  return ret;
}

int device_close(struct inode *inode, struct file *filp) {
  up(&virtual_device.sem);
  printk(KERN_INFO "fakedevice: closed device");
  return 0;
}

static int driver_entry(void) {
  ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
  if (ret < 0) {
    printk(KERN_ALERT "fakedevice: failed to allocate major number");
    return ret;
  }
  major_number = MAJOR(dev_num);
  printk(KERN_INFO "fakedevice: major number is %d", major_number);
  printk(KERN_INFO "\tuse \"mknod /dev/%s c %d 0\" for device file", DEVICE_NAME, major_number); //dmesg

  mcdev = cdev_alloc(); //create our cdev structure, initialized our cdev
  mcdev->ops = &fops; //struct file_operations
  mcdev->owner = THIS_MODULE;

  ret = cdev_add(mcdev, dev_num, 1);  //add device to kernel
  if (ret < 0) {
    printk(KERN_ALERT "fakedevice: unable to add device to kernel");
    return ret;
  }

  sema_init(&virtual_device.sem, 1);  //initial value of one

  return 0;
}

static void driver_exit(void) {
  cdev_del(mcdev);
  unregister_chrdev_region(dev_num, 1);
  printk(KERN_ALERT "fakedevice: unloaded module");
}

module_init(driver_entry);
module_exit(driver_exit);