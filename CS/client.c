#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

#define SOCK_PATH "Ksooji_Socket"

// User defined function to print errors
void error(char *error)
{
	perror(error);
	exit(1);
}


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


void main()
{
	int sock_fd, n = 0, count = 0;
    	double sum = 0, r_num = 0, tm = 0;
    	char buf[4096];

    	struct sockaddr_un local_addr;
	double start = 0.0, stop = 0.0;


	printf ("\n");

	// Creation of the Socket that returns a socket file descriptor
    	sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    	
	if (sock_fd == -1)
		perror("Socket Error\n");

	//printf("Socket file Descriptor : %d\n", sock_fd );

	// Initializing the socket memory to zero
	bzero(&local_addr, sizeof(struct sockaddr_un));

	//Setting the local address and initializing the socket path for the file
   	local_addr.sun_family = AF_UNIX;
    	strcpy(local_addr.sun_path, SOCK_PATH);	       


	// Connecting to the above created socket fd and structure     	
	if ( connect(sock_fd, (struct sockaddr *) &local_addr, sizeof(struct sockaddr_un)))
		perror("Connection Error\n");

	// Initializing the buffer memory to zero
	bzero(buf, sizeof(buf));
	printf ( "Enter the number of random numbers to be generated : ");
	scanf("%d", &count);
	//printf ( "\n\n");


	// Start time to measure the performance of the server 
	start = get_time();

	do 
	{
		r_num = (double)rand() / (double)RAND_MAX;
		snprintf(buf,sizeof(buf), "%f", r_num); 
	
		if ((n = send(sock_fd,buf,4096,0)) < 0 )
			perror("Send Error\n");

		sum = sum + r_num ;
		count-- ; 

		//printf("count = %d \n", count);
		//printf("loop Information r_num  : %f\n", r_num );
		//printf("loop Information  sum : %f\n", sum );

	}while ( count );
	
	// Start time to measure the performance of the server 
	stop = get_time();
	
	printf("Summation of the Random Numbers at Client : %.4f\n", sum );
	printf("Performance of the Client is %lf Micro Seconds\n\n", (stop - start));
	

	// close the opened socket of the client 
	close(sock_fd);
}
