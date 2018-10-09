/*	======================================================================
	FileName: TCPclient_SmallSizeFileTransfer.c
	Pair of: TCPserver_SmallSizeFileTransfer.c
	----------------------------------------------------------------------
	Purpose:
		To transfer a file of any type(e.g., txt, mp3, jpeg) from server 
		machine to a client machine.
		
		Data will be missing if the file size is large (i.e., above 1 MB).
	----------------------------------------------------------------------
	How to Run:
	A. In one terminal
		$	gcc TCPserver_SmallSizeFileTransfer.c -o 
				TCPserver_SmallSizeFileTransfer
		$	./TCPserver_SmallSizeFileTransfer
	B.	In another terminal
		$	gcc TCPclient_SmallSizeFileTransfer.c -o 
				TCPclient_SmallSizeFileTransfer
		$	./TCPclient_SmallSizeFileTransfer

	** Run TCPserver_SmallSizeFileTransfer at first and then 
		TCPclient_SmallSizeFileTransfer, otherwise there will be 
		the following error message:
		Error during connect(): Connection refused
	----------------------------------------------------------------------
	Notes:
	A.	

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
#include <string.h>		// strlen(), memset()
#include	<fcntl.h>			// open()

#define LOCAL_PORT 49153
#define REMOTE_PORT 49152
#define MSG_SZ 60
#define NAME_SZ 80
#define PATH_SZ 256
#define DATA_SZ 48576	// 1024 Bytes = 1KB
								//	1048576 Bytes = 1MB
								// 2097152 Bytes = 2 MB

int get_sockfd(char bflag);
void connect_sockets(int sockfd);
ssize_t read_file(char data[DATA_SZ]);
void send_data(int sockfd, char data[DATA_SZ], ssize_t datSz);
 
int main() {
	int sockfd;
	char data[DATA_SZ], opinion[3], msg[MSG_SZ];
	ssize_t rdDatSz;

	/*	1. Create a socket. */
	sockfd  = get_sockfd('n');

	/*	2.	Connect client socket with server's 
			active socket via it's name.	
	 */
	connect_sockets(sockfd);

	/*	3.	Upload files into connected server's space.	*/
	memset(opinion, '\0', 3);
	memset(msg, '\0', MSG_SZ);
	sprintf(msg, "Upload a file (y/n)?: ");			
	while(1){
		write(1, msg, strlen(msg));
		read(0, opinion, 2);
		if(strcmp(opinion, "y\n") == 0 || strcmp(opinion, "Y\n") == 0){
			rdDatSz = read_file(data);
			send_data(sockfd, data, rdDatSz);
		}
		else if(strcmp(opinion, "n\n") == 0 || strcmp(opinion, "N\n") == 0)
			break;
		else
			write(1, "Press y/n\n", 10);
	}

	/*	4.	Close connected socket. */
	close(sockfd);

	exit(EXIT_SUCCESS);
}

ssize_t read_file(char data[DATA_SZ]) {
	char fileName[NAME_SZ], filePath[PATH_SZ];
	char fileFullName[NAME_SZ + PATH_SZ], msg[MSG_SZ];
	ssize_t rdDatSz;
	int fd;

	while(1) {
		/*	Read the path of the desired directory. */
		write(1, "Path: ", 6);
		memset(filePath, '\0', PATH_SZ);	
		rdDatSz = read(0, filePath, PATH_SZ);
		if(filePath[rdDatSz-2] != '/'){
			filePath[rdDatSz-1] = '/';
			filePath[rdDatSz] = '\0';
		}
		else
			filePath[rdDatSz-1] = '\0';

		/*	Show the list of files in the directory. */
		memset(msg, '\0', MSG_SZ);	
		sprintf(msg, "ls -l %s", filePath);
		system(msg);
		
		/* Read the name of the desired file. */
		write(1, "File Name: ", 11);
		memset(fileName, '\0', NAME_SZ);	
		rdDatSz = read(0, fileName, NAME_SZ);
		fileName[rdDatSz - 1] = '\0'; 

		/*	open the file. */
		memset(fileFullName, '\0', NAME_SZ + PATH_SZ);	
		sprintf(fileFullName, "%s%s", filePath, fileName);	
		fd = open(fileFullName, O_RDONLY);
		if(fd <= 0){
			perror("Error during open()");
			continue;
		}
		break; // When a file is successfully opened.
	}
		
	/*	Read the content of the file. */	
	rdDatSz = read(fd, data, DATA_SZ);
	
	/*	For Debugging.*/
	memset(msg, '\0', MSG_SZ);
	sprintf(msg, "Data in %s: %lu Bytes\n", \
						fileFullName, rdDatSz);
	write(1, msg, strlen(msg));

	return rdDatSz;
}

void send_data(int sockfd, char data[DATA_SZ], ssize_t datSz) {
	ssize_t sentDatSz;
	char msg[MSG_SZ];
	
	/* Send data. */
	sentDatSz = send(sockfd, data, datSz, 0);
	if(sentDatSz == -1){
		perror("Error during send()");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	
	/*	For Debugging.*/
	memset(msg, '\0', MSG_SZ);
	sprintf(msg, "Sent Data: %lu Bytes", sentDatSz);
	write(1, msg, strlen(msg));
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
		
		Operating System (OS) automatically selects and 
		binds a name to an unbound socket of a client 
		during establishing connection.
	*/
	if(bflag == 'y'){
		addr.sin_family = AF_INET;
		addr.sin_port = htons(LOCAL_PORT);
		addr.sin_addr.s_addr = inet_addr("10.110.54.60");
		//addr.sin_addr.s_addr = htonl(INADDR_ANY);	// Any IP of the host.
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
