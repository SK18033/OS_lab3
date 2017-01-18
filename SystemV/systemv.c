#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_SIZE 2000000

struct RingBuffer
{
	double buffer[BUFFER_SIZE];
	int head;
	int tail;
};

void error(char* error)
{
	perror(error);
	exit(1);
}

void driver_process();
void application_process();
struct RingBuffer* shared_area(size_t poolsize_per_core, int* shm_id);



double get_time()
{
	struct timeval tv;
	struct timezone tz;
	double cur_time;

	gettimeofday(&tv,&tz);
	cur_time = tv.tv_sec + tv.tv_usec; 

	return cur_time;
}

int main(int argc, char* argv[]) 
{
	
	struct 	RingBuffer* rbuf;
	int 	shm_id, msg_cnt;
	double	appn_sum , dvr_sum;
	
	time_t appn_start, appn_stop, dvr_start, dvr_stop;
	
	rbuf = shared_area(sizeof(*rbuf), &shm_id);

	printf("Enter the number of messages to be communicated\n");
	scanf ("%d", &msg_cnt);


	if( fork() == 0 ) //I'm Application Process
		application_process(rbuf, msg_cnt);
	else //I'm Driver Process
		driver_process(rbuf, shm_id, msg_cnt);
	
	return 0;
}


struct RingBuffer* shared_area(size_t size, int* shm_id) 
{
	void			*shm;
	struct RingBuffer  	*rbuf;
	int                 	shm_flg = 0;
	
	shm_flg = IPC_CREAT | 0666;

	if ((*shm_id = shmget(IPC_PRIVATE, sizeof(*rbuf), shm_flg)) < 0) 
		error("Error: shmget() failed\n");

	if ((shm = shmat(*shm_id, NULL, 0)) == (void *) -1) 
		error("Error: shmat() failed\n");
		
	rbuf = (struct RingBuffer*) shm; 

	return rbuf;
}



void driver_process(struct RingBuffer* rbuf, int shm_id, int cnt) 
{
  
	int status, i = 0;
	double sum , r_num; 
	double start, stop;
	

	start = get_time();
	
	printf("\nDriver Writing data to the buffer.....\n\n");

	i = cnt;
	while(i > 0){

		while (((rbuf->head + 1) % BUFFER_SIZE) == rbuf->tail);

		r_num = (double)rand()/(double)RAND_MAX;
		sum += r_num;
		rbuf->buffer[rbuf->head] = r_num ;
		rbuf->head = (rbuf->head + 1) % BUFFER_SIZE;

		i--;
	}
	stop = get_time();

	printf("Driver writes data to shm segment!\n");
	
	wait(&status);
	
	if (shmdt(rbuf) == -1) 
		error("Driver: Can't detach memory segment\n");
	
	printf("Driver: detached memory segment\n");
  
  	/* Remove the shared memory segment */
  	if (shmctl(shm_id, IPC_RMID, NULL)) 
		error("Driver: Can't remove memory segment\n");

	printf("\nApplication reading data from the buffer.....\n\n");

  	printf("Driver: removed memory segment\n\n");
	
	printf ("The Driver process summation is %.2f \n", sum);
	printf("Performance of the Driver process is %.2f Micro Seconds\n\n\n", (stop - start));
}


void application_process(struct RingBuffer* rbuf, int cnt) 
{
  	sleep(2);
	int	i = 0;
	double 	sum, temp, start, stop;
	
	start = get_time();
	
	i = cnt;
	while(i > 0){
		
		while(rbuf->head == rbuf->tail);
		temp = rbuf->buffer[rbuf->tail];
		sum += temp;
		rbuf->tail = (rbuf->tail + 1) % BUFFER_SIZE;
		 
		i--;
	}
	stop = get_time();
	
	if(shmdt(rbuf) == -1)
    		error("Application: Can't detach memory segment\n");
   
   	printf("Application: detached memory segment\n\n");
	
	printf ("The Application process summation is %.2f \n", sum);
	printf("Performance of the Application process is %.2f Micro Seconds\n\n\n", (stop - start));
	
}
