/* ======================================================================
	FileName: openCloseExample.c
	Extension of: readWriteExample.c

	Purpose:  To understand how open(), close() system calls work.

	Task: To copy contents of a file into another file.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc openCloseExample.c -o openCloseExample
		$ ./openCloseExample
	----------------------------------------------------------------------
	Necessary System Calls:
	A. open()
		A.1 Task:
			To open and possibly create a file.

		A.2 Synopsis:
			int open(const char *pathname, int flags);
			int open(const char *pathname, int flags, mode_t mode);

			** Symbolic constants for some flags:
				O_RDONLY		opening a file only for reading
				O_WRONLY		opening a file only for writing
				O_RDWR		opening a file for both reading and writing
				O_APPEND		opening a file in append mode
				O_CREAT		creating a file if it does not exist.				
				[[ RD: read, WR: write ]]

			** Symbolic constants for some modes:
				S_IRWXU	00700		user (file owner) has read, write, and 
										execute permission. 
				S_IRUSR  00400		user has read permission. 
				S_IRGRP  00040		group has read permission. 
				S_IXOTH  00001		others have execute permission.
				[[ USR/U: user, GRP/G: group, OTH/O: others, R: read, 
					W: write, X: execute ]]

		A.3 Return Value:
			File descriptor (fd) on success and -1 on error.

		A.4 Header File:
			#include <fcntl.h>		

	B. close()
		B.1 Task:
			To close  a file descriptor, so that it no longer refers 
			to any file and may be reused.

		B.2 Synopsis:
			int close(int fd);

		B.3 Return Value:
			0 on success, and -1 on error.

		B.4 Header File:
			#include <unistd.h>
	----------------------------------------------------------------------
	Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	12/9/2017
	======================================================================
*/


#include	<fcntl.h>	// for open().
#include	<unistd.h> 	// for read(), write().
#include	<string.h>	// for strcpy(), strlen().
#include	<stdlib.h>	// for exit().
#include	<stdio.h>

#define BUFF_SIZE 1024
#define MSG_SIZE 80
#define FNAME_SIZE 200

int main(){
	char staticMsg[MSG_SIZE], msgBuffer[BUFF_SIZE];
	char srcFile[FNAME_SIZE], dstFile[FNAME_SIZE]; 
	ssize_t rdSz, wrSz;
	int rdFd, wrFd;	// File descriptors.

	/* ============= To know copy-from File's Name ============ */
	strcpy(staticMsg, "What is copy-from file's name: ");

	/*	
		1 can be replaced by STDOUT_FILENO / stdout
		0 can be replaced by STDIN_FILENO / stdin		

		** For using stdin &  stdout we need 
			#include <stdio.h> 
	*/	
	write(STDOUT_FILENO, staticMsg, strlen(staticMsg) + 1);	
	rdSz = read(STDIN_FILENO, srcFile, FNAME_SIZE);	

	/*	
		read() inserts ENTER key at the last position as '\n'. We need
		to exclude '\n' and include '\0' to make a pure filename. 
	*/
	srcFile[rdSz - 1] = '\0';	

	/* ============= Copy data from copy-from file ============= */
	strcpy(staticMsg, "ERROR!!...during open() srcFile...not clear what happened.\n");	
	rdFd = open(srcFile, O_RDONLY);
	if(rdFd == -1){
		write(1, staticMsg, strlen(staticMsg) + 1);
		exit(EXIT_FAILURE); // Equivalent to "return 1;".
	}
	rdSz = read(rdFd, msgBuffer, BUFF_SIZE);
	msgBuffer[rdSz] = '\0';	/* Without this line, we will get 
										strange charecters at the end of 
										the file. However, this line will 
										add extra byte with the file, so
										copy-to file will be 1 byte larger 
										than the original file. Need a better 
										solution of this problem.
									*/
	close(rdFd);	

	/* ============= To know copy-to File's Name ============= */
	strcpy(staticMsg, "What is copy-to file's name: ");	
	write(1, staticMsg, strlen(staticMsg) + 1);	
	wrSz = read(0, dstFile, FNAME_SIZE);	
	dstFile[wrSz - 1] = '\0';

	/* ============= Paste data into copy-to file ============= */
	strcpy(staticMsg, "ERROR!!...during open() dstFile...not clear what happened.\n");	
	wrFd = open(dstFile, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(wrFd == -1){
		write(1, staticMsg, strlen(staticMsg) + 1);
		exit(EXIT_FAILURE); 
	}
	write(wrFd, msgBuffer, strlen(msgBuffer) + 1);
	close(wrFd);

	/* ============= Terminate this process ============= */
	exit(EXIT_SUCCESS); // Equivalent to "return 0;".
}
