#include "mysem.h"

int create_sem(int size)
{
	int semid = semget(IPC_PRIVATE,size,IPC_CREAT| 0666);
	if(semid < 0)
		printf("create sem error.\n");
	return semid;
}

void destroy_sem(int semid)
{
	int res = semctl(semid,0,IPC_RMID,0);  // delete semaphore set
//	if(res < 0)
//		printf("destroy sem error.\n");
	return;
}

void setval_sem(int semid,int index,int val) 	// set value to semaphore 
{
	union semun semopt;
	semopt.val = val;
	semctl(semid,index,SETVAL,semopt);
	return ;
}

void P(int semid,int index)
{
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op  = -1;
	sem.sem_flg = 0;
	semop(semid,&sem,1);
	return ;
}

void V(int semid,int index)
{
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op  = 1;
	sem.sem_flg = 0;
	semop(semid,&sem,1);
	return ;
}
