#include<stdio.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>

int main()
{
	// test syscall 335 -- helloworld
	// return 0
	long ret = syscall(335);
	printf("return code is: %ld\n", ret);
	return 0;
}
