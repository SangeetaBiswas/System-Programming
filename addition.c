/* ======================================================================
	FileName: addition.c
   Purpose: To use it by members of exec() family such as:
				execve()/execlp()/execl().
	
	Task:
		1. Take two numbers and add them.
		2. Use the default value, if any argument is missing.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc addition.c -o addition
		$ ./addition [OPTIONS]

			OR

	B. Use it by a member of exec() family.
	----------------------------------------------------------------------
	Necessary Library Function:
	A. atoi()
		A.1 Task:
			To convert a string to an integer.

		A.2 Synopsis:
			int atoi(const char *nptr);

		A.3 Return Value:
			The converted value.

		A.4 Header File:
			#include <stdlib.h>
	----------------------------------------------------------------------
	Note:
	A. We will see the same Process ID of the process in which the 
		executable file of this process is called by a member of exec()
		family.
	B. kill the process of this program after finishing investigation.
	----------------------------------------------------------------------
   Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	10/9/2017
	======================================================================
*/
#include <stdio.h>	// For printf().
#include <stdlib.h>	// For atoi().
#include <unistd.h>	// For getpid().

#define DEFAULT_VALUE 5

int main(int argc, char *argv[]){
	int a, b;

	switch(argc){
		case 1:
			a = DEFAULT_VALUE;
			b = DEFAULT_VALUE;
		break;
		case 2:
			a = atoi(argv[1]);
			b = DEFAULT_VALUE;
		case 3:
			a = atoi(argv[1]);
			b = atoi(argv[2]);
		break;
		default:
			printf("Usage: ./addition [<a> <b>]\n");
		break;
	}

	printf("My PID: %d. My task is to add two numbers.\n", getpid());
	printf("----------------------------------------------\n");
	printf("%d + %d = %d\n", a, b, a+b);

	/* Only for tracing this process by 'top' or 
		'pstree' command.
	*/
	while(1) 
		;

	return 0;
}
