/* ======================================================================
	FileName: signalSender.c
	Extension of: signalExample.c

   Purpose: To understand how to send signals to other processes.
	Task: Send 5 kinds of signals one by one taking 2 seconds pause.
	----------------------------------------------------------------------
	How to Run: 
	A. Run other processes such as 'multipleSignalHandler1' and
		'multipleSignalHandler2', which are designed to work on signals 
		sent by this process, at different terminals before running 
		'signalSender', so that they can get time to set their signal 
		handlers. 

		Otherwise, signals sent by this process will be wasted.

	B. At a terminal, type
		$ gcc signalSender.c -o signalSender
		$ ./signalSender <PID> <PID>
		[	Say that 'multipleSignalHandler1' and 'multipleSignalHandler2'
			are running and their PIDs are 2244 and 2245 respectively.
			then $ ./signalSender 2244 2245
			and check what is going on.
		]
  	----------------------------------------------------------------------
	Necessary System Call:
	A. kill()
		A.1 Task:
			To send signal to a process or a group of processes.

		A.2 Synopsis:
			int kill(pid_t pid, int sig);

		A.3 Return Value:
			 ** On success, at least one signal was sent, and zero is 
				 returned.
			 ** On error, -1 is returned.
				
		A.4 Header File:
			#include <signal.h>		
	----------------------------------------------------------------------
	Notes:
	A. A.1.	If (pid > 0), then signal sig is sent to the process 
				with the ID specified by pid.
		A.2. 	If (pid == 0), then sig is sent to every process in  
				the process group of the calling process.
		A.3. 	If (pid == -1), then sig is sent to every process for 
				which the calling process has permission to send signals, 
				except for process 1(systemd/init).
		A.4.	If (pid < -1), then sig is sent to every process in the 
				process group whose ID is -pid.
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
#include <signal.h>	//	For kill().

#define WAIT_T 1

void sendingSignal(pid_t targetPID);

int main(int argc, char *argv[]){
	pid_t myPID, targetPID;
	int i;
	char msg[30];

	/* Send 4 kinds of signals to a specific process. */
	for(i = 1; i < argc; i++){
		targetPID = atoi(argv[i]); /* Get process IDs to 
												whom signals will be 
												sent. 
											*/
		sendingSignal(targetPID);
	}
	/* Send SIGCHLD to all processes. Its main task is to
		notify whether a child process is terminated/stopped
		/continued. Since by default SIGCHLD is ignored, it is 
		safe to send this signal to every other processes in 
		the system. 
	*/
	sprintf(msg,"Sending SIGCHLD...\n");
	write(1, msg, strlen(msg) + 1);
	kill(-1, SIGCHLD);	// -1 is for all permitted processes.
	sleep(WAIT_T);

	/* Kill processes whose PID are provided from the terminal. 
		SIGKILL signal cannot be blocked/caught/ignored.
	*/
	for(i = 1; i < argc; i++){
		targetPID = atoi(argv[i]);
		sprintf(msg,"Going to kill Process-%d\n", targetPID);
		write(1, msg, strlen(msg) + 1);
		kill(targetPID, SIGKILL);
	}

	/* Send a signal to itself. */
	myPID = getpid();
	sprintf(msg,"Good Bye!!! I am going to killing me(:\n");
	write(1, msg, strlen(msg) + 1);
	kill(myPID, SIGKILL);
	
	/* This process has already killed itself before 
		executing the following instructions. 
	*/
	sprintf(msg,"Wow!!! I am still alive:)\n");
	write(1, msg, strlen(msg) + 1);

	exit(EXIT_SUCCESS);
}

/*
	Signal SIGCONT will be sent before sending each other signal. 
	This signal is for directing a process to continue its task 
	if it is stopped for some reasons. We will see that SIGCONT 
	affects 'multipleSignalHandler1' once, while it affects
	'multipleSignalHandler2' a lot by changing its order of 
	execution.
*/
void sendingSignal(pid_t targetPID){
	char msg[30];

	sprintf(msg,"Sending SIGCONT...\n");
	write(1, msg, strlen(msg) + 1);
	kill(targetPID, SIGCONT);
	sleep(WAIT_T);
	sprintf(msg,"Sending SIGUSR1...\n");
	write(1, msg, strlen(msg) + 1);
	kill(targetPID, SIGUSR1);
	sleep(WAIT_T);
	sprintf(msg,"Sending SIGCONT...\n");
	write(1, msg, strlen(msg) + 1);
	kill(targetPID, SIGCONT);
	sleep(WAIT_T);
	sprintf(msg,"Sending SIGTERM...\n");
	write(1, msg, strlen(msg) + 1);
	kill(targetPID, SIGTERM);
	sleep(WAIT_T);
	sprintf(msg,"Sending SIGCONT...\n");
	write(1, msg, strlen(msg) + 1);
	kill(targetPID, SIGCONT);
	sleep(WAIT_T);
	sprintf(msg,"Sending SIGSTOP...\n");
	write(1, msg, strlen(msg) + 1);
	kill(targetPID, SIGSTOP);
	sleep(WAIT_T);
	sprintf(msg,"Sending SIGCONT...\n");
	write(1, msg, strlen(msg) + 1);
	kill(targetPID, SIGCONT);
	sleep(WAIT_T);
}
