#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define BUFFER_SIZE 4096

bool addValues(int fd){
	int writeCount;
	char buffer[BUFFER_SIZE];
	for(int i=0; i<10; i++){
		sprintf(buffer, "%d",i);
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
	for(int i=0;i<10; i++){
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

void *addValues_1(void *arg){
	int fd = *(int*) arg;
	int writeCount;
	char buffer[BUFFER_SIZE];
	for(int i=0; i<500; i++){
		sprintf(buffer, "%d",i);
		writeCount = write(fd, buffer,strlen(buffer));
		if(writeCount<0){
			printf("!!Failed writing string %s to device\n", buffer);
			close(fd);
			break;
		}
	}
}

void *addValues_2(void *arg){
	int fd = *(int*) arg;
	int writeCount;
	char buffer[BUFFER_SIZE];
	for(int i=500; i<1000; i++){
		sprintf(buffer, "%d",i);
		writeCount = write(fd, buffer,strlen(buffer));
		if(writeCount<0){
			printf("!!Failed writing string %s to device\n", buffer);
			close(fd);
			break;
		}
	}
}

bool addValues_3(int fd){
	int writeCount;
	char buffer[BUFFER_SIZE];
	for(int i=0; i<1001; i++){
		sprintf(buffer, "%d",i);
		writeCount = write(fd, buffer,strlen(buffer));
	}

	return true;
}

bool addValues_4(int fd){
	int writeCount;
	char buffer[BUFFER_SIZE+1];
	int readCount;
	char recieve[BUFFER_SIZE];
	
	char str_1[BUFFER_SIZE] = "";
	char str_2[BUFFER_SIZE+1] = "";
	
	for(int i=0;i<4*1024-1;i++){
		strcat(str_1,"a");
		strcat(str_2,"b");
	}
	strcat(str_2,"b");
	
	sprintf(buffer, "%s",str_1);
	writeCount = write(fd, buffer,strlen(buffer)-1);
	
	sprintf(buffer, "%s",str_2);
	writeCount = write(fd, buffer,strlen(buffer));
	
	readCount = read(fd, recieve, BUFFER_SIZE);
	recieve[readCount]='\0';
	
	readCount = read(fd, recieve, BUFFER_SIZE);
	recieve[readCount]='\0';
	return true;
}

void *readValues_1(void *arg){
	int fd = *(int*) arg;
	int readCount;
	char recieve[BUFFER_SIZE];
	for(int i=0;i<500; i++){
		readCount = read(fd, recieve, BUFFER_SIZE);
		recieve[readCount]='\0';

		if(readCount < 0){
			printf("!!Failed reading from device\n");
			perror("Reading part 3 failed");
			close(fd);	
			break;		
		}
	}	
}


bool readValues_2(int fd){	
	int readCount;
	char recieve[BUFFER_SIZE];
	for(int i=0;i<1000; i++){
		readCount = read(fd, recieve, BUFFER_SIZE);
		recieve[readCount]='\0';

		if(readCount < 0){
			printf("!!Failed reading from device\n");
			perror("Clear failed");
			close(fd);
			return false;
		}else{

			if(recieve == ""){
				printf("Reach the end.\n");
				return true;
			} 
	        
		}
		
	}

	return true;	
}

void *readValues_3(void *arg){
	int fd = *(int*) arg;
	int readCount;
	char recieve[BUFFER_SIZE];
	for(int i=500;i<1000; i++){
		readCount = read(fd, recieve, BUFFER_SIZE);
		recieve[readCount]='\0';

		if(readCount < 0){
			printf("!!Failed reading from device\n");
			perror("Reading part 4 failed");
			close(fd);	
			break;		
		}
	}	
}

bool readValues_4(int fd){	
	int readCount;
	char recieve[BUFFER_SIZE];
	for(int i=0;i<1001; i++){
		readCount = read(fd, recieve, BUFFER_SIZE);
		recieve[readCount]='\0';

		if(readCount < 0){
			printf("!!Failed reading from device\n");
			perror("Reading part 5 failed");
			close(fd);
			return false;
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
		return -1;
	}
	else{
		printf("File opened succesfully\n");
	}
	printf("***Part One Complete***\n\n");


	printf("*****Part Two*****\n");
	printf("Writing strings which contains numbers 0 to 9\n");
	success = addValues(fd);
	if(!success) return -1;
	printf("***Part Two Complete***\n\n");

	printf("*****Part Three*****\n");
	printf("Reading strings which should have numbers 0 to 9\n");
	success = readValues(fd);
	if(!success) return -1;
	printf("***Part Three Complete***\n\n");


	printf("*****Part Four*****\n");
	printf("Writing and reading strings(Concurrency)\n");
	
	int *arg = malloc(sizeof(*arg));
	*arg = fd;
	
	pthread_t threads[4];
	pthread_create(&threads[0], NULL, (void *) addValues_1, arg);
	pthread_create(&threads[1], NULL, (void *) addValues_2, arg);
	pthread_create(&threads[2], NULL, (void *) readValues_1, arg);
	pthread_create(&threads[3], NULL, (void *) readValues_3, arg);
	
	for (int i = 0; i < 4; i++) {
    		pthread_join(threads[i], NULL);
  	}
	
	printf("***Part Four Complete***\n\n");
	
	printf("*****Clean Start*****\n");
	success = readValues_2(fd);
	if(!success) return -1;
	printf("*****Clean Complete*****\n\n");
	
	printf("*****Part Five*****\n");
	printf("Writing and Reading 1001(length of list)\n");
	
	success = addValues_3(fd);
	if(!success) return -1;
	
	success = readValues_4(fd);
	if(!success) return -1;
	
	printf("***Part Five Complete***\n\n");
	
	printf("*****Part Six*****\n");
	printf("Message less than 4kb\n");
	
	success = addValues_4(fd);
	if(!success) return -1;
	
	printf("***Part Six Complete***\n\n");
	
	close(fd);
    return 0;
}
