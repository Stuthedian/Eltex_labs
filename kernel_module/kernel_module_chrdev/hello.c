#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_AUTHOR("Me");
MODULE_DESCRIPTION("Playing around with kernel modules");
MODULE_LICENSE("GPL");

ssize_t write_hello(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
ssize_t read_hi (struct file *file, char __user *buf, size_t count, loff_t *ppos);

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.read = read_hi,
	.write = write_hello
};

static unsigned int major_number;
static const char* device_name = "test";

int init_module()
{        
  	pr_info("Hello world!\n");
	major_number = register_chrdev(0, device_name, &fops);

	pr_info("My major number: %d\n", major_number);

	return 0;//success
}

void cleanup_module()
{
	unregister_chrdev(major_number, device_name);
	pr_info("Goodbye world!\n");
}

ssize_t write_hello(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char kern_buf[20] = {0};
	ssize_t rslt = simple_write_to_buffer(kern_buf, 19, ppos, buf, count);
	pr_info("%s", kern_buf);
	return rslt;
}
ssize_t read_hi (struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	return simple_read_from_buffer(buf, count, ppos, "hi!\n", 5);
}
