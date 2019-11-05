#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

MODULE_AUTHOR("Me");
MODULE_DESCRIPTION("Playing around with kernel modules");
MODULE_LICENSE("GPL");

typedef struct _RETURN_STRING 
{   char buf[ 160 ]; } RETURN_STRING;

#define TEST_IOC_MAGIC    'h'
#define IOCTL_GET_STRING _IOR(TEST_IOC_MAGIC, 1, RETURN_STRING)

ssize_t write_hello(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
ssize_t read_hi (struct file *file, char __user *buf, size_t count, loff_t *ppos);
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg); 

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.read = read_hi,
	.write = write_hello,
	.unlocked_ioctl = my_ioctl,
	.compat_ioctl	= my_ioctl,
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

static long my_ioctl( struct file *file, unsigned int cmd, unsigned long arg) 
{
	switch( cmd ) 
	{
      case IOCTL_GET_STRING:
			if(copy_to_user((void*)arg, "hi!!\n", 6))
			return -EFAULT;
         break;
      default: 
         return -ENOTTY;
	}
	return 0;
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
