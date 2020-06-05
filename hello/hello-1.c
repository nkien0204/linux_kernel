/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>
#include <linux/module.h>

int param_var[3] = {0, 0, 0};

module_param_array(param_var, int, NULL, S_IRUSR | S_IWUSR);

void display(void) {
  printk(KERN_INFO "TEST: param_var0 = %d\n", param_var[0]);
  printk(KERN_INFO "TEST: param_var1 = %d\n", param_var[1]);
  printk(KERN_INFO "TEST: param_var2 = %d\n", param_var[2]);
}

static int hello_init(void)
{
	printk(KERN_INFO "TEST: Hello world 1\n");
  display();
	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}

static void hello_clean(void)
{
	printk(KERN_INFO "TEST: Goodbye world 1.\n");
}

module_init(hello_init);
module_exit(hello_clean);