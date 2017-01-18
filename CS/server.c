#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
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
	int sock_fd, cln_fd;
    	double sum = 0 , temp = 0 , tm = 0;
	int done = 0 , n=0 ;    
	char buf[4096];


	struct sockaddr_un local_addr, cln_addr;
	socklen_t cln_size;
	long int start = 0.0, stop = 0.0;

	printf ("\n");

	// Creation of the Socket that returns a socket file descriptor
	sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	
	if (sock_fd < 0 )
		error("Socket Error\n");
	
	//printf("Socket file Descriptor : %d\n", sock_fd );

	// Initializing the socket memory to zero	
	bzero(&local_addr, sizeof(struct sockaddr_un));
   

	//Setting the local address and initializing the socket path for the file
	local_addr.sun_family = AF_UNIX;
	strcpy(local_addr.sun_path, SOCK_PATH);

	unlink(local_addr.sun_path);

	// bind to the client created socket fd and structure     		
	if (bind(sock_fd, (struct sockaddr *) &local_addr, sizeof(struct sockaddr_un)) < 0 )
		error("Bind Error\n");

	//Server keeps the socket open and listens to the client
	if ( listen(sock_fd, 8) < 0) 
		error("Listen Error\n");

	//accessing the client data
	cln_size = sizeof(struct sockaddr_un);
	
	// Initializing the buffer memory to zero
	bzero(buf, 128);

	// Read the data written by the client to the socket
    	cln_fd = accept(sock_fd, (struct sockaddr *) &cln_addr, &cln_size);
    	
	if (cln_fd < 0)
		error("Accept Error");


	// Start time to measure the performance of the server 
	start = get_time();

	n = recv(cln_fd,buf,4096,0); 
	
	while ( n > 0  )
	{	
		n = recv(cln_fd,buf,4096,0);
		sscanf(buf, "%lf" , &temp); 
		sum = sum + temp;
		//printf("loop Information : %f\n", sum );

	}
	
	// Start time to measure the performance of the server 
	stop = get_time();
	
	printf("Summation of Random Numbers at Server : %.4f\n", sum );
	printf("Performance of the Server is %lf Micro Seconds\n\n", (stop - start));

	// close the opened socket of the server 
	close(cln_fd);
}
