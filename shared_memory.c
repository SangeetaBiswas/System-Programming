/*
	To understand how parent process and child process can communicate via shared memory segment.
	----------------------
	Sangeeta Biswas, Ph.D.
	Dept. of Computer Science and Engineering,
	University of Rajshahi, Rajshahi-6205, Bangladesh
	4.5.2025
*/
#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sched.h>

int main(){
	//Common variables of parent process and child process
	int cpu, node;
	int *shmaddr;
	int i;
	
	//Create a shared memory segment
	//int shmget(key_t key, size_t size, int shmflg);
	int shmid;
	key_t key = 1234;
	shmid = shmget(key, sizeof(int), IPC_CREAT |0666);
	if(shmid == -1){
		perror("shmget: ");
		exit(EXIT_FAILURE);	
	}
	
	//Create a child process
	pid_t pid;
	pid = fork(); 
	if(pid == -1){
		perror("fork: ");
		exit(EXIT_FAILURE);
	}
	
	if(pid == 0){//Child process
		//Attach child process to the shared memory segment
		//void *shmat(int shmid, const void *_Nullable shmaddr, int shmflg);
		shmaddr	= shmat(shmid, NULL, 0);
		if(shmaddr == (int*) -1){
			perror("shmaddr: ");
			exit(EXIT_FAILURE);
		}
		
		//Put data into the shared memory segment
		for(i = 0; i < 100; i++){
			getcpu(&cpu, &node);
			printf("Child Process [shared_var: %d] at CPU: %d, Node: %d\n", *shmaddr, cpu, node);
			*shmaddr = 20;
			getcpu(&cpu, &node);
			printf("Child Process [shared_var: %d] at CPU: %d, Node: %d\n", *shmaddr, cpu, node);
		}
		
		//Detach child process from the shared memory sgment
		//int shmdt(const void *shmaddr);
		if(shmdt(shmaddr) == -1){
			perror("shmdt: ");
			exit(EXIT_FAILURE);		
		}		
	}
	else{//Parent process
		//Attach parent process to the shared memory segment
		//void *shmat(int shmid, const void *_Nullable shmaddr, int shmflg);
		shmaddr	= shmat(shmid, NULL, 0);
		if(shmaddr == (int*) -1){
			perror("shmaddr: ");
			exit(EXIT_FAILURE);
		}
		
		//Put data into the shared memory segment;
		for(i = 0; i < 100; i++){
			getcpu(&cpu, &node);
			printf("Parent Process [shared_var: %d] at CPU: %d, Node: %d\n", *shmaddr, cpu, node);
			*shmaddr = 200;
			getcpu(&cpu, &node);
			printf("Parent Process [shared_var: %d] at CPU: %d, Node: %d\n", *shmaddr, cpu, node);
		}
				
		//Wait for child to be terminated in order to avoid zombie and orphan process
		int status;
		wait(&status);
		
		//Get data from the shared memory segment
		getcpu(&cpu, &node);
		printf("Parent Process [shared_var: %d] at CPU: %d, Node: %d\n", *shmaddr, cpu, node);
		
		//Detach parent process from the shared memory sgment
		//int shmdt(const void *shmaddr);
		if(shmdt(shmaddr) == -1){
			perror("shmdt: ");
			exit(EXIT_FAILURE);		
		}	
		
		//Destroy the shared memory segment
		//int shmctl(int shmid, int op, struct shmid_ds *buf);
		if(shmctl(shmid, IPC_RMID, NULL) == -1){
			perror("shmctl: ");
			exit(EXIT_FAILURE);
		}
	}
	
	//Terminate parent process and child process successfully
	exit(EXIT_SUCCESS);
}
