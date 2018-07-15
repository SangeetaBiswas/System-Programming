/* ======================================================================
	FileName: cpuAssign.c
	Purpose:	To understand how to distributes CPU to a process and its
				threads.
	Tasks:	1.	Take n from the CLI
			2.	Create n threads, therefore there will be (n+1) threads.
			3.	Assign CPU-0 to odd number threads and CPU-1 to even
				number threads. Kernel will decides CPU of the main
				thread.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc cpuAssign.c -o cpuAssign -lpthread
		$ ./cpuAssign <threadNo>
		[e.g. $./cpuAssign 10]
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
#include	<sched.h>		//	sched_setaffinity()
#include	<stdlib.h>		//	exit()
#include	<stdio.h>		//	printf(), perror()
#include	<pthread.h>		//	pthread_create()

void *subThread(void *arg)	{
	int tIndex, c;
	cpu_set_t mask;
	size_t sz;
	pthread_t TID;
	
	/*	Set CPU-0 to the even number thread and CPU-1
		to the odd number thread.
	*/
	TID = gettid();
	tIndex = *(int *)arg;
	c = tIndex % 2;
	CPU_ZERO(&mask);
	CPU_SET(c, &mask);
	sz = sizeof(cpu_set_t);
	sched_setaffinity(, sz, mask);

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
		printf("Usage: ./cpuInfo <threadNo>");
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
