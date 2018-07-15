/* ======================================================================
	FileName: cpuInfo.c
	Purpose:	To understand which CPU is assigned to a process and its
				threads.
	Tasks:		1. Take n from the CLI
				2. Create n threads, therefore there will be (n+1) threads.
				3.	Check which thread is running at which CPU.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc cpuInfo.c -o cpuInfo -lpthread
		$ ./cpuInfo <threadNo>
		[e.g. $./cpuInfo 10]
	----------------------------------------------------------------------
	Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	24/5/2018
	======================================================================
*/
#define _GNU_SOURCE
#include	<sched.h>		//	sched_getcpu()
#include	<stdlib.h>		//	exit()
#include	<stdio.h>		//	printf(), perror()
#include	<pthread.h>		//	pthread_create()

void *subThread(void *arg)	{
	int tIndex, c;

	tIndex = *(int *)arg;

	/*	Check in which CPU a thread is running. */
	while(1)	{
		c = sched_getcpu();
		printf("Thread-%d is running in CPU-%d\n", tIndex, c);
	}
}

int main(int argc, char *argv[]) {
	pthread_t *TIDPtr;
	int c, i, n, status;
	int *tIndexPtr;

	/*	Read number of threads from the CLI. */
	if(argc == 1)	{
		printf("Usage: ./cpuInfo <threadNo>\n");
		exit(EXIT_FAILURE);
	}
	n = atoi(argv[1]);
	TIDPtr = (pthread_t *)malloc(n * sizeof(pthread_t));
	tIndexPtr = (int *)malloc(n * sizeof(int));

	/*	Create n threads. */
	for(i = 0; i < n; i++)	{
		tIndexPtr[i] = i + 1;
		status = pthread_create(&TIDPtr[i], NULL, &subThread, &tIndexPtr[i]);
		if(status != 0)
			perror("Error during pthread_create()"); 
	}

	/*	Check in which CPU the main thread is running. */
	while(1)	{
		c = sched_getcpu();
		printf("MainThread running in CPU-%d\n",c);
	}

	exit(EXIT_SUCCESS);
}
