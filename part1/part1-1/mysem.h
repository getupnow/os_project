#ifndef SEM_H_
#define SEM_H_

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/syscall.h>
#include<pthread.h>
#include<time.h>
#include<sys/shm.h>
#include<string.h>
#include<fcntl.h>

#define gettid() syscall(__NR_gettid)

#define BUFFERNUM  64      // num of buffer in the circular buffers 
#define DATASIZE   64      // num of bytes in a buffer

#define USABLE   123
#define FINISHED 789  // buffer status : finished

typedef struct buff
{
	char data[DATASIZE];
	int used;           // num of bytes used
	int status;         
	int next;           // id of next shared memory
	
} Buffer;

union semun
{
	int val;	// value for SETVAL in semctl
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo * __buf;
};

int create_sem(int size);          // create semaphore set

void destroy_sem(int semid);       

void setval_sem(int semid,int index,int val); 	// set value for semaphore

void P(int semid,int index);

void V(int semid,int index);

#endif // SEM_H_


