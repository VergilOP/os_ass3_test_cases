#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 4096

bool addValues(int fd){
	int writeCount;
	char buffer[BUFFER_SIZE];

	for(int i=0; i<1000; i++){
		sprintf(buffer, "%d\n",i);
		writeCount = write(fd, buffer,strlen(buffer));
		if(writeCount<0){
			printf("!!Failed writing string %s to device\n", buffer);
			perror("Writing part 2 failed");
			close(fd);
			return false;
		}
	}

	return true;

}

bool readValues(int fd){	
	int readCount;
	char recieve[BUFFER_SIZE];

	for(int i=0;i<1000; i++){
		readCount = read(fd, recieve, BUFFER_SIZE);
		recieve[readCount]='\0';

		if(readCount < 0){
			printf("!!Failed reading from device\n");
			perror("Reading part 3 failed");
			close(fd);
			return false;
			
		}else{

			if(i!=atoi(recieve)){
				printf("!!Strings didn't match. Expected %d got %s.\n", i, recieve);
				return false;
			} 
	        
		}
	}

	return true;	
}

int main(){

	int fd;
	bool success=false;

	printf("*****Part One*****\n");
	printf("Opening device\n");

	fd = open("/dev/chardev", O_RDWR);
	if(fd < 0){
		printf("Failed opening file\n ");
		
	}
	else{
		printf("File opened succesfully\n");
	}
	printf("***Part One Complete***\n\n");


	printf("*****Part Two*****\n");
	printf("Writing strings which contains numbers 0 to 999\n");
	success = addValues(fd);
	if(!success) return -1;
	printf("***Part Two Complete***\n\n");

	printf("*****Part Three*****\n");
	printf("Reading strings which should have numbers 0 to 999\n");
	success = readValues(fd);
	if(!success) return -1;
	printf("***Part Three Complete***\n\n");

	

    return 0;
}