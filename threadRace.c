/* ======================================================================
	FileName: threadRace.c
   Purpose: To understand how data race happens in a multi-threaded 
				process.
	----------------------------------------------------------------------	
	Tasks:
	A. Create two threads, where:
		A.1. one thread is increasing a global variable.
		A.2. one thread is decreasing that global variable.
		A.3. main thread is not doing any operation on that global 
				variable.

	B. Use a decent finite or an infinite loop in all threads.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc threadRace.c -o threadRace -pthread
		$ ./threadRace

	B. Run threadRace few times to observe some confusing behavior 
		such as
	
		Sample Output:
		****************	
		57. I am Thread-1. My task is to increase a. Before (a = a + 1): 0
		57. I am Thread-1. My task is to increase a. After (a = a + 1): 1
		58. I am Thread-1. My task is to increase a. Before (a = a + 1): 1
		56. I am Thread-2. My task is to decrease a. After (a = a - 1): -47
		57. I am Thread-2. My task is to decrease a. Before (a = a - 1): 1
		122. I am Main Thread. I am not changing a. I found a: -47
		57. I am Thread-2. My task is to decrease a. After (a = a - 1): 1
		58. I am Thread-1. My task is to increase a. After (a = a + 1): 2
		59. I am Thread-1. My task is to increase a. Before (a = a + 1): 1
		59. I am Thread-1. My task is to increase a. After (a = a + 1): 2
	----------------------------------------------------------------------
	Notes: 
	A. Data race is an confusing sitution which can occur in a
		multi-threaded process.

	B. A data race occurs when:
		B.1.	two or more threads in a single process try to write on the
				same memory location (i.e., global variables) concurrently, 
		B.2. 	the threads are not using any exclusive locks to control 
				their accesses to that memory.

	C. For the data race, unexpected results may generate which could
		sometimes arise confusing and dangerous situations. 

	D. The thread scheduler can switch between threads at any time.
		Therefore, we cannot predict the order in which the threads will 
		attempt to access the shared data. As a result, it may happen 
		that multiple threads unintensionally get involved in a "race"
		(UNHEALTHY COMPETITION).

	E. Race condition is problematic specialy when there is a 
		"check-then-act" block based on a shared variable. 

		For example, one thread at first "check" if the value of a shared 
		varible is X, then "act" to do something that depends on the value 
		being X. But if another thread does something to the value of the
		shared variable in between the "check" and the "act", 
		funny/dangerous situation may arise.

		In One Thread
		****************
		if (a == 5){
			b = a * 2;
			printf("b = %d", b); 
		}

		In Another Thread
		*******************
		a = a + 10;

		It may happen that we can see b = 30 instead of b = 10. Is it
		Funny/ Scary??
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

/* Global Variable which will be the reason
	of data race condition.
*/
int a;

/* =========== Thread Functions ============== */
void *thread1 (void *arg) {
	/* 
		Local variables having common names 
		with other threads. 
	*/
	int i;
	char msg[MSG_SZ];	

	for (i = 1; i <= N; i++) {
		/*
			** The following instructions can be simply written as:
			-------------------------------------------------------
			printf("%d. I am Thread-1. My task is to increase a. Before (a = a + 1): %d\n", i, a);	
			a = a + 1;
			printf("%d. I am Thread-1. My task is to increase a. After (a = a + 1): %d\n", i, a);
			-------------------------------------------------------
		*/
		sprintf(msg, "%d. I am Thread-1. My task is to increase a. Before (a = a + 1): %d\n", i, a);	
		write(1, msg, strlen(msg) + 1);
		a = a + 1;
		sprintf(msg, "%d. I am Thread-1. My task is to increase a. After (a = a + 1): %d\n", i, a);
		write(1, msg, strlen(msg) + 1);

		/*
			** Check if there is any change, if we write:
			-------------------------------------------------------
			printf("%d. I am Thread-1. My task is to increase a. Before (a++): %d\n", i, a);	
			a++;
			printf("%d. I am Thread-1. My task is to increase a. After (a++): %d\n", i, a);
			-------------------------------------------------------
		*/
	}	
}

void *thread2 (void *arg) {
	/* 
		Local variables having common names 
		with other threads. 
	*/
	int i;
	char msg[MSG_SZ];	

	for (i = 1; i <= N; i++) {
		sprintf(msg, "%d. I am Thread-2. My task is to decrease a. Before (a = a - 1): %d\n", i, a);	
		write(1, msg, strlen(msg) + 1);
		a = a - 1;
		sprintf(msg, "%d. I am Thread-2. My task is to decrease a. After (a = a - 1): %d\n", i, a);
		write(1, msg, strlen(msg) + 1);
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
		To make the main thread wait so that other 
		thread can be properly observed.
	*/
	for(i = 1; i <= N; i++){
		sprintf(msg, "%d. I am Main Thread. I am not changing a. I found a: %d\n", i, a);
		write(1, msg, strlen(msg) + 1);
	}

	exit(EXIT_SUCCESS);	// similar to "return 0;"
}



