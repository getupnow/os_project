#include<stdio.h>
#include<unistd.h>
#include<linux/kernel.h>
#include<sys/syscall.h>

int main(int argc, char *argv[])
{
	// test syscall 336 -- mycp
	// return 0
	// copy argv[1] to argv[2]
	long ret = syscall(336, argv[1], argv[2]);
	printf("return code is: %ld \n", ret);
	return 0;
}
