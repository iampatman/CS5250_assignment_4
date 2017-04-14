#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <sys/ioctl.h>  
//needed for IO things. Attention that this is different from kernel mode 
int lcd; 
#define SCULL_IOC_MAGIC  'k' 
#define SCULL_HELLO _IO(SCULL_IOC_MAGIC,   1) 
#define MAJOR_NUM 100

#define IOCTL_SET_MSG _IOR(MAJOR_NUM, 0, char *)

#define IOCTL_GET_MSG _IOR(MAJOR_NUM, 1, char *)
void test() 
{ 
  int k, i, sum; 
  char s[3]; 
 
  memset(s, '2', sizeof(s)); 
  printf("test begin!\n"); 
 
      k = write(lcd, s, sizeof(s)); 
  printf("written = %d\n", k); 
   k = ioctl(lcd, 1); 
  printf("result = %d\n", k); 
     
} 


void ioctl_set_msg(int file_desc, char *message)
{
	int ret_val;

	ret_val = ioctl(file_desc, IOCTL_SET_MSG, message);

	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
}

void ioctl_get_msg(int file_desc)
{
	int ret_val;
	char message[100];

	/* 
	 * Warning - this is dangerous because we don't tell
	 * the kernel how far it's allowed to write, so it
	 * might overflow the buffer. In a real production
	 * program, we would have used two ioctls - one to tell
	 * the kernel the buffer length and another to give
	 * it the buffer to fill
	 */
	ret_val = ioctl(file_desc, IOCTL_GET_MSG, message);

	if (ret_val < 0) {
		printf("ioctl_get_msg failed:%d\n", ret_val);
		exit(-1);
	}

	printf("get_msg message:%s\n", message);
}
int main(int argc, char **argv) 
{ 
  lcd = open("/dev/four", O_RDWR); 
  if (lcd == -1) { 
      perror("unable to open lcd"); 
      exit(EXIT_FAILURE); 
  } 
 	char *msg = "Message passed by ioctl NGUYEN BUI AN TRUNG A0148376U\n";
 
test(); 
	ioctl_set_msg(lcd, msg);
 ioctl_get_msg(lcd);

  close(lcd); 
  return 0; 
} 
