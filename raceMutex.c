/* ======================================================================
	FileName: raceMutex.c
   Purpose: To understand how data race condition can be avoided by 
				mutual exclusion.
	----------------------------------------------------------------------	
	Tasks:
	A. Create two threads, where:
		A.1. one thread is increasing a global variable.
		A.2. one thread is decreasing that global variable.
		A.3. main thread is not doing any operation on that global 
				variable.

	B. Use a decent finite or an infinite loop in all threads.

	C. Use mutex to avoid other thread changing shared data when one 
		thread is working.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc raceMutex.c -o raceMutex -pthread
		$ ./raceMutex

	B. Run raceMutex few times to observe some confusing behavior 
		such as
	
		Sample Output:
		****************	
		196. I am Main Thread. I am not changing a. I found a: 93
		94. I am Thread-1. My task is to increase a. Before (a = a + 1): 93
		94. I am Thread-1. My task is to increase a. After (a = a + 1): 94
		197. I am Main Thread. I am not changing a. I found a: 94
		1. I am Thread-2. My task is to decrease a. Before (a = a - 1): 94
		1. I am Thread-2. My task is to decrease a. After (a = a - 1): 93
	----------------------------------------------------------------------
	Necessary Library Functions:
	A.	pthread_mutex_lock()
		A.1 Task:
			To lock a mutex object in order to protect shared resources.
		
			** If the mutex is already locked, the calling thread shall 
				be blocked until the mutex becomes available. 

		A.2 Synopsis:
			int pthread_mutex_lock(pthread_mutex_t *mutex);

		A.3 Return Value:
			0 on success and -1 on error.

		A.4 Header File:
			#include <pthread.h>

	B.	pthread_mutex_unlock()
		B.1 Task:
			To release a mutex object.

			** If one or more threads are waiting to lock the mutex, 
				pthread_mutex_unlock() causes one of those threads to 
				return from pthread_mutex_lock() with the mutex object 
				acquired. If no threads are waiting for the mutex, the 
				mutex unlocks with no current owner.

		B.2 Synopsis:
			int pthread_mutex_unlock(pthread_mutex_t *mutex);

		B.3 Return Value:
			0 on success and -1 on error.

		B.4 Header File:
			#include <pthread.h>
	----------------------------------------------------------------------
	Notes: 
	A. mutex--> MutEx --> Mutual Exclusion

	B. Mutual Exclusion is a property of concurrency control, which is 
		introduced for preventing data race conditions.

	C. By mutex one thread of execution never enters its critical 
		section when another concurrent thread of execution enters 
		its own critical section. 

	D. Critical section is a segment of code in a thread where the 
		thread works on shared data. For example, in critical section
		a thread can:
		D.1. 	change a shared variable, 
		D.2.	update a shared table,
		D.3.	write a shared file, 
		D.4. 	print after	reading a shared	variable, and so on.  

	E. By mutex we can set an entrance point and an exit point before
		and after the critical section. They will act as gurads of
		the critical sections. Each thread needs a permission before 
		entering the critical section.
	----------------------------------------------------------------------
   Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	25/11/2017
	======================================================================
*/

// Standard Libraries
#include <pthread.h>	// For pthread_create().
#include <stdio.h>	// For sprintf().
#include <unistd.h>	//	For write().
#include <string.h>	// For strlen().
#include <stdlib.h>	// For exit().


#define N_THREADS 2
#define N 200
#define MSG_SZ 200

/* 
	Global Variable which could be the reason
	of data race condition.
*/
int a;

/* 
	Global variable which will be used for solving
	data race situation.
*/
pthread_mutex_t key;

/* =========== Thread Functions ============== */
void *thread1 (void *arg) {
	/* 
		Local variables having common names 
		with other threads. 
	*/
	int i;
	char msg[MSG_SZ];	

	/*
		We need to figure out at first what is our critical section,
		and then we need to use lock-&-unlock functions.
		------------------------------------------
		pthread_mutex_lock(&key);	
			//	Critical Section
		pthread_mutex_unlock(&key);	
		------------------------------------------

		** The position of lock-&-unlock have effect on output.

		** Check what would happen if we set lock-&-unlock at Point-1 
			or Point-3 instead of Point-2.
	*/
	//pthread_mutex_lock(&key);	//	Lock at Point-1
	for (i = 1; i <= N; i++) {
		pthread_mutex_lock(&key);	//	Lock at Point-2
		sprintf(msg, "%d. I am Thread-1. My task is to increase a. Before (a = a + 1): %d\n", i, a);	
		write(1, msg, strlen(msg) + 1);
		//pthread_mutex_lock(&key);	//	Lock at Point-3
		a = a + 1;
		//pthread_mutex_unlock(&key);	//	Unlock at Point-3
		sprintf(msg, "%d. I am Thread-1. My task is to increase a. After (a = a + 1): %d\n", i, a);
		write(1, msg, strlen(msg) + 1);
		pthread_mutex_unlock(&key);	//	Unlock at Point-2
	}
	//pthread_mutex_unlock(&key);	//	Unlock at Point-1	
}

void *thread2 (void *arg) {
	/* 
		Local variables having common names 
		with other threads. 
	*/
	int i;
	char msg[MSG_SZ];	

	for (i = 1; i <= N; i++) {
		pthread_mutex_lock(&key);
		sprintf(msg, "%d. I am Thread-2. My task is to decrease a. Before (a = a - 1): %d\n", i, a);	
		write(1, msg, strlen(msg) + 1);
		a = a - 1;
		sprintf(msg, "%d. I am Thread-2. My task is to decrease a. After (a = a - 1): %d\n", i, a);
		write(1, msg, strlen(msg) + 1);
		pthread_mutex_unlock(&key);
	}	
}

/* =========== Main Function ============== */
int main() {
	/* 
		Local variables having uncommon names 
		with other threads. 
	*/
	int status; 
	pthread_t TID[N_THREADS];
	/* 
		Local variables having common names 
		with other threads. 
	*/
	int i;	
	char msg[MSG_SZ];

	a = 0; // Global variable initialization.

	status = pthread_create(&TID[0], NULL, thread1, NULL);
	if (status != 0) {
		perror("Error during pthread_create()");
		exit(EXIT_FAILURE);
	}

	status = pthread_create(&TID[1], NULL, thread2, NULL);
	if (status != 0) {
		perror("Error during pthread_create()");
		exit(EXIT_FAILURE);	// similar to "return 0;"
	}

	/* 
		To make the main thread waiting so that other 
		thread can be properly observed.
	*/
	for(i = 1; i <= N; i++){
		sprintf(msg, "%d. I am Main Thread. I am not changing a. I found a: %d\n", i, a);
		write(1, msg, strlen(msg) + 1);
	}

	exit(EXIT_SUCCESS);	// similar to "return 0;"
}



