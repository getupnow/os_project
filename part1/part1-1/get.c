#include "mysem.h"

int idHead,idTail;
int semid;
char sourcefile[20] = {0};

//process: read file(in) to buffer
int pRead(int in)
{
	nice(100);
	Buffer *tail = (Buffer*)shmat(idTail,0,0);

	while(1)
	{
		P(semid,1); // P(empty)
		P(semid,2); // P(mutex)
	
		int r;
		if((r = read(in, tail->data, DATASIZE)) == 0)
		{
			printf("read %d bytes from file[%s] and write to buffer[%d]\n",r,sourcefile,idTail);
			printf("Finish reading.\n");
			tail->status = FINISHED;
			tail->used   = r;
			close(in);
			V(semid,2);
			V(semid,0);
			return 0;	
		}
		printf("read %d bytes from file[%s] and write to buffer[%d]\n",r,sourcefile,idTail);
		tail->used = r;

		idTail = tail->next;
		tail = (Buffer*)shmat(idTail,0,0);
		V(semid,2); // V(mutex)
		V(semid,0); // V(full)
	}
}


int main(int argc,char *argv[])
{
	int in;
	if(( in = open(argv[1], O_RDONLY)) < 0 ) // open file -- read only
	{
		perror("Fail to open reading file.");
		return 1;	
	}
	strcpy(sourcefile,argv[1]);
	sscanf(argv[2],"%d",&idHead);
	sscanf(argv[3],"%d",&idTail);
	sscanf(argv[4],"%d",&semid);
	
	pRead(in);
	return 0;
}
