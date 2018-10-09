/* ======================================================================
	FileName: sameNameChildren.c

   Purpose: To create multiple child processes of a process,e.g.,  P, 
				having the same name of their parent, i.e., P.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc differentNameChildren.c -o differentNameChildren
		$ ./differentNameChildren <childNo>
		
	B.	Example:
		B.1 $ ./differentNameChildren 44 4

		B.2. Open another terminal (Ctrl+Alt+T)
			$ pstree 
			-- gterminal -- bash -- differentNameCh -- addition
															  	|-- subtraction
															  	|-- multiplication
															  	|-- division
		B.3. Kill all processes by Ctrl+C 
	----------------------------------------------------------------------
   Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	12/10/2017
	======================================================================
*/
#include <stdio.h>	// For perror().
#include <unistd.h>	// For fork().
#include <stdlib.h>	//	For atoi(), exit().
#include <string.h>	// For strlen().

#define CHILD_N 4
#define BUFFER_SZ 100

int main(int argc, char *argv[]){
	pid_t CID;	// Child process's ID
	int i, exeSt;
	char *msg = "Usage: ./differentNameChildren [<a> <b>]\n";

	/* Executable(i.e., exe) files of child processes. */
	char exeFile[CHILD_N][BUFFER_SZ] = {
		"/home/sangeeta/OS/Code/InstantNoodlesFestival-2017/addition", 
		"/home/sangeeta/OS/Code/InstantNoodlesFestival-2017/subtraction",
		"/home/sangeeta/OS/Code/InstantNoodlesFestival-2017/multiplication",
		"/home/sangeeta/OS/Code/InstantNoodlesFestival-2017/division"	
	};

	for(i = 0; i < CHILD_N; i++){
		CID = fork();
		if(CID == -1){
			perror("Error during fork()");
			exit(EXIT_FAILURE);	// Unsuccessful termination
		}

		if(CID == 0){
			switch(argc){
				case 1:
					exeSt = execlp(exeFile[i], exeFile[i], NULL);
				break;
				case 2:
					exeSt = execlp(exeFile[i], exeFile[i], argv[1], NULL);
				break;
				case 3:
					exeSt = execlp(exeFile[i], exeFile[i], argv[1], argv[2], NULL);
				break;
				default:
					write(1, msg, strlen(msg) + 1);
					exit(EXIT_FAILURE); // Unsuccessful termination
				break;
			}

			if(exeSt == -1){
				perror("Error during execlp()");
				exit(EXIT_FAILURE);	// Unsuccessful termination
			}
		}
	}
	
	/* 
		To keep parent process alive as long as child
		processes are alive. If parent process dies before
		child processes, they become orphan process. Process
		'systemd' / 'upstart' becomes parent of orphan processes. 
	*/
	while(1);

	exit(EXIT_SUCCESS);	//Successful Termination
}
