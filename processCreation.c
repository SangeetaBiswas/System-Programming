/* ======================================================================
	FileName: processCreation.c
   Purpose: To understand how to create a child process.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc processCreation.c -o processCreation 
		$ ./processCreation. 
	----------------------------------------------------------------------
	Sample Outputs:
	Run it several times and try to understand how parent process and
	and child process execute.

	A. $ ./processCreation
		I am Process-12696. My child is Process-12697.
		I am Process-12697. My parent is Process-12696.
		
		** Expected output. Child process completed execution before its
			parent

	B. $ ./processCreation
		I am Process-7291. I am Process-7292. My parent is Process-7291.
		My child is Process-7292.
		
		** A bit unexpected output. However, it happened because the  
			scheduler took away CPU from the parent process and assigned  
			to the child process.

	C. $ ./processCreation
		I am Process-12694. My child is Process-12695.
		I am Process-12695. My parent is Process-989.

		** A bit unexpected output. However, It happened because the 
			parent process completed its execution and died before the 
			child process. 'upstart' process became the parent of the 
			orphaned child process. Therefore, different PIDs were printed 
			by parent as its own PID (12694) and child as its parent's 
			PID (989).
   ----------------------------------------------------------------------
	Necessary System Calls:
	A. fork()
		A.1 Task:
			To create a child.

		A.2 Synopsis:
			pid_t fork(void);

		A.3 Return Value:
			** On success, the PID of the child process is returned in 
				the parent, and 0 is returned in the child.  
			** On failure, -1 is returned in the  parent, no child process 
				is created, and errno is set appropriately.

		A.4 Header File:
			#include <unistd.h>		
	----------------------------------------------------------------------
	Notes:
	A. fork() creates a new process by duplicating the calling process.  
		A.1.	The new process is referred to as the child process.  
		A.2. 	The  calling  process is referred to as the parent process.

	B. The child process is an exact duplicate of the  parent  process  
		except for the following points:
		B.1. 	The child has its own unique process ID, different from
				parent's ID.
		B.2. 	The child process and the parent process run in separate 
				memory spaces.

	C. The child's parent process ID is the same as the parent's process
		ID.

	D. Both child and parent processes execute the same instructions
		after fork() in parallel. Therefore, it is necessary to make a 
		boundary between instructions sets for these processes by
		if-else.
	----------------------------------------------------------------------
   Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	10/10/2017
	======================================================================
*/
#include <unistd.h>	// For fork(), write().
#include <stdio.h>	// For sprintf().
#include <string.h> 	// For strlen().
#include <stdlib.h> 	// For exit().

#define MSG_SZ 200

int main(){
	pid_t myPID, parentPID, childPID;
	char msg[MSG_SZ];

	/* 
		Create a new process by the system call, fork(). 
	*/
	childPID = fork(); 

	/* 
		If Error occurred, the following part will be executed. 
		It is better not to write any instructions before 
		checking whether child process is created or not by 
		fork(). 
	*/
	if(childPID < 0){
		perror("Error during fork()\n");
		exit(EXIT_FAILURE); 
	}

	/* 
		Child process will execute the following part
		whereas its Parent process will escape it.
   */
	if(childPID == 0){
		myPID = getpid();
		sprintf(msg, "I am Process-%d. ", myPID);
		write(1, msg, strlen(msg) + 1);

		/*
			It is possible that parent process dies before
			getppid() is able to send parent's PID to child.
			In that case, 'upstart' process becomes the new
			parent of the orphaned child in Ubuntu 16.04.
	
			In some systems initd/systemd can be the parent
			of orphaned children.

			Therefore, we may see different PIDs printed by
			parent as its own PID and child as its parent PID.  
		*/
		parentPID = getppid();
		sprintf(msg, "My parent is Process-%d.\n", parentPID);
		write(1, msg, strlen(msg) + 1);
	}

	/* 
		Parent process will execute the following part
		whereas its Child process will escape it.
   */
	else{ 
		myPID = getpid();
		sprintf(msg, "I am Process-%d. ", myPID);
		write(1, msg, strlen(msg) + 1);
		sprintf(msg, "My child is Process-%d.\n", childPID);
		write(1, msg, strlen(msg) + 1);
	}

	exit(EXIT_SUCCESS);
}
