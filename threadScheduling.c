#include <stdlib.h>	// exit()
#include <pthread.h>	// pthread_create(), pthread_join(), 
							//	pthread_exit()
#include <stdio.h>	// perror()
#include <unistd.h>	// read()
#include <string.h>	// strlen()

#define THREAD_NO 3

typedef struct {
	int tNo;
	char c;
}THREAD_ARGV;
pthread_mutex_t key1, key2;

void *threadFunction(void *argv) {
	static int a;

	THREAD_ARGV tinfo = *(THREAD_ARGV *) argv;
	if(tinfo.c == 'y') {
		pthread_mutex_lock(&key2);
		printf("Thread-%d: ", tinfo.tNo);
		scanf("%d", &a);
		pthread_mutex_unlock(&key2);
	}
		
	pthread_exit(&a);
}

int main() {
	int i, status, retVal[THREAD_NO];
	pthread_t TID[THREAD_NO];
	THREAD_ARGV targv[THREAD_NO];
	char choice[2], msg[50];
	
	status = pthread_mutex_init(&key1, NULL);
	if(status != 0) {
		perror("Error during pthread_mutex_init()");
		exit(EXIT_FAILURE);
	}
	for(i = 0; i < THREAD_NO; i++){
		targv[i].tNo = i;
		pthread_mutex_lock(&key1);
		//sprintf(msg, "\nDo you want any value returned from Thread-%d (y/n): ", i);
		//write(1, msg, strlen(msg));
		//read(0, choice, 2);
		//tArgv[i].c = choice[0];	// '\n' is in choice[1], so we can ignore it.
		printf("\nDo you want any value returned from Thread-%d (y/n): ", i);
		scanf("%c", &targv[i].c);
		getchar();	// for discarding '\n' so that it does not 
		
		pthread_mutex_unlock(&key1);

		status = pthread_create(&TID[i], NULL, &threadFunction, &targv[i]);
		if(status != 0) {
			perror("Error during pthread_create()");
			exit(EXIT_FAILURE);
		}
	}
	/*status = pthread_mutex_destroy(&key1);
	if(status != 0) {
		perror("Error during pthread_mutex_destroy()");
		exit(EXIT_FAILURE);
	}*/

	for(i = 0; i < THREAD_NO; i++) {
		status = pthread_join(TID[i], (void *)&retVal[i]);
		if(status != 0) {
			perror("Error during pthread_create()");
			exit(EXIT_FAILURE);
		}
		printf("Return from Thread-%d: %d\n", i, retVal[i]);
	}		

	exit(EXIT_SUCCESS);
}
