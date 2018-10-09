/* ======================================================================
	FileName: sleepExample.c
   Purpose: To understand how to stop execution of a process/thread
				for a while.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc sleepExample.c -o sleepExample
		$ ./sleepExample [<sleepingTime>]
  	----------------------------------------------------------------------
	Necessary Library Function:
	A. sleep()
		A.1 Task:
			To make the calling thread sleep until specified seconds have 
			elapsed or a signal arrives which is not ignored.

		A.2 Synopsis:
			unsigned int sleep(unsigned int seconds);

		A.3 Return Value:
			** Zero if the requested time has elapsed. Or,
			** The number of seconds left to sleep, if the call was 
				interrupted by a signal handler.

		A.4 Header File:
			#include <unistd.h>		
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
#include <string.h> 	// For strlen().
#include <unistd.h>	// For write().
#include <stdio.h> 	// For sprintf(). 

int main(int argc, char *argv[]){
	int i, j;
	char msg[300] = "Bangladesh is a green land. Trees and plants grow easily everywhere. It could be a beautiful country if we do not throw garbage here and there. For our bad habit, this country looks dirty specially in the uban area. It is our responsibility to treat our motherland like our own mother.";
	char ch[3];
	int time;

	switch(argc){
		case 1:
			time = 1;
		break;
		case 2:
			time = atoi(argv[1]);
		break;
		default:
			sprintf(msg, "Usage: ./sleepExample <sleepingTime_Sec>\n");
			write(1, msg, strlen(msg) + 1);
			exit(EXIT_FAILURE);
		break;
	}

	/* Print character of 'msg' one-by-one with a pause. */
	j = strlen(msg);
	for(i = 0; i < j; i++ ){
		sprintf(ch, "%c\n", msg[i]);
		write(1, ch, 3);
		sleep(time);	
	}

	exit(EXIT_SUCCESS);
}

