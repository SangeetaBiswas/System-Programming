/*	======================================================================
	FileName: TCPserver_ChatWSingleClient.c
	Pair of: TCPclient_ChatWSingleClient.c
	Extension of: TCPserver_HelloMsg.c
	----------------------------------------------------------------------
	Purpose:
		To chat with a remote machine.
	----------------------------------------------------------------------
	How to Run:
	A. In one terminal
		$	gcc TCPserver_ChatWSingleClient.c -o TCPserver_ChatWSingleClient
			-lpthread
		$	./TCPserver_ChatWSingleClient
	B.	In another terminal
		$	gcc TCPclient_ChatWSingleClient.c -o TCPclient_ChatWSingleClient
		$	./TCPclient_ChatWSingleClient
	----------------------------------------------------------------------
	Notes:
	A.	Run server Process at first and then the client process, otherwise 
		the following error message will be displayed and client process 
		will be terminated.
	----------------------------------------------------------------------
	Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	27/04/2018
	======================================================================
*/

#include <stdlib.h>		// exit()
#include <sys/socket.h>	//	socket(), bind()
#include <stdio.h>		// perror()
#include <netinet/in.h>	// htons(), htonl()
#include <unistd.h>		// close()
#include <string.h>		// strlen(), strcpy(), strcat(), memset()
#include <pthread.h>		// pthread_create(), pthread_join()
#include <signal.h>		// pthread_kill()

#define LOCAL_PORT 49152
#define MSG_SZ 256

typedef struct{
	int sockfd;
	pthread_t TID1, TID2;
}thread_argv;

int get_sockfd(char bflag);
void turn_passive_socket(int sockfd, int backlog);
int get_active_sockfd(int sockfd);
void *receive_msg(void *arg);
void *send_msg(void *arg);
void chitChat(int sockfd);

 
int main() {
	int sockfd, active_sockfd;

	/*	1. Create a socket and bind a unique name to it. */
	sockfd  = get_sockfd('y');

	/*	2.	Turn a normal socket into a passive socket. */
	turn_passive_socket(sockfd, 1);

	/*	3.	Create an active socket. */
	active_sockfd = get_active_sockfd(sockfd);

	/*	4.	Chitchat with a client.	*/
	chitChat(active_sockfd);

	/*	5.	Close sockets.	*/
	close(active_sockfd);
	close(sockfd);

	exit(EXIT_SUCCESS);
}

void chitChat(int sockfd){
	pthread_t TID1, TID2;
	int status;
	thread_argv targ;
	
	/* Create two threads so that server can send to and
		receive message from client simultaneously.
	*/
	targ.sockfd = sockfd;
	status = pthread_create(&TID1, NULL, send_msg, &targ);
	if(status != 0){
		perror("Error during pthread_create() for sending message");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	status = pthread_create(&TID2, NULL, receive_msg, &targ);
	if(status != 0){
		perror("Error during pthread_create() for receiving message");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/*	Wait until threads are terminated. */
	pthread_join(TID1, NULL);
	pthread_join(TID2, NULL);
}

void *send_msg(void *arg) {
	thread_argv *targPtr;
	char msg[MSG_SZ];
	ssize_t rdMsgSz, sentStrSz;

	targPtr = (thread_argv *)arg; 
	targPtr->TID1 = pthread_self();
	do{
		write(1, "Server: ", 8);
		/*	1.	Read message from STDIN */	
		memset(msg, '\0', MSG_SZ);	//	Clear buffer before using.
		rdMsgSz = read(0, msg, MSG_SZ);

		/*	2.	Send message to the remote host. */	
		sentStrSz = send(targPtr->sockfd, msg, rdMsgSz, 0);
		if(sentStrSz == -1){
			perror("Error during send()");
			close(targPtr->sockfd);
			exit(EXIT_FAILURE);
		}
		if(strcmp(msg, "Bye\n") == 0){
			pthread_kill(targPtr->TID2, SIGTERM);
			break;
		}
	}while(1);
}

void *receive_msg(void *arg) {
	thread_argv *targPtr;
	char msg[MSG_SZ], dispMsg[MSG_SZ + 9];
	ssize_t recvStrSz;

	/*	Receive and display the message sent by 
		the remote host. 
	*/	
	targPtr = (thread_argv *) arg;
	targPtr->TID2 = pthread_self();
	do{
		memset(msg, '\0', MSG_SZ);
		recvStrSz = recv(targPtr->sockfd, msg, MSG_SZ, 0);
		if(recvStrSz == -1){
			perror("Error during recv()");
			close(targPtr->sockfd);
			exit(EXIT_FAILURE);
		}

		/*	2. Display the message. */
		memset(dispMsg, '\0', MSG_SZ + 9);
		strcpy(dispMsg, "\nClient: ");
		strcat(dispMsg, msg);
		write(1, dispMsg, strlen(dispMsg));

		/*	3. Terminate oher thread. */
		if(strcmp(msg, "Bye\n") == 0){
			pthread_kill(targPtr->TID1, SIGTERM);
			break;
		}
		write(1, "Server: ", 8);
	}while(1);
}

int get_active_sockfd(int sockfd) {
	/*	accept() extracts the first connection request on the 
		queue of pending connections for the listening socket,
		sockfd, creates a new connected socket, and returns a 
		new file descriptor  referring  to that socket. The 
		newly created socket is not in the listening state. The 
		original socket  sockfd  is  unaffected  by  this call.
	*/
	int active_sockfd = accept(sockfd, NULL, NULL);
	if(active_sockfd == -1){
		perror("Error during accept()");
		close(sockfd);
		exit(EXIT_FAILURE);
	}	

	return active_sockfd;
}

void turn_passive_socket(int sockfd, int backlog) {
	/*	listen() turns the socket referred to by 'sockfd' as a 
		passive socket, that is, as a socket that will be used 
		to accept incoming connection requests using accept(2).

		'backlog' argument defines the maximum length of queue
		holding sockfd. By this a server can be active for 
		multiple clients at a time.
	*/
	int status = listen(sockfd, backlog); 
	if(status == -1){
		perror("Error during listen()");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
}

int get_sockfd(char bflag) {
	int sockfd, addrLen, status;
	struct sockaddr_in addr;
	struct sockaddr *sockAddrPtr;
	
	/*	Create a socket without a name (i.e., an address.)*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("Error during socket()");
		exit(EXIT_FAILURE);
	}

	/*	Explicitly bind a unique name (i.e., address) 
		to a socket. For a server it is mandatory to
		bind a TCP socket. But for a client it is optional.
	*/
	if(bflag == 'y'){
		addr.sin_family = AF_INET;
		addr.sin_port = htons(LOCAL_PORT);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);	// Any IP of the host.
		/* In order to use a specific IP, use:
				inet_aton("10.10.10.1", &addr.sin_addr.s_addr);

			*	It is not mandatory to use htonl() for INADDR_ANY.		
		*/
    	sockAddrPtr = (struct sockaddr *) &addr;
    	addrLen = sizeof(struct sockaddr_in);
		status = bind(sockfd, sockAddrPtr, addrLen);
		if(status == -1){
			perror("Error during bind()");
			close(sockfd);
			exit(EXIT_FAILURE);
		}
	}
	
	return sockfd;
}
