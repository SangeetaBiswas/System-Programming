/* ======================================================================
	FileName: perrorExample.c
	Extension of: mainArguments.c

   Purpose: To understand how we can generate appropriate error message
				by perror().

	Tasks:
		A. Take two filenames from the CLI as arguments.
		B. Copy content of one file into another file.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc perrorExample.c -o perrorExample
		$ ./perrorExample [OPTIONS]
	----------------------------------------------------------------------
	Necessary Library Functions:
	A. perror()
		A.1 Task:
			To print a message on standard error describing the last error
			encountered during a call to a system or library function.

		A.2 Synopsis:
			void perror(const char *str);

		A.3 Return Value:
			Nothing.

		A.4 Header File:
			#include <stdio.h>
	----------------------------------------------------------------------
	Notes:
	A. When  a  system  call fails, it usually returns -1 and sets the 
		variable errno to a value describing what went wrong. perror() 
		serves to translate this error code into human-readable form.
	
	B. If str is NULL an error message corresponding to the current 
		value of errno and a new-line are printed. For example,

		fd = open("MyDoc", O_RDONLY);
		if(fd == -1)
			perror(NULL);

		The above code may generate the follwoing error messages:
		B.1. When there is no 'MyDoc' file:

      	No such file or directory.

		B.2. When user does not have read permission:

			Permission denied

	C. If str is not NULL and *str is not a null byte ('\0'), the 
		argument string str is printed, followed by a colon and a blank.   
		Then an error message corresponding to the current value of 
		errno and a new-line are printed.

		fd = open("mydoc", O_RDONLY);
		if(fd == -1)
			perror("Error during open ()");

		The above code may generate the follwoing error messages:
		C.1. When there is no 'MyDoc' file:

      	Error during open (): No such file or directory.

		C.2. When user does not have read permission:

			Error during open (): Permission denied

	D. It is recommended to include the name of the function that 
		incurred the error, into the argument string, str.

	E.	A common mistake is to do:

		if (systemCall() == -1) {
			printf("systemCall() failed\n");
			if (errno == ...) { ... }
		}
		
		** Where  errno no longer needs to have the value it had upon 
			return from systemCall() (i.e., it may have been changed 
			by the printf()).
       
		** If the value of errno should be preserved across a 
			library call, it must be saved:

			if (systemCall() == -1) {
				int errsv = errno;
				printf("systemCall() failed\n");
				if (errsv == ...) { ... }
			}

	F. A list of error messages defined on Linux can be found:
		$ man errno
	----------------------------------------------------------------------
   Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	15/9/2017
	======================================================================
*/

#include <stdio.h> 	// for sprintf().
#include	<fcntl.h>	// for open().
#include	<unistd.h> 	// for read(), write(), close().
#include	<string.h>	// for strcpy(), strlen().
#include	<stdlib.h>	// for exit().

#define BUFF_SIZE 1024
#define MSG_SIZE 80

int main(int argc, char *argv[]){
	char staticMsg[MSG_SIZE], msgBuffer[BUFF_SIZE];
	int rdSz, wrSz;
	int rdFd, wrFd;	

	switch(argc){
		case 1:
			sprintf(staticMsg, "Usage: %s [copyFromFileName] [copyToFileName]\n", argv[0]);
			wrSz = write(1, staticMsg, strlen(staticMsg) + 1);	
			if(wrSz == -1){
				perror("Error during write()");
				exit(EXIT_FAILURE);
			}		
		break;

		case 2:
			strcpy(staticMsg, "Too few arguments\n");
			wrSz = write(1, staticMsg, strlen(staticMsg) + 1);
			if(wrSz == -1){
				perror("Error during write()");
				exit(EXIT_FAILURE);
			}	
			sprintf(staticMsg, "Usage: %s [copyFromFileName] [copyToFileName]\n", argv[0]);
			wrSz = write(1, staticMsg, strlen(staticMsg) + 1);	
			if(wrSz == -1){
				perror("Error during write()");
				exit(EXIT_FAILURE);
			}			
		break;

		case 3:
			/* ============= Copy data from Copy-From file ============= */	
			rdFd = open(argv[1], O_RDONLY);
			if(rdFd == -1){
				//perror(NULL); // Not very good style.
				perror("ERROR!!...during open() copyFromFile...");
				exit(EXIT_FAILURE); 
			}
			rdSz = read(rdFd, msgBuffer, BUFF_SIZE);
			if(rdSz == -1){
				perror("Error during read()");
				exit(EXIT_FAILURE);
			}

			msgBuffer[rdSz] = '\0';	
			close(rdFd);

			/* ============= Paste data into Copy-To file ============= */
			wrFd = open(argv[2], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if(wrFd == -1){
				//perror(NULL); // Not very good style.
				perror("ERROR!!...during open() copyToFile...");
				exit(EXIT_FAILURE); 
			}
			wrSz = write(wrFd, msgBuffer, strlen(msgBuffer) + 1);
			if(wrSz == -1){
				perror("Error during write()");
				exit(EXIT_FAILURE);
			}	
			cs = close(wrFd);
			if(cs == -1){
				perror("Error during close()");
				exit(EXIT_FAILURE);
			}
		break;

		default:
			strcpy(staticMsg, "Too many arguments.\n");
			wrSz = write(1, staticMsg, strlen(staticMsg) + 1);
			if(wrSz == -1){
				perror("Error during write()");
				exit(EXIT_FAILURE);
			}	
		break;
	}

	exit(EXIT_SUCCESS);
}
