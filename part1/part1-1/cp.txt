#include "mysem.h"

int idHead,idTail;
int semid;
char targetfile[20] = {0};

//process: write file(out) from buffer
int pWrite(int out)
{
	nice(100);
	Buffer * head = (Buffer*)shmat(idHead,0,0);

	while(1)
	{
		P(semid,0); // P(full)
		P(semid,2); // P(mutex)

		// finish writing file 
		if(head->status == FINISHED)
		{
			printf("read %d bytes from buffer[%d] and write to file[%s]\n",head->used,idHead,targetfile);
			printf("Finish writing.\n");
			// fwrite((void*)(head->data),head->used,1,out);
			write(out, head->data, head->used);
			close(out);
			V(semid,2);  // V(mutex)
			V(semid,1);  // V(empty)
			return 0;
		}
		write(out, head->data, head->used);
		printf("read %d bytes from buffer[%d] and write to file[%s]\n",head->used,idHead,targetfile);

		idHead = head->next;
		head = (Buffer*)shmat(idHead,0,0);
		V(semid,2);  // V(mutex)
		V(semid,1);  // V(empty)
	}
}



int main(int argc,char *argv[])
{
	int out;
	if((out = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO)) < 0)
	{
		perror("Fail to open writing file.");
		return 1;
	}
	strcpy(targetfile,argv[1]);
	sscanf(argv[2],"%d",&idHead);
	sscanf(argv[3],"%d",&idTail);
	sscanf(argv[4],"%d",&semid);

	pWrite(out);
	return 0;
}
