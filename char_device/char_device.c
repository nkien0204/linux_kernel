#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "LED"

dev_t dev;
struct cdev *cdev;
struct class * my_class;
struct device * my_device;
int ret;
char data[100];

static int dev_open (struct inode * inode_ptr, struct file * file_ptr);
static int dev_close (struct inode * inode_ptr, struct file * file_ptr);
static ssize_t dev_read (struct file * fptr, char * buff, size_t len, loff_t * offset);
static ssize_t dev_write (struct file * fptr, const char * buff, size_t len, loff_t * offset);

static const struct file_operations fops = {
	.open = dev_open,
	.release = dev_close,
	.read = dev_read,
	.write = dev_write,
};

static int dev_open (struct inode * inode_ptr, struct file * file_ptr) {
	printk (KERN_INFO "Opened device file\n");
	return 0;
}

static int dev_close (struct inode * inode_ptr, struct file * file_ptr) {
	printk (KERN_INFO "Closed device file\n");
	return 0;
}

static ssize_t dev_read (struct file * fptr, char * buff, size_t len, loff_t * offset) {
	ret = copy_to_user (buff, data, len);
	printk (KERN_INFO "Reading from file: %s\n", buff);
	return len;
}

static ssize_t dev_write (struct file * fptr, const char * buff, size_t len, loff_t * offset) {
	printk (KERN_INFO "Writing to file\n");
	ret = copy_from_user (data, buff, len);
	return len;
}

static int init_cdev (void) {
	ret = alloc_chrdev_region (&dev, 0, 1, DEVICE_NAME);
	if (ret) {
		printk ("Can not regist major no\n");
		return ret;
	}
	printk (KERN_INFO "Registing successfully, major no is: %d\n", MAJOR (dev));

	cdev = cdev_alloc ();
	cdev->owner =  THIS_MODULE;
	cdev->ops = &fops;
	cdev->dev = dev;

	ret = cdev_add (cdev, dev, 1);
	if (ret < 0) {
		printk (KERN_INFO "cdev_add error\n");
		return ret;
	}

	my_class = class_create (THIS_MODULE, "LED_class");
	if (IS_ERR (my_class)) {
		printk (KERN_INFO "Creating class failed\n");
		return PTR_ERR(my_class);
	}
	printk (KERN_INFO "Creating class successfully\n");

	my_device = device_create (my_class, NULL, dev, NULL, DEVICE_NAME);
	if (IS_ERR (my_device)) {
		printk (KERN_INFO "Creating device failed\n");
		return PTR_ERR (my_device);
	}
	printk (KERN_INFO "Creating device successfully\n");

	return 0;
}

static void exit_cdev(void) {
	cdev_del (cdev);
	device_destroy (my_class, dev);
	class_destroy (my_class);
	unregister_chrdev (dev, DEVICE_NAME);
}

module_init (init_cdev);
module_exit (exit_cdev);

MODULE_LICENSE ("GPL");
MODULE_AUTHOR ("Kien Nguyen");
MODULE_DESCRIPTION ("Blink LED for demo");
MODULE_VERSION ("0.01");
