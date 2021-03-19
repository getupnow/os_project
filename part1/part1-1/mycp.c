#include "mysem.h"

// process id for reading and writing
int idRead = 0,idWrite = 0;
char sourcefile[30] = {0};
char targetfile[30] = {0};

// semaphore id
int semid = 0; 

// head and tail of circular buffer
int idHead = 0, idTail = 0;

// create circular buffers
// return 1 if fail to create
// return 0 if successfully create
int circularBuffer()
{
	key_t shmkey;
	if((shmkey = ftok("./",'a')) == (key_t)-1)
	{
		perror("ftok error.");
		return 1;
	}

	if((idHead = shmget(shmkey,sizeof(Buffer),IPC_CREAT|0660)) <= 0)
	{
		perror("shmget error.");
		return 1;
	}

	Buffer * tmp = (Buffer*)shmat(idHead,0,0);
	if((int64_t)tmp == -1)
	{
		perror("shmat error.");
		return 1;
	}
	tmp->status = USABLE;
	tmp->used = 0;

	for(int i = 0; i < BUFFERNUM; i++)
	{
		key_t shmkey;
		if((shmkey = ftok("./",i)) == (key_t)-1)
		{
			perror("ftok error.");
			return 1;
		}
		int idshm;
		if((idshm = shmget(shmkey,sizeof(Buffer),IPC_CREAT|0660)) <= 0)
		{
			perror("shmget error.");
			return 1;	
		}
		tmp->next = idshm;
		tmp = (Buffer *)shmat(idshm,0,0);
		if((int64_t)tmp == -1)
		{
			perror("shmat error.");
			return 1;
		}
		tmp->status = USABLE;
		tmp->used = 0;
	}
	idTail = idHead;
	tmp->next = idHead;
	return 0;
}

// first parameter: name of executable file
// second param   : source file
// thrid param    : target file
int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		printf("Usage: %s <source-file> <target-file>\n",argv[0]);
		return 1;
	}
	
	strcpy(sourcefile,argv[1]);
	strcpy(targetfile,argv[2]);
	
	char sidHead[20] = {0};
	char sidTail[20] = {0};
	char ssemid[20]  = {0};

	// create circular buffers
	if(circularBuffer())
	{
		printf("Fail to create circular buffer.");
		return 1;
	}

	// create three semaphores
	// first sem: num of full buffer
	// senond   : num of empty buffer
	// thrid    : mutex
	semid = create_sem(3);     
	
	setval_sem(semid,0,0);          // num of full buffer
	setval_sem(semid,1,BUFFERNUM);  // num of empty buffer
	setval_sem(semid,2,1);          // mutex
	
	sprintf(sidHead,"%d",idHead);  // integer to string
	sprintf(sidTail,"%d",idTail);
	sprintf(ssemid,"%d",semid);
	
	// read process : read data from file and write to buffer
	if((idRead = fork()) == -1)
	{
		perror("fork error.");
		return 1;
	}
	if(idRead == 0)
	{
		printf("Executing get.out....\n");
		char * argv[] = { "./get",sourcefile,sidHead,sidTail,ssemid,0};
		execv("./get",argv);
	}

	// write process : read date from buffer and write to file
	if((idWrite = fork()) == -1)
	{
		kill(idRead,SIGKILL);
		perror("fork error.");
		return 1;
	}
	if(idWrite == 0)
	{
		printf("Executing put.out...\n");
		char * argv[] = { "./put",targetfile,sidHead,sidTail,ssemid,0};
		execv("./put",argv);
	}

	// wait to finish
	waitpid(idRead,0,0);
	waitpid(idWrite,0,0);
	destroy_sem(semid);

	return 0;
}

