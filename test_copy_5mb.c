#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define BUFFER_LENGTH 4*1024 *1024              ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
 

char* open_file(){
	char *buffer = NULL;
	size_t size = 0;

	/* Open your_file in read-only mode */
	FILE *fp = fopen("5mb.txt","r");

	/* Get the buffer size */
	fseek(fp, 0, SEEK_END); /* Go to end of file */
	size = ftell(fp); /* How many bytes did we pass ? */

	/* Set position of stream to the beginning */
	rewind(fp);

	/* Allocate the buffer (no need to initialize it with calloc) */
	buffer = malloc((size + 1) * sizeof(*buffer)); /* size + 1 byte for the \0 */

	/* Read the file into the buffer */
	fread(buffer, size, 1, fp); /* Read 1 chunk of size bytes from fp into buffer */

	/* NULL-terminate the buffer */
	buffer[size] = '\0';

	/* Print it ! */
//	printf("%s\n", buffer);
	return buffer;
}

char* open_5mb_file() {
   printf("Opening file...\n");
   FILE* fptr = fopen("5mb.txt","r");
   char* buff = malloc(5 * 1024 * 1024 * sizeof(char));
   if (fptr == NULL){
       printf("Error! opening file");
       exit(1);
   }

   char line[150];

   fgets(buff, 5*1024*1024, fptr);
//   printf("String read: %s\n", buff);
   fclose(fptr); 
   return buff;
}

int main(){
   int ret, fd;
   char stringToSend[BUFFER_LENGTH];

   char* strFromFile = open_file();
   //printf("String read from file: %s\n", strFromFile);
   printf("Starting device test code example...\n");
   fd = open("/dev/four", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }

   printf("String length: %d\n", strlen(strFromFile));
   
   ret = write(fd, strFromFile, strlen(strFromFile)); // Send the string to the LKM
   printf("Return value of write function: %d\n", ret);
   if (ret < 0){
      perror("Failed to write the message to the device.");
      return errno;
   }

 
   printf("Reading from the device...\n");
   ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
   if (ret < 0){
      perror("Failed to read the message from the device.");
      return errno;
   }
   printf("Return value of read function: %d\n", ret);
//   puts(receive);
   printf("The head of the device is: \n");
   for (int j=0; j < 100;j++){
	printf("%c",receive[j]);
   }
   printf("\n");
   printf("The tail of the device is:\n");
   for (int j=100; j > 0; j--){
	printf("%c",receive[ret-j]);
   }	

   printf("\nThe leftover starts from: ");
   for (int j=ret+1; j < ret + 100;j++){
	printf("%c",strFromFile[j]);
   }
   printf("\nEnd of the program\n");
   return 0;
}



