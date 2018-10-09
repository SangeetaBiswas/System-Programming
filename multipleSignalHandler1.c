/* ======================================================================
	FileName: multipleSignalHandler1.c
	Extension of: signalExample.c

   Purpose: To understand how to handle multiple signals in different
				ways.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc multipleSignalHandler1.c -o multipleSignalHandler1
		$ ./multipleSignalHandler1

	B. Run 'signalSender' at different terminal after it, so that it 
		can receive signals from 'signalSender'. 
	----------------------------------------------------------------------
	Notes:
	A. It is better to be familiar with different options of different 
		signals.
		$ man 7 signal
			OR
		https://en.wikipedia.org/wiki/Signal_(IPC)

	B. Execept SIGKILL & SIGSTOP, all other signals can be handled by
		the user defined handler function installed inside the process, 
		i.e.,	signal(<sigName>, <signalHandler>);

	C. If a signal handler is not installed for a particular signal, 
		the default handler set by the OS is used. 

	D. Beside handling a signal by a user defined handler function,
		a signal can be
		D.1. 	ignored
					signal(<sigName>, SIG_IGN);
		D.2.	handled by default actions
					signal(<sigName>, SIG_DFL);			 
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

int i;

/* We can handle multiple signals by installing a single handler function 
	and using the value of 'sig', since 'sig' receives the signal number.
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
		case SIGCHLD:
			sprintf(msg,"Signal NO-%d is sent to me. Strange!!! I do not have any child.\n", sig);
			write(1, msg, strlen(msg) + 1);
		break;
		/* The following case will not be executed, if SIG_IGN/SIG_DFL
			option is used as second argument of signal(). SIG_IGN stands 
			for ignoring a signal and SIG_DFL stands for performing default
			action after receiving a signal.
		*/
		case SIGCONT:
			sprintf(msg,"Signal NO-%d is sent to me. Stopped process will not execute anymore.\n", sig);
			write(1, msg, strlen(msg) + 1);
		break;
		/* The following case will never be executed, since SIGSTOP
 			is handled by OS. It cannot be caught/blocked/ignored.
		*/
		case SIGSTOP:
			sprintf(msg,"Signal NO-%d is sent to me. I am ignoring it.\n", sig);
			write(1, msg, strlen(msg) + 1);
		break;
	}
}

int main(){
	pid_t myPID;
	char msg[30];

	/* To print PID so that we can pass it to 'signalSender'. */
	myPID = getpid();
	sprintf(msg,"My PID-%d.\n", myPID);
	write(1, msg, strlen(msg) + 1);

	/* To install a single signal handler for multiple signals. */
	signal(SIGUSR1, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGCHLD, signalHandler);

	/* SIGSTOP cannot be caught/blocked/ignored. Therefore, 
		it is unnecessary to install a signal handler for it.
	*/
	signal(SIGSTOP, signalHandler);
	
	/* Using of SIG_DFL for a signal is equivalent to not 
		installing any signal handler for that signal, because 
		in both cases default action is performed.  
	*/
	signal(SIGCONT, SIG_DFL);	

	while(1){
		sprintf(msg,"i = %d\n", i);
		write(1, msg, strlen(msg) + 1);
		i++;
		sleep(1);
	}

	exit(EXIT_SUCCESS);
}
