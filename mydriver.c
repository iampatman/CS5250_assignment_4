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

#define BUF_LEN 1024*4
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
char* msg_Ptr = NULL;
bool readFirstChar = false;
int onebyte_open(struct inode *inode, struct file *filep)
{
printk(KERN_NOTICE "Open device\n");
msg_Ptr = onebyte_data;
return 0; // always successful
}

int onebyte_release(struct inode *inode, struct file *filep)
{
return 0; // always successful
}


int times = 0;


ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{


	printk(KERN_NOTICE "data: %s\n", msg_Ptr);
	if (*msg_Ptr == 0)
		return 0;

	put_user(*(msg_Ptr), buf);
	msg_Ptr++;
	
	

	printk(KERN_NOTICE "Read from device %d\n", times);

	times++;

	return 1;
/*please complete the function on your own*/
}
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{	

	printk("count: %d\n", count);
	printk("bf: %c\n", &buf);
	*(onebyte_data) = buf;
	get_user(onebyte_data[i], buf + f_pos);
	printk("data: %s\n", onebyte_data);
	if (count>1024*4-1){
		printk(KERN_NOTICE "No more memory\n");
		return -28;
	} 
	
    	return 1;
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
	onebyte_data = kmalloc(sizeof(char)*BUF_LEN, GFP_KERNEL);
	if (!onebyte_data) {
		onebyte_exit();
		return -ENOMEM;
	}
	sprintf(onebyte_data, "Init content of 4MB device. Please use echo and cat to test. Author: Nguyen Trung");
	printk(KERN_ALERT "This is a 4MB device module\n");
	return 0;
}



static void onebyte_exit(void)
{
	if (onebyte_data) {
		kfree(onebyte_data);
		onebyte_data = NULL;
	}
	// unregister the device
	unregister_chrdev(MAJOR_NUMBER, "onebyte");
	printk(KERN_ALERT "4MB device module is unloaded\n");
}



MODULE_LICENSE("GPL");
module_init(onebyte_init);
module_exit(onebyte_exit);

