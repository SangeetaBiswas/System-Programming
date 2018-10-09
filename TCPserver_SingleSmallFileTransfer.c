/*	======================================================================
	FileName: TCPserver_SingleSmallFileTransfer.c
	Pair of: TCPclient_SingleSmallFileTransfer.c
	----------------------------------------------------------------------
	Purpose:
		To accept a file of any type(e.g., txt, mp3, jpeg) from a client
		machine and store in the favourite place. 
		
		Data will be missing if the file size is large (i.e., above 1 MB).
	----------------------------------------------------------------------
	How to Run:
	A. In one terminal
		$	gcc TCPserver_SingleSmallFileTransfer.c -o 
				TCPserver_SingleSmallFileTransfer
		$	./TCPserver_SingleSmallFileTransfer
	B.	In another terminal
		$	gcc TCPclient_SingleSmallFileTransfer.c -o 
				TCPclient_SingleSmallFileTransfer
		$	./TCPclient_SingleSmallFileTransfer

	** Run TCPserver_SingleSmallFileTransfer at first and then 
		TCPclient_SingleSmallFileTransfer, otherwise there will be 
		the following error message:
		Error during connect(): Connection refused
	----------------------------------------------------------------------
	Tasks:
	A.	Server Side:
		1. Connect with the socket of a single client at a time.
		2. Receive data from the client.
		3. Take file name from the user of the server.
		4. Store data in a file.
		5. Close active and passive sockets.

	B. Client Side:
		1. Connect with the socket of the server having a name
			(i.e. IP address:Port Address)
		2.	Take path of directory from the user of the client.
		3. Show the list of the files in the directory.
		4. Take a file name from the user of the client.
		5. Check wheter that file is a valid file in the specified
			directory.
		6. Open the file after concating a valid path with a valid
			file name.
		7. Read the file and send its content to the server.
		8. Close connection with the server by closing socket.
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

#include <stdlib.h>		// exit(), system()
#include <sys/socket.h>	//	socket(), bind()
#include <stdio.h>		// perror()
#include <netinet/in.h>	// htons(), htonl()
#include <unistd.h>		// close(), read(), write()
#include <string.h>		// strlen(), strcpy(), strcat(), memset()
#include <fcntl.h>		// open()

#define LOCAL_PORT 49152
#define DATA_SZ 1048576	// 1024 Bytes = 1KB
								//	1048576 Bytes = 1MB
								// 2097152 Bytes = 2 MB
#define NAME_SZ 336
#define MSG_SZ 60

int get_sockfd(char bflag);
void turn_passive_socket(int sockfd, int backlog);
int get_active_sockfd(int sockfd);
ssize_t receive_data(int sockfd, char data[DATA_SZ]);
void store_file(char data[DATA_SZ], ssize_t datSz);
 
int main(int argc, char *argv[]) {
	int sockfd, active_sockfd;	
	char data[DATA_SZ];
	ssize_t recvDatSz;

	/*	1. Create a socket and bind a unique name to it. */
	sockfd  = get_sockfd('y');

	/*	2.	Turn a normal socket into a passive socket. */ 
	turn_passive_socket(sockfd, 1);

	/*	3.	Create an active socket. */
	active_sockfd = get_active_sockfd(sockfd);

	/*	4.	Store files uploaded by a client.*/
	recvDatSz = receive_data(active_sockfd, data);
	store_file(data, recvDatSz);

	/*	5.	Close sockets. */	
	close(active_sockfd);
	close(sockfd);

	exit(EXIT_SUCCESS);
}

ssize_t receive_data(int sockfd, char data[DATA_SZ]) {
	ssize_t recvDatSz;
	char msg[MSG_SZ];

	/*	Receive Data from the remote machine. */
	memset(data, '\0', DATA_SZ);	//	Clear buffer before using.
	recvDatSz = recv(sockfd, data, DATA_SZ, 0);
	if(recvDatSz == -1){
		perror("Error during recv()");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/*	For Debugging.*/
	memset(msg, '\0', MSG_SZ);	//	Clear buffer before using.	
	sprintf(msg, "Received Data: %lu Bytes\n", recvDatSz);
	write(1, msg, strlen(msg));

	return recvDatSz;
}

void store_file(char data[DATA_SZ], ssize_t datSz) {
	char fileName[NAME_SZ], msg[MSG_SZ];
	ssize_t rdStrSz, wrStrSz;
	int fd;

	while(1){	// Do until a file is not successfully opened for writing.
		/*	Decide the file name. */
		write(1, "Save As: ", 9);
		memset(fileName, '\0', NAME_SZ);	
		rdStrSz = read(0, fileName, NAME_SZ); // Enter fileName with absolute path
		if(rdStrSz == 1){	// If user just press ENTER without a file name.
			write(1, "Enter a valid file name.", 24);
			continue;
		}
		fileName[rdStrSz - 1] = '\0';

		/*	Open a file in the current directory. */	
		fd = open(fileName, O_CREAT | O_WRONLY, 00444);
		if(fd <= 0){
			perror("Error during open()");
			continue;
		}
	}
	
	/*	Store the received content into the opened file. */	
	wrStrSz = write(fd, data, datSz);
	close(fd);

	/*	For Debugging.*/
	memset(msg, '\0', MSG_SZ);	//	Clear buffer before using.	
	sprintf(msg, "Stored %lu Bytes in %s\n", wrStrSz, fileName);
	write(1, msg, strlen(msg));
}

int get_active_sockfd(int sockfd) {
	/*	accept() extracts the first connection request on the 
		queue of pending connections for the listening socket,
		sockfd, creates a new connected socket, and returns a 
		new file descriptor  referring  to that socket. 

		The newly created socket is not in the listening state. 
		The original socket  sockfd  is  unaffected  by  this 
		call.
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

		However, if syncookies is set 1(i.e., true), then there 
		is no logical maximum length and this setting is ignored. 
		Check the value of syncookies by typing:
		$  cat /proc/sys/net/ipv4/tcp_syncookies
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
		
		Operating System (OS) automatically selects and 
		binds a name to an unbound socket of a client 
		during establishing connection.
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
