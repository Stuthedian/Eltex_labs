#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/stat.h>
#include <linux/proc_fs.h>

MODULE_AUTHOR("Me");
MODULE_DESCRIPTION("Playing around with kernel modules");
MODULE_LICENSE("GPL");

ssize_t write_hello(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
ssize_t read_hi (struct file *file, char __user *buf, size_t count, loff_t *ppos);

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.read = read_hi,
	.write = write_hello,
};
static struct proc_dir_entry* proc_test;

int init_module()
{        
	umode_t mode = 	S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH  | S_IWOTH;
  	pr_info("Hello world!\n");
	proc_test = proc_create("proc_test", mode, NULL, &fops);

	return 0;//success
}

void cleanup_module()
{
	proc_remove(proc_test);
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
