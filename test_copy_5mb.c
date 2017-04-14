#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define BUFFER_LENGTH 4*1024               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
 
char* open_5mb_file() {
   printf("Opening file...\n");
   FILE* fptr = fopen("5mb.txt","r");
   char* buff = malloc(5 * 1024 * sizeof(char));
   if (fptr == NULL){
       printf("Error! opening file");
       exit(1);
   }
   fgets(buff, 5*1024, fptr);
   printf("String read: %s\n", buff);
   fclose(fptr); 
   return buff;
}

int main(){
   int ret, fd;
   char stringToSend[BUFFER_LENGTH];

   char* strFromFile = open_5mb_file();
   printf("String read from file: %s\n", strFromFile);
   printf("Starting device test code example...\n");
   fd = open("/dev/four", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }
   printf("Type in a short string to send to the kernel module:\n");
   scanf("%[^\n]%*c", stringToSend);                // Read in a string (with spaces)
   printf("Writing message to the device [%s].\n", stringToSend);
   printf("String length: %d\n", strlen(stringToSend));
   
   ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
   printf("Return value of write function: %d\n", ret);
   if (ret < 0){
      perror("Failed to write the message to the device.");
      return errno;
   }
 
   printf("Press ENTER to read back from the device...\n");
   getchar();
 
   printf("Reading from the device...\n");
   ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
   if (ret < 0){
      perror("Failed to read the message from the device.");
      return errno;
   }
   printf("The received message is: [%s]\n", receive);
   printf("End of the program\n");
   return 0;
}



