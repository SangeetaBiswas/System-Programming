/*	======================================================================
	FileName: TCPclient_HelloMsg.c
	Pair of: TCPserver_HelloMsg.c
	----------------------------------------------------------------------
	Purpose:
		To understand how TCP server-client model works.
	----------------------------------------------------------------------
	How to Run:
	A. In one terminal
		$	gcc TCPserver_HelloMsg.c -o TCPserver_HelloMsg
		$	./TCPserver_HelloMsg
	B.	In another terminal
		$	gcc TCPclient_HelloMsg.c -o TCPclient_HelloMsg
		$	./TCPclient_HelloMsg

	** Run TCPserver_HelloMsg at first and then TCPclient_HelloMsg,
		otherwise there will be the following error message:
		Error during connect(): Connection refused
	
	Output:		
		In two terminals, messages sent by each other will be displayed.
	----------------------------------------------------------------------
	Necessary System Calls:
	A. connect()
		*	Task:	Invite incoming connections.		
		*	Synopsis: int connect(int sockfd, const struct sockaddr *addr,
                   				socklen_t addrlen);
		*	Return: 0 on suucess, -1 on error
		*	Header File: #include <sys/socket.h>

	B. Other system calls are: socket(), send(), recv(), close()
	----------------------------------------------------------------------
	Notes:
	A. Sockets are commonly used in client-to-server model.

		In a client-to-server model, server process provides services
		whereas client process requests for services.

		The socket on the server process waits for requests from a client.
		The socket on the client process connects it to the server process. 

			Server									Client	
		===========								===========
			socket()									socket()												
				|											|
				|											|
			bind()[mandatory]						bind()[optional]
				|											|
				|											|
			listen()										|
				|											|
				|<----------------------------connect()
			accept()										|
				|											|
				|											|
			send()/recv()							send()/recv()
				|											|
				|											|
			close()									close()
	----------------------------------------------------------------------
	Sangeeta Biswas, Ph.D.
	Assistant Professor,
	Dept. of CSE, University of Rajshahi,
	Rajshahi-6205, Bangladesh.
	sangeeta.cse.ru@gmail.com / sangeeta.cse@ru.ac.bd
	----------------------------------------------------------------------
	26/04/2018
	======================================================================
*/

#include <stdlib.h>		// exit()
#include <sys/socket.h>	//	socket(), bind()
#include <stdio.h>		// perror()
#include <netinet/in.h>	// htons(), htonl()
#include <unistd.h>		// close()
#include <string.h>		// strlen(), strcat(), strcpy(), memset()
#include <pthread.h>		// pthread_create(), pthread_join()
#include <signal.h>		// pthread_kill()

#define LOCAL_PORT 49153
#define REMOTE_PORT 49152
#define MSG_SZ 256

typedef struct{
	int sockfd;
	pthread_t TID1, TID2;
}thread_argv;

int get_sockfd(char bflag);
void connect_sockets(int sockfd);
void *receive_msg(void *arg);
void *send_msg(void *arg);
void chitChat(int sockfd);
 
int main() {
	int sockfd;

	/*	1. Create a socket at first. Then bind a 
			unique name to it (optional).
	*/
	sockfd  = get_sockfd('n');

	/*	2.	Connect client sockets with server's
			active socket via it's name.	
	 */
	connect_sockets(sockfd);

	/*	3.	Exchange message with a client.	*/
	chitChat(sockfd);

	/*	4.	Close sockets by releasing any system
			resources acquired by sockets.	
	*/
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
	targPtr->TID1 = pthread_self(); /*	Get it's own thread ID which
													can be used to kill by another
													thread when it is necessary.
												*/	
	do{
		/*	1.	Read message from STDIN */
		write(1, "Client: ", 8);	
		memset(msg, '\0', MSG_SZ);	
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

	/*	1.	Receive and display the message sent by 
		the remote host. 
	*/	
	targPtr = (thread_argv*) arg;
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
		strcpy(dispMsg, "\nServer: ");
		strcat(dispMsg, msg);
		write(1, dispMsg, strlen(dispMsg));
		if(strcmp(msg, "Bye\n") == 0){
			pthread_kill(targPtr->TID1, SIGTERM);
			break;
		}
		write(1, "Client: ", 8);
	}while(1);
}

void connect_sockets(int sockfd) {
	int addrLen, status;
	struct sockaddr_in addr;
	struct sockaddr *sockAddrPtr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(REMOTE_PORT);	// Server's Port.
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	// Any IP of the host.
	sockAddrPtr = (struct sockaddr *) &addr;
   addrLen = sizeof(struct sockaddr_in);

	/*	connect() attempts to make a connection to the socket 
		that is bound to the address specified by addr.
	*/
	status = connect(sockfd, sockAddrPtr, addrLen);
	if(status == -1){
		perror("Error during connect()");
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
