#   Windows thread synchronization example
#
#

#include <windows.h>
#include <stdio.h>
#define SIZE 10

char mybuf[25];
CRITICAL_SECTION cs[SIZE];
HANDLE mReady[SIZE];
int sharedValue=0;

DWORD WINAPI first(LPVOID Param)
{
		int myid=*(int*)Param;
 	int myval;
   fprintf(stderr,"Head thread %d is called...\n",myid);
   Sleep(1);
   EnterCriticalSection(&cs[myid]);
  fprintf(stderr,"Head thread [%d] taking shared value\n",myid);

   myval = sharedValue;
   myval +=10;
   sprintf(&mybuf[0], "Hi 415 from Windows thread %d;  read critical value of %d\n",myid,myval);
   fprintf(stderr,"Head thread [%d] re-writing shared value as %d.\n",myid,myval);
		sharedValue=myval;
   LeaveCriticalSection(&cs[myid]);
   SetEvent(mReady[myid]);
   return 0;
}

DWORD WINAPI second (LPVOID Param)
{
	int myid =*(int*)Param;
	int myval;
   fprintf(stderr,"thread [%d] waiting for%d!\n",myid,myid-1);

   Sleep(1);

   WaitForSingleObject (mReady[myid-1], INFINITE);
   fprintf(stderr,"thread [%d] got signal from %d, and is entering Critical section!\n",myid,myid-1);

   EnterCriticalSection(&cs[myid]);
   fprintf(stderr,"Head thread [%d] taking shared value in Critical Section\n",myid);

   myval = sharedValue;
   myval +=10;
   sprintf(&mybuf[0], "Hi 415 from Windows thread %d;  read critical value of %d\n",myid,myval);
   fprintf(stderr,"Head thread [%d] re-writing shared value as %d.\n",myid,myval);
		sharedValue=myval;
   fprintf(stderr,"Head thread [%d] leaving critical section\n",myid);

   LeaveCriticalSection(&cs[myid]);
   //release
   SetEvent(mReady[myid]);
   return 0;
}

int main ()
{
   DWORD ThreadID[SIZE];
   HANDLE ThreadHandle[SIZE];
   int i=0;
   int Param[SIZE];

   for(i=0;i<SIZE;i++){
   		Param[i]=i;
   		InitializeCriticalSection (&cs[i]);
   		mReady[i] = CreateEvent (NULL,FALSE,FALSE, NULL);

   	}
   
for(i=1;i<SIZE;i++){
	      fprintf(stderr,"Making thread %d:  ",i);

 ThreadHandle[i] = CreateThread(
      NULL,
      0,
      second,
      &Param[i],
      0,
      &ThreadID[i]);

   if (ThreadHandle[i] == NULL) {
      fprintf(stderr,"Can't create thread %d\n",i);
      return -1;
   }  
   fprintf(stderr,"thread [%d] created\n",i);
}

   fprintf(stderr,"Making thread [0]:  ");
   ThreadHandle[0] = CreateThread(
      NULL,
      0,
      first,
      &Param[0],
      0,
      &ThreadID[0]);

   if (ThreadHandle[0] == NULL) {
      fprintf(stderr,"Can't create first thread\n");
      return -1;
   }
      fprintf(stderr,"thread [0] created\n");

        fprintf(stderr,"\nNow executing threads\n");

   for(i=SIZE-1;i>=0;i--){
   	  fprintf(stderr, "Waiting for thread %d to exit\n",i);

   		WaitForSingleObject(ThreadHandle[i],INFINITE);
   		   	  fprintf(stderr, "   Closing thread %d.\n",i);

   		CloseHandle(ThreadHandle[i]);
 }
 /*
   fprintf(stderr, "Waiting for second thread to exit\n"); 
   WaitForSingleObject(ThreadHandle2,INFINITE);
   CloseHandle(ThreadHandle2);
*/

  DeleteCriticalSection(&cs[SIZE-1]);

  for(i=SIZE-1;i>=0;i--){
  	//fprintf(stderr, "->Shutting down [%d];\n",i); 
   	//DeleteCriticalSection(&cs[i]);
    //fprintf(stderr, "-->closing [%d];\n",i); 
   	CloseHandle(mReady[i]);
 }
   fprintf(stderr,"Main Routine Exiting\n");
}
