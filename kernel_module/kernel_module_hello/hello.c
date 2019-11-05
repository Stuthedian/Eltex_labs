#include <linux/module.h>
#include <linux/printk.h>

MODULE_AUTHOR("Me");
MODULE_DESCRIPTION("Playing around with kernel modules");
MODULE_LICENSE("GPL");

int init_module()
{        
  	pr_info("Hello world!\n");
	return 0;//success
}

void cleanup_module()
{        
	pr_info("Goodbye world!\n");
}
