#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#define MAJOR_NUMBER 61


int onebyte_open(struct inode *inode, struct file *filep);
int onebyte_release(struct inode *inode, struct file *filep);
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t onebyte_write(struct file *filep, const char *buf,
size_t count, loff_t *f_pos);
static void onebyte_exit(void);

struct file_operations onebyte_fops = {
	read: onebyte_read,
	write: onebyte_write,
	open: onebyte_open,
	release: onebyte_release
};

char *onebyte_data = NULL;

int onebyte_open(struct inode *inode, struct file *filep)
{
printk(KERN_NOTICE "Open device");
return 0; // always successful
}

int onebyte_release(struct inode *inode, struct file *filep)
{
return 0; // always successful
}



ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_NOTICE "Read from device");
	if (*onebyte_data == 0)
		return 0;

	/* 
	 * Actually put the data into the buffer 
	 */
	put_user(*(onebyte_data), buf);
	onebyte_data++;
	return 1;
/*please complete the function on your own*/
}
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
//	printk( KERN_NOTICE "Write to device");
  //  	if( copy_from_user(buf, onebyte_data, 1) != 0 )
    //    	return -EFAULT;    

    	return -EINVAL;
/*please complete the function on your own*/
}


static int onebyte_init(void)
{
	int result;
	// register the device
	result = register_chrdev(MAJOR_NUMBER, "onebyte",
	&onebyte_fops);
	if (result < 0) {
		return result;
	}
	// allocate one byte of memory for storage
	// kmalloc is just like malloc, the second parameter is
	// the type of memory to be allocated.
	// To release the memory allocated by kmalloc, use kfree.
	onebyte_data = kmalloc(sizeof(char), GFP_KERNEL);
	if (!onebyte_data) {
		onebyte_exit();
		// cannot allocate memory
		// return no memory error, negative signify a failure
		return -ENOMEM;
	}
	// initialize the value to be X
	*onebyte_data = 'X';
	printk(KERN_ALERT "This is a onebyte device module\n");
	return 0;
}



static void onebyte_exit(void)
{
	// if the pointer is pointing to something
	if (onebyte_data) {
	// free the memory and assign the pointer to NULL
		kfree(onebyte_data);
		onebyte_data = NULL;
	}
	// unregister the device
	unregister_chrdev(MAJOR_NUMBER, "onebyte");
	printk(KERN_ALERT "Onebyte device module is unloaded\n");
}



MODULE_LICENSE("GPL");
module_init(onebyte_init);
module_exit(onebyte_exit);

