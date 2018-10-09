/* ======================================================================
	FileName: readWriteExample.c
	Purpose:  To understand how read(), write() system calls work.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc readWriteExample.c -o readWriteExample
		$ ./readWriteExample
	----------------------------------------------------------------------
	Necessary System Calls:
	A. read()
		A.1 Task:
			To read from a file descriptor (fd).
			**	File descriptor (fd) is a non-negative integer which acts
				as a handle for accessing a file. Each Linux process gets 
				three fd by default:
	
				Standard Input		0		STDIN_FILENO	stdin
				Standard Output		1		STDOUT_FILENO	stdout
				Standard Error		2		STDERR_FILENO	stderr

		A.2 Synopsis:
			ssize_t read(int fd, void *buf, size_t count);

		A.3 Return Value:
			The number of bytes read on success and -1 on error.

		A.4 Header File:
			#include <unistd.h>	
	
	B. write()
		B.1 Task:
			To write to a file descriptor.

		B.2 Synopsis:
			ssize_t write(int fd, const void *buf, size_t count);

		B.3 Return Value:
			The number of bytes written on success and -1 on error.

		B.4 Header File:
			#include <unistd.h>	
	----------------------------------------------------------------------
	Necessary Library Functions:
	A. strcpy()
		A.1 Task:
			To copy a string.

		A.2 Synopsis:
      		char *strcpy(char *dest, const char *src);
			
			**	strcpy() includes the terminating null 
				byte ('\0') to the buffer pointed to by
				dest.

		A.3 Return Value:
			A pointer to the destination string, dest.

		A.4 Header File:
			#include <string.h>

	B. strlen()
		B.1 Task:
			To calculate the length of a string.

		B.2 Synopsis:
      	size_t strlen(const char *s);
			
			** strlen() excludes the terminating 
				null byte ('\0').

		B.3 Return Value:
			The number of bytes in the string s.

		B.4 Header File:
			#include <string.h>

	C. exit()
		C.1 Task:
			To cause normal process termination.

		C.2 Synopsis:
			void exit(int status);

			** Two constants used as arguments:
			EXIT_SUCCESS 	successful termination
			EXIT_FAILURE 	successful termination

		C.3 Return Value:
			Nothing will be returned.

		C.4 Header File:
			#include <stdlib.h>
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

#include <unistd.h> 	// for read(), write().
#include <string.h> 	// for strcpy(), strlen().	
#include <stdlib.h>		// for exit().

#define BUFF_SIZE 1024
#define MSG_SIZE 22

int main(){
	char staticMsg[MSG_SIZE], msgBuffer[BUFF_SIZE]; 
	ssize_t rdSz, wrSz;

	strcpy(staticMsg, "What is your message: ");	
	write(1, staticMsg, strlen(staticMsg));	// Write to stdout.

	/* 	Since we want to read whole stream, but we do not know how 
		much data will be entered by stdin, we use BUFF_SIZE to be
		at safe size. We will get size of actual read data in rdSz.
	*/
	rdSz = read(0, msgBuffer, BUFF_SIZE);	// Read from stdin.

	/* 
		read() does not include any null byte ('\0') at the end of   
		the input stream, whereas a string needs '\0' byte at 
		the end to be a string. Therefore, we need to add '\0' at  
		msgBuffer. If we do not do it, we will get strange characters,
		when we will print message of msgBuffer.
	 */
	msgBuffer[rdSz] = '\0';

	strcpy(staticMsg, "Your written message: ");
	write(1, staticMsg, strlen(staticMsg)); // Write to stdout.
	write(1, msgBuffer, strlen(msgBuffer)); // Write to stdout.

	exit(EXIT_SUCCESS); // Equivalent of "return 0;".
}
