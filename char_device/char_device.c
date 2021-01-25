#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "dummy_device"
#define MAX_DATA_LEN 10

dev_t dev_nr;
struct cdev *cdev;
struct class *my_class;
struct device *my_device;
int ret;
bool is_first_oper;
char data[MAX_DATA_LEN];

static int dev_open(struct inode *inode_ptr, struct file *file_ptr);
static int dev_close(struct inode *inode_ptr, struct file *file_ptr);
static ssize_t dev_read(struct file *fptr, char *buff, size_t len, loff_t *offset);
static ssize_t dev_write(struct file *fptr, const char *buff, size_t len, loff_t *offset);

static const struct file_operations fops = {
	.owner = THIS_MODULE,
  .open = dev_open,
  .release = dev_close,
	.read = dev_read,
	.write = dev_write,
};

static int dev_open(struct inode *inode_ptr, struct file *file_ptr) {
	printk(KERN_INFO "Opened device file\n");
  is_first_oper = true;
	return 0;
}

static int dev_close(struct inode *inode_ptr, struct file *file_ptr) {
	printk(KERN_INFO "Closed device file\n");
	return 0;
}

static ssize_t dev_read(struct file *fptr, char *buff, size_t len, loff_t *offset) {
  int byte_read = 0;

  if (!is_first_oper) {
    return 0;
  }

  if (len < MAX_DATA_LEN) byte_read = len;
  else byte_read = MAX_DATA_LEN;

  ret = copy_to_user(buff, data, byte_read);
	printk(KERN_INFO "Reading from file\n");
  is_first_oper = false;

	return byte_read;
}

static ssize_t dev_write(struct file *fptr, const char *buff, size_t len, loff_t *offset) {
  if (!is_first_oper) {
    return 0;
  }

  memset(data, 0, MAX_DATA_LEN);
  printk(KERN_INFO "Writing to file, len = %ld\n", len);
  
	ret = copy_from_user(data, buff, len);
  is_first_oper = false;

	return len;
}

static int init_cdev(void) {
	ret = alloc_chrdev_region(&dev_nr, 0, 1, DEVICE_NAME);
	if (ret) {
		printk("Can not regist major no\n");
		return ret;
	}
	printk(KERN_INFO "Registing successfully, major no is: %d\n", MAJOR(dev_nr));

	cdev = cdev_alloc();
	cdev->owner = THIS_MODULE;
	cdev->ops = &fops;
	cdev->dev = dev_nr;

	ret = cdev_add(cdev, dev_nr, 1);
	if (ret < 0) {
		printk(KERN_INFO "cdev_add error\n");
		return ret;
	}

	my_class = class_create(THIS_MODULE, "LED_class");
	if (IS_ERR(my_class)) {
		printk(KERN_INFO "Creating class failed\n");
		return PTR_ERR(my_class);
	}

	my_device = device_create(my_class, NULL, dev_nr, NULL, DEVICE_NAME);
	if (IS_ERR(my_device)) {
		printk(KERN_INFO "Creating device failed\n");
		return PTR_ERR(my_device);
	}

	return 0;
}

static void exit_cdev(void) {
	cdev_del(cdev);
	device_destroy(my_class, dev_nr);
	class_destroy(my_class);
	unregister_chrdev(dev_nr, DEVICE_NAME);
}

module_init(init_cdev);
module_exit(exit_cdev);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kien Nguyen");
MODULE_DESCRIPTION("Dummy device for demo");
MODULE_VERSION("1.0");
