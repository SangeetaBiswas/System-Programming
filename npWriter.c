/* ======================================================================
	FileName: npWriter.c
   Purpose:  To understand how to use a named pipe to establish a
				 communication between two processes.

	Problem: Suppose one Writer tells her story to a reader 
				little-by-little based on reader's opinion. Writer tells
				her story as long as reader wants to listen. A named
				pipe is used for communication. That means both Writer and
				reader use the same named pipe for sending their message
				to others.
	
	Tasks of Writer:
		1. Check whether any name is provided from the terminal for
			a named pipe.
		2. Write the content of an array row by row into the pipe.
		3. Read Reader's opinion from the pipe and check whether
			it is equal to 'n'. 
		4. If it is 'n', stop writing into the pipe.
		5. If it is 'y', write i-th row of the array containing
			the story into the pipe.
		6. Increase 'i; after each write operation as long as it does
			not exceed the number of rows.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, 
		A.1.	at first create a named pipe.
				$ mkfifo <pipeName> 
				[ i.e., mkfifo RoyalStory ]

		A.2.	then compile and run 'npWriter.c'.
				$ gcc npWriter.c -o npWriter
				$ ./npWriter <pipeName>
				[ i.e. $./npWriter RoyalFamily]

	B. At another terminal, compile and run 'npeader.c'.
		$ gcc npReader.c -o npReader
		$ ./npReader <pipeName>
		[ i.e. $./npReader RoyalFamily]

		*** It would be problematic, if multiple 'npReader' processes run.
	----------------------------------------------------------------------
	Notes:
	A.	
		A.1.	It is a special kind of file which exists in the file
				system (i.e., in the tree structure) like a regular file.
		A.2.	It is also known as FIFO (First-In-First-Out).
	B.
		B.1.	Unlike the regular file, the content of a named pipe is 
				not stored in the hard disk, it exists in the memory. 
		B.2.	Its size is always 0 KB.
	C.	
		C.1.	Unlike regular (i.e., unnamed) pipes, named pipes can be 
				used to establish communication between unrelated processes.
		C.2.  A named pipe needs to delete explicitly, since it remains
				in the file system.
	D. 
		D.1.	A named pipe can be accessed by the same functions
				used for accessing a regular file.
		D.2.	WR_ONLY option will block a process until any other 
				process reads data from the pipe.
		D.3.	RD_ONLY option will block a process until any other 
				process reads data from the pipe.
		D.4. 	The content of a named pipe is flushed out after read 
				operation. So, it needs to be write again, if we need to 
				read it further.
	F.
		F.1.	To create a named pipe in the terminal
				$ mknod <pipeName> p
				[ i.e., $ mknod RoyalFamily p ]				

				$ mkfifo [-m MODE] <pipeName>
				[ i.e., $ mkfifo -m 644 RoyalFamily ]
				[ i.e., $ mkfifo -m a=rw RoyalFamily ]	
	
		F.2.	To delete a named pipe in the terminal

				$ rm <pipeName>
				[ i.e., $ rm RoyalFamily ]
						OR
				$ unlink <pipeName>
				[ i.e., $ unlink RoyalFamily ]

		F.3.	To create a named pipe inside a process

      		int mkfifo(const char *pathname, mode_t mode);
				[ i.e., mkfifo("/home/sangeeta/OS/Code/RoyalFamily", 644) ]

		F.4.	To delete a named pipe inside a process

       		#include <unistd.h>
				int unlink(const char *pathname);
				[ i.e., unlink("/home/sangeeta/OS/Code/RoyalFamily"); ]
	----------------------------------------------------------------------
   Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	19/11/2017
	======================================================================
*/
#include <stdlib.h>	// For exit()
#include <fcntl.h>	// For open()
#include <unistd.h>	// For close()
#include <stdio.h>	// For perror()
#include <string.h>	// For strlen()

#define OPINION_SZ 3

int main(int argc, char *argv[]){
	int fd, i;
	char *story[] = { 
		"Helloooo, I am a writer. I have an interesting story to tell you. \nDo you want to read it? y/n: ",
		"Once upon a time, there was a happy royal family. The king was a banyan tree, the queen was a lake, the prince was a frog and the princess was a frog. \nDo you want to read more? y/n: ",		
		"Queen's mother was a water lily. Prince liked to chit-chat with his grandma sitting on her arm i.e., leaf. Princess liked to eat the honey which her grandma stored for her. \nDo you want to read more? y/n: ",	
		"Sorry, I have not written the next part yet. Press 'n' to quit."	 
	};
	int storyPartN = 3;	// This value depends on the number of row
								// of the above 'story' array. 
	char opinion[OPINION_SZ] = "y";
	char *msg = "Usage: ./npWriter <pipeName>\n";
	
	/* =====================================================
		Check whether the name of the pipe is provided. 
		===================================================== 
	*/
	if(argc != 2){
		write(1, msg, strlen(msg) + 1);
		exit(EXIT_FAILURE);
	}
	
	while(strcmp(opinion,"y") == 0){
		/* =====================================================
			 Write a story for a reader into the named pipe. 
			===================================================== 
		*/
		fd = open(argv[1], O_WRONLY);
		if(fd == -1){
			perror("Error during open()");
			exit(EXIT_FAILURE);
		}
		write(fd, story[i], strlen(story[i]) + 1);
		close(fd);
		if(i < storyPartN)
			i++;
		else
			i = storyPartN;

		/* =====================================================
			 Read reader's opinion from the named pipe. 
			===================================================== 
		*/
		fd = open(argv[1], O_RDONLY);
		if(fd == -1){
			perror("Error during open()");
			exit(EXIT_FAILURE);
		}	
   	read(fd, opinion, OPINION_SZ);
		close(fd);
   }

	exit(EXIT_SUCCESS);
}
