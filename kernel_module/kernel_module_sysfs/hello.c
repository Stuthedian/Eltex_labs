#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/init.h>

//based on /samples/kobject/kobject-example.c [linux kernel source, v5.0]

MODULE_AUTHOR("Me");
MODULE_DESCRIPTION("Playing around with kernel modules");
MODULE_LICENSE("GPL");

ssize_t read_hi (struct kobject *kobj, struct kobj_attribute *attr, char *buffer);
ssize_t write_hello(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t size);

static rwlock_t rwlock;
static struct kobject* my_kobject;

static struct kobj_attribute my_attribute =
	__ATTR(TeSt, 0664, read_hi, write_hello); 

static struct attribute *attrs[] = 
{
	&my_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};
static struct attribute_group attr_group = { .attrs = attrs, };

int __init init_module()
{        
	int retval;
  	pr_info("Hello world!\n");
	my_kobject = kobject_create_and_add("TeSt", NULL);
	if(my_kobject == NULL)
	{
		pr_info("Failed to create kobject!\n");
		return 1;
	}
	retval = sysfs_create_group(my_kobject, &attr_group);
	if (retval)
		kobject_put(my_kobject);
	rwlock_init(&rwlock);
	return retval;
}

void __exit cleanup_module()
{
	kobject_put(my_kobject);
	pr_info("Goodbye world!\n");
}

ssize_t write_hello(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t size)
{
	write_lock(&rwlock);
	pr_info("%s", buffer);
	write_unlock(&rwlock);
	return (strlen(buffer) + 1);
}

ssize_t read_hi (struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	int rslt;
	read_lock(&rwlock);
	rslt = sprintf(buffer, "%s\n", "hi!");
	read_unlock(&rwlock);
	return rslt; 
}
