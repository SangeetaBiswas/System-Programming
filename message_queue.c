#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <string.h>

int main(){
	//Common variables of parent process and child process
	struct msg_buffer {
		long type;
		char text[100];
	} msg;
	
	//Convert a pathname and a project identifier to a IPC key
	key_t key = ftok("/home/kinu/CSE_Courses/OS_3241/Code/shared_memory", 127);
	printf("key: %d\n", key);
	
	//Create a message queue
	//int msgget(key_t key, int msgflg);
	int msqid;	
	msqid = msgget(key, 0666|IPC_CREAT);
	if(msqid == -1){
		perror("msgget: ");
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
		printf("Child process\n");
		
		//Send message
		//int msgsnd(int msqid, const void msgp[.msgsz], size_t msgsz, int msgflg);
		msg.type = 1;
		strcpy(msg.text, "Hi, I am Child\n");
		msgsnd(msqid, &msg, sizeof(msg), 0);
	}
	else{//Parent process
		printf("Parent Process\n");
				
		//Wait for child to be terminated in order to avoid zombie and orphan process
		wait(NULL);
		
		//Receive message from the message queue
		//ssize_t msgrcv(int msqid, void msgp[.msgsz], size_t msgsz, long msgtyp, int msgflg);
		msgrcv(msqid, &msg, sizeof(msg), 1, 0);	
		printf("Message from Child: %s", msg.text);
		
		//Destroy the message queue
		//int msgctl(int msqid, int op, struct msqid_ds *buf);
		if(msgctl(msqid, IPC_RMID, NULL) == -1){
			perror("msgctl: ");
			exit(EXIT_FAILURE);
		}
	}
	
	//Terminate parent process and child process successfully
	exit(EXIT_SUCCESS);
}
