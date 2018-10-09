/*	======================================================================
	File Name: threadCreation.c
	Purpose: To understand how multi-threading works.
	----------------------------------------------------------------------
	How to Run: 
	A. At one terminal, type
		$ gcc threadCreation.c -o threadCreation -pthread
		$ ./threadCreation

		** We will see a flow of three messages at different order
		5 + 3 = 8
		5 - 3 = 2
		5 - 3 = 2
		5 - 3 = 2
		I am the unstoppable main thread.
		5 + 3 = 8

   B. At another terminal, type
		$ pstree
		
		** We will be able to see a process named 'threadCreation' with two
		threads inside curly brackets.
		├─gnome-terminal-─┬─bash───threadCreation───2*[{threadCreation}]

		$ pstree -p
		├─gnome-terminal-(2026)─┬─bash(2035)───threadCreation(14682)─┬─{threadCreation}(14683)
                             		                                  └─{threadCreation}(14684)
	----------------------------------------------------------------------
	Notes:	
	A.	If we forget to write -pthread, we would get an error message
		similar to the following error message.

		$ gcc threadCreation.c -o threadCreation 
		/tmp/cceFx1jH.o: In function `main':
		threadCreation.c:(.text+0x78): undefined reference to `pthread_create'
		threadCreation.c:(.text+0xb4): undefined reference to `pthread_create'
		collect2: error: ld returned 1 exit status

	B. If we do not use infinite loop, we would not be able to
		trace either process or newly created threads by pstree command.

	C. It is better to open the terminal in full screen mode to see
		the whole tree structure.
   ---------------------------------------------------------------------- 
	Necessary Library Function:
	A. pthread_create()

		A.1. Task: 
			Create two new threads which will execute separately from
			the main thread.

		A.2. Synopsis:
	 		1. Original form:
				int pthread_create(	pthread_t *thread, 
											const pthread_attr_t *attr,
                          			void *(*start_routine) (void *), 
											void *arg
										);

      	2. At the beginning, we can use a simpler form:
				int pthread_create(	pthread_t *thread, 
											NULL,
                          			void *(*start_routine) (void *), 
											NULL
										);
				** After creating a thread successfully, our process will
					get thread's ID at the first parameter.

					The code for each thread needs to be put in a function.
					Third Parameter is the name of that function.

		A.3. Return Value:
			0 on success and an error number on failure.

		A.4. Header File:
			#include <pthread.h>
 	---------------------------------------------------------------------- 
	Sangeeta Biswas
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
   ---------------------------------------------------------------------- 
	14/11/2017
	======================================================================
*/

#include	<pthread.h>	// For pthread_create().
#include	<stdio.h>	// For printf().
#include	<stdlib.h>	// For exit().

void *sub(){
	int sub = 5 - 3;

	while(1){
		printf("Thread-1: 5 - 3 = %d\n", sub);
	}
}

void *sum(){
	int sum = 5 + 3;

	while(1){
		printf("Thread-2: 5 + 3 = %d\n", sum);
	}
}

int main(){
	/*=======================Part-1===================================*/
	/* If we try to run this part (i.e., Part-1) instead of Part-2, 
		we would never be able to run Part-3, even we will get stuck 
		at sum().
	*/ 
	//sum();
	//sub();

	/*=======================Part-2====================================*/
	/* Two threads will be created and they will run parallely
		with Part-3.
	*/
	int tdStatus;
	pthread_t TID_1, TID_2; /* These varaibles will hold the thread IDs,
										if they are successfully created.
									*/
	tdStatus = pthread_create(&TID_1, NULL, sum, NULL);
	if(tdStatus != 0){
		perror("Error during pthread_create()");
		exit(EXIT_FAILURE); // Equivalent of "return 1;".
	}

	tdStatus = pthread_create(&TID_2, NULL, sub, NULL);
	if(tdStatus != 0){
		perror("Error during pthread_create()");
		exit(EXIT_FAILURE);
	}

	/*=======================Part-3====================================*/
	/* This part will help two threads, created in Part-2, run as long 
		as they want. If we do not use an infinite loop here, we would not
		be able to trace two new threads. Because main thread will 
		terminate after creating two threads. Two threads will be 
		destroyed when main thread will teminate by reaching at 
		"exit(EXIT_SUCCESS)". Smart way is to use pthread_join().
	*/
	while(1){
		printf("I am the unstoppable main thread.\n");
	}

	exit(EXIT_SUCCESS); // Equivalent of "return 0;".
}
