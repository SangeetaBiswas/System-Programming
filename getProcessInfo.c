/* ======================================================================
	FileName: getProcessInfo.c
   Purpose: To get information about a process using different  
				system calls.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc getProcessInfo.c -o getProcessInfo
		$ ./getProcessInfo
	----------------------------------------------------------------------	
	Necessary System Calls:
	A.	getpid()
		A.1 Task:
			To get the process ID of the calling process.

		A.2 Synopsis:
			pid_t getpid(void);

		A.3 Return Value:
			It is always successful and returns process ID.

		A.4 Header File:
			#include <unistd.h>

	B.	getppid()
		B.1 Task:
			To the process ID of the parent of the calling process.

		B.2 Synopsis:
			pid_t getpid(void);

		B.3 Return Value:
			Process ID on success and 0 when the caller's parent is 
			in a different PID namespace.

		B.4 Header File:
			#include <unistd.h>

	C.	getpriority()
		C.1 Task:
			To obtain the  scheduling  priority (i.e., niceness)
			of  the process, process group, or user, as indicated 
			by which and who.

			** It is necessary to clear the external variable errno 
				prior  to  the  call, then  check  it  afterward  to  
				determine  if -1 is an error or a legal priority 
				value.

			** The default priority is 0; lower priorities cause 
				more favorable scheduling.

		C.2 Synopsis:
			int getpriority(int which, id_t who);

			** The value 'which' is one of PRIO_PROCESS, PRIO_PGRP, 
				or PRIO_USER, and 'who' is interpreted relative to 
				'which'.

		C.3 Return Value:
			It returns the highest priority (lowest numerical 
			value) enjoyed by any of the specified processes.

		C.4 Header File:
			#include <sys/resource.h>

	D.	setpriority()
		D.1 Task:
			To set the  scheduling  priority (i.e., niceness),
			of  the process process group, or user, as 
			indicated by 'which' and 'who'.

			** It sets the priorities of all of the specified 
				processes to the specified value.  

		D.2 Synopsis:
			int setpriority(int which, id_t who, int prio);
			
			** The 'prio' argument is a value in the range 
				-20 to 19. The default priority is 0; lower 
				values mean higher prioity.

		D.3 Return Value:
			0 on success and -1 on error.

		D.4 Header File:
			#include <sys/resource.h>
	
	E.	nice()
		E.1 Task:
			To increase the  nice value for the calling process.
			
			** nice() Vs. setpriority()
				--> 	'nice()' increases priority of the current process,
						whereas 'setpriority()' can change priority of
						other processes.  

		E.2 Synopsis:
			int nice(int inc);

			** A higher 'inc' value means a low priority.  
			** Only the superuser may specify a negative 'inc', 
				or priority decrease.

		E.3 Return Value:
			The nice value on success and -1 on error.

		E.4 Header File:
			#include <unistd.h>
	----------------------------------------------------------------------
   Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	20/10/2017
	======================================================================
*/


#include <unistd.h>			// For getpid(), getppid().
#include <stdio.h>			// For sprintf().
#include <string.h>			// For strlen().
#include <sys/resource.h>	// For getpriority(), setpriority().
#include <stdlib.h>			// For exit().

#define MSG_SZ 200

int main(){
	pid_t myPID, myPPID;
	char msg[MSG_SZ];
	int prio;

	myPID = getpid();
	myPPID = getppid();
	sprintf(msg, "I am Process: %d. My parent's PID: %d\n", myPID, myPPID);
	write(1, msg, strlen(msg) + 1);

	prio = getpriority(PRIO_PROCESS, myPID);
	sprintf(msg, "My default niceness: %d.\n", prio);
	write(1, msg, strlen(msg) + 1);

	setpriority(PRIO_PROCESS, myPID, 8);	
	prio = getpriority(PRIO_PROCESS, myPID);
	sprintf(msg, "After setting 8 by setpriority(), my current niceness: %d\n", prio);
	write(1, msg, strlen(msg) + 1);
	
	nice(2);
	prio = getpriority(PRIO_PROCESS, myPID);
	sprintf(msg, "After executing nice(2), my current niceness: %d\n", prio);
	write(1, msg, strlen(msg) + 1);

	/*
		The following instructions will not change the current niceness,
		since current niceness is 10 higher than 2.
	*/
	setpriority(PRIO_PROCESS, myPID, 2);
	prio = getpriority(PRIO_PROCESS, myPID);
	sprintf(msg, "After executing setpriority(2), my current priority: %d\n", prio);
	write(1, msg, strlen(msg) + 1);	
	
	/*
		Since the valid range of niceness is -20 to 19, so we cannot increase
		niceness more than 19.
	*/
	nice(12);
	prio = getpriority(PRIO_PROCESS, myPID);
	sprintf(msg, "After executing nice(12), my current priority: %d\n", prio);
	write(1, msg, strlen(msg) + 1);

	/* 
		To trace this process by top, pstree, cat /proc/PID/maps.
	*/
	sprintf(msg, "\n!!! Please kill me when you are done.!!!\n");
	write(1, msg, strlen(msg) + 1);
	while(1);	

	exit(EXIT_SUCCESS);
}


