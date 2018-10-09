/* ======================================================================
	FileName: multipleSignalHandler2.c
	Extension of: multipleSignalHandler1.c

   Purpose: To understand how to handle multiple signals.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc multipleSignalHandler2.c -o multipleSignalHandler2
		$ ./multipleSignalHandler2

	B. Run 'signalSender' at different terminal after running it, so 
		that it can receive signals from 'signalSender'. 
	----------------------------------------------------------------------
	Notes:		 
	----------------------------------------------------------------------
   Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	16/11/2017
	======================================================================
*/
#include <unistd.h>	//	For getpid(), write().
#include <stdio.h>	//	For sprintf().
#include <string.h>	// For strlen().
#include <stdlib.h>	// For exit().
#include <signal.h>	//	For signal().

#define ACTIVE 1
#define INACTIVE 0

/* Global variables */
int i;	// It will be used as a counter.
int prcState = ACTIVE;	/* It will be used to check whether this 
									process is stopped or doing its operation.
								*/
pid_t myPID;	// To hold ID of this process.

/* We can handle multiple signals by installing by a single handler 
	function and using the value of 'sig', since 'sig' receives the 
	signal number.
*/
void signalHandler(int sig){
	char msg[100];

	switch(sig){
		case SIGUSR1:
			sprintf(msg,"Signal NO-%d is sent to me. I am resetting the value of variable 'i'.\n", sig);
			write(1, msg, strlen(msg) + 1);
			i = 0;
		break;
		case SIGTERM:
			sprintf(msg,"Signal NO-%d is sent to me. I was ordered to be terminated. But I am ignoring it.\n", sig);
			write(1, msg, strlen(msg) + 1);
		break;
	}
}

/* We can have separate signal handler for each signal. The following
	signal handler will handle SIGCONT signal, whose default task is to 
	instruct OS to restart a process stopped by SIGSTOP/ SIGTSTP. But 
	here it is also used to stop a process by the help of a global
	variable 'state' and a signal, SIGSTOP. That means SIGCONT will be
	used to change the state of this process i.e., send this process from 
	sleeping/running state to running/sleeping state.
*/
void sigContHandler(int sig){
	char msg[30];

	if(prcState == INACTIVE){
		prcState = ACTIVE;
		sprintf(msg, "INACTIVE --> ACTIVE\n");
		write(1, msg, strlen(msg) + 1);

	}else{
		prcState = INACTIVE;
		sprintf(msg, "ACTIVE --> INACTIVE\n");
		write(1, msg, strlen(msg) + 1);

		/* To force this process to go to the interruptible 
			sleeping state, i.e., it will awake by a signal 
			such as SIGCONT.
		*/
		kill(myPID, SIGSTOP);	
	}
}

int main(){
	char msg[30];

	/* To print PID so that we can pass it to 'signalSender'. */
	myPID = getpid();
	sprintf(msg,"My PID-%d.\n", myPID);
	write(1, msg, strlen(msg) + 1);

	/* To install the same signal handlers for multiple signals. */
	signal(SIGUSR1, signalHandler);
	signal(SIGTERM, signalHandler);

	/* To install a separate signal handler for SIGCONT.*/
	signal(SIGCONT, sigContHandler);

	while(1){
		sprintf(msg,"i = %d\n", i);
		write(1, msg, strlen(msg) + 1);
		i++;
		sleep(1);
	}

	exit(EXIT_SUCCESS);
}
