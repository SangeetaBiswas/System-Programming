/* ======================================================================
	FileName: mainArguments.c
	Extension of: openCloseExample.c

	Purpose:	To understand how parameters are passed from the CLI 
				to the function, main().

	Tasks:
		A. Take two filenames from the CLI as arguments.
		B. Copy content of one file into another file.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc mainArguments.c -o mainArguments
		$ ./mainArguments [OPTIONS]
	----------------------------------------------------------------------
	Necessary Library Functions:
	A. sprintf()
		A.1 Task:
			To write to the character string str.

		A.2 Synopsis:
			int sprintf(char *str, const char *format, ...);

		A.3 Return Value:
			** The number of characters printed (excluding the null byte 
				used at the end of str), on success and 
			** A negative value on error.

		A.4 Header File:
			#include <stdio.h>
	----------------------------------------------------------------------
	Notes:
	A.	Generally, the C programs, we write, cannot take arguments from
		the command line, e.g.,
		$ ./helloWorld

	B.	However, the commands, we type in CLI, can take many arguments.
		$ gcc helloWorld.c -o helloWorld
		
		Here, gcc takes 3 arguments:
			1. helloWorld.c 
			2. -o 
			3. helloworld

	C.	Command-line arguments are very useful. It helps us run a 
		command with our desired options. Actually, C functions 
		wouldn't be very useful if we couldn't ever pass arguments 
		to them.

	D.	In order to pass information to our program from CLI, we just
		need to do it via parameters of the main() function, i.e.,

		int main(){

			return 0;
		}

		........needs to be written as:

		int main(int argc, char *argv[]){

			return 0;
		}

		where:
			argc: argument count
			argv: argument vector

	E.	'argc' contains the number of arguments passed to the program. 
		'argv' contains a set of strings in an one dimensional array.

		The first room of 'argv' contains command's name therefore
		argc is equal to 1 even no argument is passed. That means, for

		$ ./helloWorld
			argc = 1
			argv[0] = "helloWorld"

		And for
		$ gcc helloWorld.c -o helloWorld
			argc = 4
			argv[0] = "gcc"
			argv[1] = "helloWorld.c"
			argv[2] = "-o"
			argv[3] = "helloWorld"
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

#include	<stdio.h> 	// for sprintf().
#include	<fcntl.h>	// for open().
#include	<unistd.h> 	// for read(), write(), close().
#include	<string.h>	// for strcpy(), strlen().
#include	<stdlib.h>	// for exit().

#define BUFF_SIZE 1024
#define MSG_SIZE 80

int main(int argc, char *argv[]){
	char staticMsg[MSG_SIZE], msgBuffer[BUFF_SIZE];
	int rdSz;
	int rdFd, wrFd;	// File descriptors.

	switch(argc){
		case 1:
			sprintf(staticMsg, "Usage: %s [copyFromFileName] [copyToFileName]\n", argv[0]);
			write(1, staticMsg, strlen(staticMsg) + 1);			
		break;

		case 2:
			strcpy(staticMsg, "Too few arguments\n");
			write(1, staticMsg, strlen(staticMsg) + 1);
			sprintf(staticMsg, "Usage: %s [copyFromFileName] [copyToFileName]\n", argv[0]);
			write(1, staticMsg, strlen(staticMsg) + 1);			
		break;

		case 3:
			/* ============= Copy data from Copy-From file ============= */
			strcpy(staticMsg, "ERROR!!...during open() copyFromFile...not clear what happened.\n");	
			rdFd = open(argv[1], O_RDONLY);
			if(rdFd == -1){
				write(1, staticMsg, strlen(staticMsg) + 1);
				exit(EXIT_FAILURE); // Equivalent to "return 1;".
			}
			rdSz = read(rdFd, msgBuffer, BUFF_SIZE);
			msgBuffer[rdSz] = '\0';	/*	Without this line, we will get strange 
										charecters at the end of the file.
									*/
			close(rdFd);

			/* ============= Paste data into Copy-To file ============= */
			strcpy(staticMsg, "ERROR!!...during open() copyToFile...not clear what happened.\n");	
			wrFd = open(argv[2], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if(wrFd == -1){
				write(1, staticMsg, strlen(staticMsg) + 1);
				exit(EXIT_FAILURE); 
			}
			write(wrFd, msgBuffer, strlen(msgBuffer) + 1);
			close(wrFd);
		break;

		default:
			strcpy(staticMsg, "Too many arguments.\n");
			write(1, staticMsg, strlen(staticMsg) + 1);
		break;
	}

	exit(EXIT_SUCCESS);
}
