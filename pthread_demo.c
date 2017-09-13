/* to compile:  gcc -o pthread_demo pthread_demo.c -l pthread
 *
*/ 

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

//My added declarations
#define MAXQUEUESIZE 100

pthread_mutex_t my_lock;//mutex for thread lock
	
int 	buf_size_n;//bounded queue buffer size
int	p_count, c_count;//# of producers/consumers
int 	x_num;//number each producer will place into the buffer
int 	consume_num;//number of buffered items each consumer will consume
int	p_time, c_time;// time each producer/consumer will wait after each iteration
int	buf_counter;//number of items currently in the buffer

int 	i;//for counter

sem_t empty;
sem_t full;
int     head, tail;//bbqueue head and tail
int     bbqueue[MAXQUEUESIZE];//Bounded buffer queue

void producer(void	*firstarg) {
	unsigned long int mytid=pthread_self();//current id
	int threadno=*(int*)firstarg;
	int value;

	fprintf(stderr, "hello from producer #%lu, with tid#%lu.\n", *(unsigned long int*)firstarg, mytid);
//produce if there is room in the queue

for(i=0;i<p_count;i++){
	value = rand() %100;//create a random value for each produced threads buffer addition.
	fprintf(stderr, ":- producer #%lu, with tid#%lu checking buffer size.\n",*(long int *)firstarg,mytid);
	if(buf_counter>=buf_size_n){
		fprintf(stderr, ":- producer #%lu, with tid#%lu waiting for empty buffer.\n",*(long int *)firstarg,mytid);
		sem_post(&full);	
		sem_wait(&empty);
	}
	fprintf(stderr, ":- producer #%lu, with tid#%lu locking.\n",*(long int *)firstarg,mytid);
	pthread_mutex_lock(&my_lock);

	fprintf(stderr, ":-- producer #%lu, with tid#%lu inside lock.\n",*(long int *)firstarg,mytid);

	if(enqueue(value)==0){
		fprintf(stderr,"Error:enqueueing!\n");	
	}

		fprintf(stderr, ":--- producer #%lu, with tid#%lu has increased buffer size!\n  new size = %d.\n", *(long int *)firstarg, mytid, buf_counter);

		pthread_mutex_unlock(&my_lock);
		usleep(p_time);
	}
		fprintf(stderr, ":- producer #%lu, with tid#%lu has completed and is releasing lock.\n",*(long int *)firstarg,mytid);
sem_post(&full);
		fprintf(stderr, ":- producer #%lu, with tid#%lu has released lock.\n",*(long int *)firstarg,mytid);
return;
}

int consumer(void	*firstarg) {
	unsigned long int mytid=pthread_self();//current id
	int threadno=*(int*)firstarg;
	int value;

	fprintf(stderr, "hello from consumer #%lu, with tid#%lu.\n", *(long int*)firstarg, mytid);
	for(i=0; i< consume_num; i++){
	fprintf(stderr, ":- consumer #%lu, with tid#%lu checking.\n",*(long int *)firstarg,mytid);
	if(buf_counter<=0){
		fprintf(stderr, ":- consumer #%lu, with tid#%lu waiting for some data in the buffer.\n",*(long int *)firstarg,mytid);
		sem_post(&empty);
		sem_wait(&full);
}
	fprintf(stderr, ":- consumer #%lu, with tid#%lu locking.\n",*(long int *)firstarg,mytid);
		//wait for some product to consume	
		pthread_mutex_lock(&my_lock);

		fprintf(stderr, ":-- consumer #%lu, with tid#%lu inside lock.\n",*(long int *)firstarg, mytid);
		
		value=dequeue();

		fprintf(stderr, ":--- consumer #%lu, with tid#%lu has decreased buffer size!\n  new size=%d.\n",*(long int *)firstarg,mytid,buf_counter);
		pthread_mutex_unlock(&my_lock);
		usleep(ctime);
	}
sem_post(&empty);
return value;

}

int main(int argc, char* argv[]) {
	int i;
	int	retvalue;

	if(argc != 7)
	{
		fprintf(stderr, "This program requires 6 integer arguments:\n    thread [queue_size] [num_producers] [num_consumers] [num_each_producer_produces] [p_spin_time] [c_spin_time]\n");
		exit(0);

	}

	buf_size_n= atoi(argv[1]);
	p_count = atoi(argv[2]);
	c_count = atoi(argv[3]);
	x_num = atoi(argv[4]);
	p_time = atoi(argv[5]);
	c_time = atoi(argv[6]);
              
	consume_num= p_count* (x_num / c_count);

	pthread_t producers[p_count];//One for each producer
	pthread_t consumers[c_count];//One for each consumer
	
	int	producerid[p_count];
	int 	consumerid[c_count];

	//
	if ((retvalue=pthread_mutex_init(&my_lock,NULL))<0){
		error("Create  mutex  failed!");
		exit(-1);
	}

	//create an array of digits with which to uniquely identify each child	
	for(i=0;i<p_count;i++){
		producerid[i]=i;
	}
//create an array of digits with which to uniquely identify each child	
	for(i=0;i<c_count;i++){
		consumerid[i]=i;
	}
	
	fprintf(stderr,"Test program\nThis program will create %d producers, and %d consumers.\n    Each producer will place %d numbers into a buffer, while \n    each consumer consumes %d numbers from the buffer.\n", p_count, c_count, x_num, consume_num);

	fprintf(stderr,"\n");

//start by creating the producer threads
	for(i=0; i< p_count; i++){
		if ((retvalue=pthread_create(&producers[i],NULL, (void *)producer, (void *)&producerid[i])) < 0){
			perror("Unable to create producer thread.");
			exit(-1);
		}
	}
//start by creating the producer threads
	for(i=0; i< c_count; i++){
		if ((retvalue=pthread_create(&consumers[i],NULL, (void *)consumer, (void *)&consumerid[i])) < 0){
			perror("Unable to create producer thread.");
			exit(-1);
		}
	}

//now join the consumer threads
	for(i=0; i< c_count; i++){
			pthread_join(consumers[i],NULL);
	}
//finally, join the producer threads
	for(i=0; i< p_count; i++){
			pthread_join(producers[i],NULL);
	}
	fprintf(stderr,"Main routine exiting: \n");
}

int enqueue(int value){
	fprintf(stderr,"Enqueueing %d.\n",value);

	//tail=(tail+1) % (buf_size_n+1);
	if(num_enqueued >= buf_size_n){
		printf ("Queue is currently full\n");
		return -1;	
	}
	//buf_counter++;	
	//bbqueue[tail]=value;
	if((buf_size_n -num_enqueued) > 0){
		bbqueue[head++]=value;
	}
	if(head==buf_size_n) {head=0;}
	num_enqueued++;
	return 0;
}

int dequeue (int value){
	if (head == tail){
		printf ("Queue is empty.\n");
		return 0;
	}
	head = (head+1) %(buf_size_n + 1);
	buf_counter--;
	return bbqueue[head];
}
