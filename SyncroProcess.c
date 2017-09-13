/* to compile:  gcc -o SyncroProcess SyncroProcess.c
 *  This program will exhibit race conditions
*/ 

#include <stdio.h>
#include <stdlib.h>

//added includes
#include <signal.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>
//#include <sys/wait.h>

#define NUMPROCESSES 10
#define COUNT 10

//catcher method added
void catcher( int sig ) {
    printf( "Signal catcher called with signal %d\n", sig );
    }
    
int second(int procnum, int mypid) {
	// "boilerplate" variables added for signalling:
	struct sigaction sigact;
    sigset_t waitset;
    int sig;
    int result = 0;

    sigemptyset( &sigact.sa_mask );
    sigact.sa_flags = 0;
    sigact.sa_handler = catcher;
    sigaction( SIGUSR1, &sigact, NULL );

    sigemptyset( &waitset );
    sigaddset( &waitset, SIGUSR1 );

    sigprocmask( SIG_BLOCK, &waitset, NULL );

	int counter=rand() %200;//perfect random timer for excellent deadlock action
	int i;//a generic counter
	if(procnum != 0){
		result = sigwait( &waitset, &sig );//wait for a signal
    	if( result == 0 )
        	fprintf(stdout, "sigwait() returned for signal %d\n", sig );
    	else {
        	fprintf(stdout, "sigwait() returned error number %d\n", errno );
        	perror("sigwait() function failed\n" );
    	}
	}
    
    //at this point, either this is the final process or a locked process has been unlocked.
    for(i=0;i<10;i++){
		fprintf(stderr,"(process #%d):%d\n",procnum,i);	
		usleep(counter);
	}
	//killing self starts the signal process
	kill(mypid,SIGUSR1);
	return result;
}


main() {
	int i;
	int parentpid=getpid();
	int childpid[NUMPROCESSES];
	
	for (i=0;i<NUMPROCESSES;i++){
		childpid[i]=0;
	}
	fprintf(stderr,"Main Program executing:\n");
	for(i=0;i<NUMPROCESSES;i++){
		if((childpid[i] = fork())==-1){
			fprintf(stderr,"Process creation(i=%d: pid#%d)has failed.", i, childpid[i]);	
			return 1;	
		}
		else if(childpid[i]){
			second(i,childpid[i]);
			break;
			return 0;
		}
		else{
		// do nothing
		}	
	}
}//end main,


