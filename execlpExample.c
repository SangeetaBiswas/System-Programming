/* ======================================================================
	FileName: execlpExample.c

   Purpose: To  understand how execlp() works.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc execlpExample.c -o execlpExample
		$ ./execlpExample [OPTIONS]

		** By 'pstree' command we will see a process named 'subtraction'
			instead of 'execExample' is running as a child process
			of 'bash'. 				
	----------------------------------------------------------------------
	Necessary Library Function:
	A. execlp()
		A.1 Task:
			To replace the current process image with a new process image.

		A.2 Synopsis:
			int execlp(const char *file, const char *arg, ...
                    (char *) NULL);

		A.3 Return Value:
			It returns -1, and errno, only if an error has occurred. 

		A.4 Header File:
			#include <unistd.h>
	----------------------------------------------------------------------
	Notes:
	A. execlp() is a member of exec family, whose other members are: 
		A.1. 	execl(), execle(), execv(), execvp(), execvpe() 
				[Library Functions]

		A.2. 	execve() [System Call]
				
		A.3. 	After 'man', if we write '2', we will get information
				about a system call. And if we write '3', we will get
				infomation about a libary function. 
				$ man 2 execve
				$ man 3 exec

	B. All members of exec family almost do the same thing. All of them
		replace the current process image with a new process image. The
		main difference is how arguments are passed to them.

		B.1. 	execl(), execlp(), and execle() take a list of NULL
				terminated strings, e.g.,
				arg0, arg1, arg2,......,NULL

		B.2. 	execv(),  execvp(), execvpe() functions and execve() system
				call need an array of pointers to null-terminated strings.

		B.3. 	The first argument (ie., arg0), by convention, should  
				point to the filename associated with the file being 
				executed.

	C. Example:
		execlp("./subtraction", "subtraction", NULL);
		execlp("./subtraction", "subtraction", "12", NULL);
		execlp("./subtraction", "subtraction", "12", "5", NULL);

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
#include <unistd.h>	// For execlp(), getpid().
#include <stdlib.h>	// For exit().

#define BUFFER_SZ 80

int main(int argc, char *argv[]){
	int exeSt;

	/* Executable(i.e., exe) files of child processes. */
	char exeFile[BUFFER_SZ] = "/home/sangeeta/OS/Code/InstantNoodlesFestival-2017/subtraction"; 
	
	printf("Hello, I am Process-%d\n", getpid());
	switch(argc){
		case 1:
			exeSt = execlp(exeFile, exeFile, NULL);
		break;
		case 2:
			exeSt = execlp(exeFile, exeFile, argv[1], NULL);
		break;
		case 3:
			exeSt = execlp(exeFile, exeFile, argv[1], argv[2], NULL);
		break;
		default:
			printf("Usage: ./execlpExample [<a> <b>]\n");
			exit(EXIT_FAILURE);
		break;
	}
	if(exeSt == -1){
		perror("Error during exec()");
		exit(EXIT_FAILURE);	// Unsuccessful termination
	}
	
	/*	The following instruction will never be executed. If execlp()
		executes sucessfully, it will replace this process image with 
		the process image of the <exeFile>.	If execlp() fails, this 
		process will be terminated.					
	*/
	printf("Nice to meet you.\n"); 

	exit(EXIT_SUCCESS);	//Successful Termination
}

