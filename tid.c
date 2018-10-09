/* ======================================================================
	FileName: tid.c
   Purpose: To understand thread id.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc tid.c -o tid -lpthread
		$ ./tid <threadNo>
		[e.g. $./tid 10]
	----------------------------------------------------------------------
	Notes:
	A. There is no glibc wrapper function for the system call gettid().
		Therefore, syscall() is needed to use this system call.
		syscall(SYS_gettid);
	----------------------------------------------------------------------
	Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	25/5/2018
	======================================================================
*/
#include	<stdlib.h>			//	exit()
#include	<unistd.h>			// getpid()
#include	<pthread.h>			//	pthread_create(), pthread_self()
#include	<stdio.h>			// perror(), printf()
#include	<sys/syscall.h>	//	gettid()

void *threadFunction(void *argv)	{
	int i;
	pid_t PID, LWPID;
	pthread_t TID;

	i = *(int *)argv;

	PID = getpid();
	LWPID = syscall(SYS_gettid);
	TID = pthread_self();
	printf("Thread-%d's PID: %d\n", i, PID);
	printf("Thread-%d's LWPID: %d\n", i, LWPID);
	printf("Thread-%d's TID: %ld\n", i, TID);

	return 0;
}

int main(int argc, char *argv[])	{
	pid_t PID, LWPID;
	pthread_t TID, *TIDPtr;
	int n, i, *TIndex, status;
	char msg[30];
	
	/*	Get the number of sub-threads from the CLI. */
	if(argc == 1)	{
		printf("Usage: ./tid <nThread>\n");
		exit(EXIT_FAILURE);
	}
	n = atoi(argv[1]);
	TIDPtr = (pthread_t *)malloc(n * sizeof(pthread_t));
	TIndex = (int *)malloc(n * sizeof(int));

	/*	Create n sub-threads. */
	for(i = 0; i < n; i++)	{
		TIndex[i] = i + 1;
		status = pthread_create(&TIDPtr[i], NULL, &threadFunction, &TIndex[i]);
		sprintf(msg,"Error during creating Thread-%d by pthread_create()", TIndex[i]);
		if(status != 0)	
			perror(msg);
	}

	/*	Print IDs of Main Thread. */
	PID = getpid();
	LWPID = syscall(SYS_gettid);
	TID = pthread_self();
	printf("Main Thread's PID: %d\n", PID);
	printf("Main Thread's LWPID: %d\n", LWPID);
	printf("Main Thread's TID: %ld\n", TID);

	/*	Wait for the termination of the sub-threads. */
	for(i = 0; i < n; i++)
		pthread_join(TIDPtr[i], NULL);
	
	exit(EXIT_SUCCESS);
}
