# pthreads-with-mutex-locks
Linux c example with pthreads and Mutex locks

This file explores various methods to fix the race conditions in multi-processes, multi-threaded program. When processes 
and threads access the same resources, race condition will occur and data or output will not be the same as expected. 
This program uses signals, semaphores and mutex locks to prevent Race Conditions.

Part 1: Process:  10 processes will be created, each process will have an array contains number from 1 to 10. Then we will sleep for 
     a random number of milliseconds to induce Race Condition as in assignment 3. Then we fix by adding sigwait and kill. 

Part 2: Threads: The program will have a global counter variable. 10 threads will be created, each thread will have a temporary 
     local variable copied from global counter, local variable will be increased by 10, and then local variable will be copied back to 
     global counter to induce Race Condition.  We will fix the race condition using a semaphore. 
 
The program is compiled using gcc for Linux and Visual Studio compiler for Windows. Codes with comments are shown below. 

Linux usage: ./SyncProcess  and  ./SyncThread . Windows usage:  WThread.exe 

Design: 
Part1: Processes: we will use for loop to initialize processes. The child will allocate an array for number from 1 to 10. 
     All printing will be done in the child. Both versions will use BUFFER_SIZE and SIZE constant to initialize array of processes 
     instead of dynamic allocation.  

Unix version: 1 file will contain both parent and children. A loop will fork 10 process children. Then we use sigaction and sigwait 
     to block and receive signal from child before parent process start execute. 

Part2: Threads: Both implementations will use a loop to create 10 threads. The thread number will be casted from integer to void* 
     and passed in subroutine code of thread. Inside the thread function, the thread number will be recasted back to integer to 
     be printed. Mutex lock is defined in main and all threads are locked except first thread. First thread will finish execute and 
     unlock the next thread. Each thread will enter critical section before print its welcome and processing data. Local variable 
     will be incremented. Finally main will print the global counter after all threads finish processing. 
 
Codes are written with error checking already performed in order to ensure that the threads and process are created correctly, 
     writing to stderr without error and handle are closed after using. Signal is checked if it is received. If error to occur, 
     the appropriate message will be displayed.
      
Both Linux and Windows versions of the program were run multiple times from the command line for consistency. 
 



 
Unix Version 

Function Used: 
fprintf() sprint() 

fprintf() function formats a stream of character and prints to specified output. The function returns an integer represents the 
     number of character printed not including the end of file character. 

sprintf() is the same as fprintf but instead of prints to an 
     output, sprintf prints to a buffer. To use these functions, we needs to include <stdio.h> 
     format used in this program is %d for integer output and %s for string output 
     stream used in this program is stderr 
 
perror() 
The perror() produces a message about the error encountered to standard output. To use this function, we needs <stdio.h> 
 
fork() 
The fork() function creates a child process from a current process. Fork returns 2 times, if return value is 0, it is in the child process. If return value is greater than 0, it is in the parent. If return value is -1, there is an error. Requires <sys/types.h> , <unistd.h> 
 
getpid() 
The getpid() function gives the process ID of current process. This function doesn’t have return value. Requires <sys/types.h>,  <unistd.h> 
 
strerror() 
The strerror() function returns the description of error of the error number passed in. This function returns a string of description of the error. Requires <string.h> 
 
wait() 
The wait() function stops the execution of current process and waits for a child or specified process. This function returns the terminated process ID number or -1 if there is error. Requires <sys/types.h>, <sys/wait.h> 
 
sleep(), usleep() 
The sleep() function stops the execution of current process for specified number of seconds. usleep() function stops the execution of current process for specified nanoseconds. Requires <unistd.h> 
 
pthread_self() 
The pthread_self() function gives the current calling thread ID number. This function is always success. Requires <pthread.h> 
 
pthread_create() 
The pthread_create() function is used to create a new thread from current running process or thread. New thread get a thread ID and starts execution. Successful thread creation will return 0. 
Requires <pthread.h> 
 
pthread_join() 
The pthread_join() function stops the execution of current running process or thread and waits for the specified thread to return or terminated. Successful join thread will return 0. Requires <pthread.h> 
 
sigemptyset() 
The sigemptyset() clears the sig struct of any signals. Successful empty the set will return 0. Requires <signal.h> 
 
sigaction() 
The sigaction() is used to change the action of the signal when invoked.  Successful change of action will return 0, else return -1. Requires <signal.h> 
 
sigaddset() 
The sigaddset() adds specified signal into the signal set. Successful add will return 0. Requires <signal.h> 
 
sigprocmask() 
The sigprocmask() is used to change the signal mask to the calling process. It is used to block the calling process. Successful change will return 0, else -1 is returned. Requires <signal.h> 
 
sigwait() 
The sigwait() stops the execution of the current process or thread until a signal is arrived. The functions store return signal in sig parameter. Successful sigwait will return 0, else a positive number is returned. Requires <signal.h> 
 
kill() 
The kill() sends a signal to specified process. Successful signal sent will return 0, else -1 is returned. Requires <sys/types.h> <signal.h> 
 
sem_post(), sem_wait() 
The sem_wait() subtract 1 to the semaphore, if the value is 0, the thread will block. 
The sem_post() add 1 to the semaphore, and if value is greater than 0 the thread will continue execution. 
 
pthread_mutex_lock(), pthread_mutex_unlock() 
The pthread_mutex_lock() allows the code to execute in mutual exclusion. 
The pthread_mutex_unlock() releases the mutual exclusion previously locked by mutex_lock. 
Requires <semaphore.h> 
