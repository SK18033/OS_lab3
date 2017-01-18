#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


#define BUFFER_SIZE 	256
#define MEM_NAME	"/POSIX"


//Ring Buffer data structure
struct RingBuffer
{
	double buffer[BUFFER_SIZE];
	int head;
	int tail;
	
};


// User defined function to print errors
void error(char* error)
{
	perror(error);
	exit(1);
}


//Function declerations
void driver_process();
void application_process();
struct RingBuffer* shared_area(size_t poolsize_per_core, int *shm_fd);


// The below function caluctates the current time and returns
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
	int 	shm_fd;
	long int msg_cnt ;
	
	//rbuf returned as the shared memory 
	rbuf = shared_area(sizeof(*rbuf), &shm_fd);

	rbuf-> head = 0;
	rbuf-> tail = 0;

	printf("Enter the number of messages to be communicated\n");
	scanf ("%ld", &msg_cnt);


	//I'm Application Process
	if( fork() == 0 ) 	
		application_process(rbuf, &shm_fd, msg_cnt);
	//I'm Driver Process
	else
		driver_process(rbuf, msg_cnt);

	return 0;
}



// The below method rerurns the required shared memory
struct RingBuffer* shared_area(size_t size, int *shm_fd) 
{
	struct RingBuffer  *rbuf;
	
	//creates a memory as a file as returns the descriptor
	if ((*shm_fd = shm_open(MEM_NAME, O_CREAT | O_RDWR , 0666)) == -1 ) 
		error("Error: shmopen() failed\n");

	// The memory created is truncated according to the size mentioned
	if(ftruncate(*shm_fd, BUFFER_SIZE)!= 0)
		error("Error: ftruncate() failed\n");
	
	rbuf = (struct RingBuffer*)mmap(0, BUFFER_SIZE, PROT_WRITE | PROT_READ , MAP_SHARED, *shm_fd, 0);
	
	if ( rbuf == MAP_FAILED) 
		error("Error: mmap() failed\n");
	
	return rbuf;
}

void driver_process(struct RingBuffer* rbuf, long int cnt) 
{
  
  	int status;
	long int i = 0;
	double 	sum = 0.0, r_num = 0.0 ; 
	int flag = 0;
	double start, stop;
	
	printf("\nDriver Writing data to the buffer.....\n\n");


	start = get_time();
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


	printf("Driver writes data to shm segment!\n\n");

	//printf("Rbuf Ddata -  %d, %d", rbuf->head, rbuf->tail);

	printf("\nApplication reading data from the buffer.....\n\n");
	
	wait(&status);
	
	printf ("The Driver process summation is %.2f \n", sum);
	printf("Performance of the Driver process is %.2f MicroSeconds\n\n", (stop - start));
	
}


void application_process(struct RingBuffer* rbuf, int *shm_fd, long int cnt) 
{
	sleep(2);

	long int i = 0 ;
	double	sum= 0.0 , temp;
	int flag = 0;
	
	double start, stop;

	// Start time to measure the performance	
	start = get_time();
	
	
	i = cnt;
	while(i > 0){
		
		while(rbuf->head == rbuf->tail);
		temp = rbuf->buffer[rbuf->tail];
		sum += temp;
		rbuf->tail = (rbuf->tail + 1) % BUFFER_SIZE;
		 
		i--;
	}

	// Stop time to measure the performance 
	stop = get_time();


	if(shm_unlink(MEM_NAME) == -1)
    		error("Application: Can't detach memory segment\n");
   
   	printf("Application: detached memory segment\n\n");
	
   	//printf("Rbuf Ddata -  %d, %d\n", rbuf->head, rbuf->tail);


	printf ("The Application process summation is %.2f \n", sum);
	printf("Performance of the Application process is %.2f MicroSeconds\n\n\n", (stop-start));

}
