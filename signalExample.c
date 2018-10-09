/* ======================================================================
	FileName: signalExample.c
	Extension of: sleepExample.c

   Purpose: To understand how to handle a signal.
	Task: Only one signal named SIGTERM is used.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc signalExample.c -o signalExample
		$ ./signalExample 

	B. B.1.	Say PID of 'signalExample' is 9933.
				At another terminal, send SIGTERM signal to this 
				process by typing
				$ kill -15 9933
						OR
				$ kill -SIGTERM 9933

		B.2. Kill this process by typing
				$ kill -9 9933
  	----------------------------------------------------------------------
	Necessary System Call:
	A. signal()
		A.1 Task:
			To install a handler function in order to handle a signal.

		A.2 Synopsis:
 			sighandler_t signal(int signum, sighandler_t handler);

		A.3 Return Value:
			 ** On success, the previous value of the signal handler.
			 ** On error, SIG_ERR.
				
		A.4 Header File:
			#include <signal.h>		
	----------------------------------------------------------------------
	Notes:
	A. A.1.	A signal is a way of generating software interrupt.
		A.2. 	It is used to notify a process or a specific thread of a
				process about an event.
		A.3. 	It is considered as a limited form of interprocess
				communication.

	B.	B.1.	All signals are defined as macro constants in <signal.h> 
				header file. 
		B.2.	The name of the macro constant consists of a "SIG" prefix 
				followed by a mnemonic name for the signal.
		B.3. 	Some macro constants for signals are:
				SIGCONT:	to continue executing, if stopped.
				SIGHUP:	to hangup.
				SIGPIPE:	to write on a pipe with no one to read it.
				SIGSTOP:	to stop execution (cannot be caught or ignored).
				SIGTERM: to terminate.
				SIGUSR1:	User-defined signal 1.
				SIGUSR2:	User-defined signal 2.	
			
	C. C.1.	A signal handler needs to be installed in order to 
				customize the operations after the arrival of a signal.
		C.2. 	If a signal handler is not installed for a particular 
				signal, the default handler set by the OS is used. 
		
	D.	D.1. 	A signal handler is a function which is called by a process 
				when the corresponding signal occurs. 
		D.2.	Normal activities of a process are suspended until the 
				signal handler returns or calls longjmp().
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
#include <stdlib.h>	// For exit().
#include <signal.h>	// For signal().
#include <string.h> 	// For strlen().
#include <unistd.h>	// For write().
#include <stdio.h> 	// For sprintf(). 

/* This signal handler will get value of the parameter 'sig'
	from the first argument of signal(), i.e., in this case 'SIGTERM'.
*/
void signalHandler(int sig){
	char *msg = "OOPS!!! I was ordered to be terminated. But I am ignoring it.\n";
	write(1, msg , strlen(msg) + 1);
}

int main(){
	pid_t myPID;
	int i, j;
	char longMsg[300] = "Bangladesh is a green land. Trees and plants grow easily everywhere. It could be a beautiful country if we do not throw garbage here and there. For our bad habit, this country looks dirty specially in the uban area. It is our responsibility to treat our motherland like our own mother.";	
	char shortMsg[30], ch[3];

	/* To print PID which can be used to send it a signal later on. */
	myPID = getpid();
	sprintf(shortMsg,"My PID-%d.\n", myPID);
	write(1, shortMsg, strlen(shortMsg) + 1);

	/* To install a signal handler to handle SIGTERM signal.*/
	signal(SIGTERM, signalHandler);

	/* Print character of 'msg' one-by-one with 1 second pause. */
	j = strlen(longMsg);
	for(i = 0; i < j; i++ ){
		sprintf(ch, "%c\n", longMsg[i]);
		write(1, ch, 3);
		sleep(1);	
	}

	exit(EXIT_SUCCESS);
}

