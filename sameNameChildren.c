/* ======================================================================
	FileName: sameNameChildren.c

   Purpose: To create multiple child processes of a process,e.g.,  P, 
				having the same name of their parent, i.e., P.
	----------------------------------------------------------------------
	How to Run: 
	A. At a terminal, type
		$ gcc sameNameChildren.c -o sameNameChildren
		$ ./sameNameChildren <childNo>
		
	B.	Example:
		B.1 $ ./sameNameChildren 4

		B.2. Open another terminal (Ctrl+Alt+T)
			$ pstree 
			-- gterminal -- bash -- sameNameChildre -- sameNameChildre
															  	|-- sameNameChildre
															  	|-- sameNameChildre
															  	|-- sameNameChildre
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

#define DEFAULT_N 2

int main(int argc, char *argv[]){
	int child_N, i;
	char *msg = "Usage: ./sameNameChildren <childNo>\n";
	pid_t CID;	// Child process's ID

	if(argc == 1)
		child_N = DEFAULT_N;
	else if(argc == 2)
		child_N = atoi(argv[1]);
	else{
		write(1, msg, strlen(msg) + 1);
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < child_N; i++){
		CID = fork();
		if(CID == -1){
			perror("Error during fork()");
			exit(EXIT_FAILURE);	// Unsuccessful termination
		}
		
		if(CID == 0)
			while(1);	/* Only for tracing child process 
								by 'pstree'/ 'top'. Since child process
								will be in infinte loop, parent process
								can generate another child process.
							*/
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
