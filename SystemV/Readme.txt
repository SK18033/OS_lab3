/*=============================================================================

CS 50300 Operating Systems Lab3 - Inter-Process Communication Engines (POSIX Version)

Posix.c Source code		

Kartik Sooji (ksooji)

=============================================================================*/


The purpose of the lab is to perform communication using posix IPC(Shared memory)


There are 3 functionalities that are supported by the program:

void error(char *error)
struct RingBuffer* shared_area(size_t poolsize_per_core, int *shm_fd);
void driver_process();
void application_process();
int main()

void error(char *error):
~~~~~~~~~~~~~~~~~~~~~~~
It is a generic user defined error display function which catches error status and displays the error accordingly.

struct RingBuffer* shared_area(size_t poolsize_per_core, int *shm_fd):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This function creates a block of memory as ring buffer which can be accessible both for reading and writing.
This memory is shared among driver and application process and they operate on the memory based on the shared file descriptor.

void driver_process():
~~~~~~~~~~~~~~~~~~~~~
This function is used for writing data to the head of shared memory ring buffer.

void application_process():
~~~~~~~~~~~~~~~~~~~~~~~~~~
This function reads the data from the tail of the shared memory ring buffer. This process sleeps for two seconds initially
inorder for the driver process to write some data on to the buffer.

double get_time():
~~~~~~~~~~~~~~~~~
This function is used to check performance of the communication and returns the current time in microseconds. 

int main():
~~~~~~~~~~~~
There are few inbuilt functions that would help in socket creation and communication between two processes.
This can be fulfilled by creating the Socket that returns a socket file descriptor value and temporary buffer is 
created in order to read the data that client writes to the socket. The server can read as it binds the client created 
socket fd and Server keeps the socket open and listens to the client until the client is done with writing.
Finally the connection between the client and server is closed.


Exceution Steps :

1) Change the current working directory to the source code location
2) Make sure there is a ".c" files
3) Type make and hit enter
4) Provide appropriate inputs as prompted
5) Once the program is done run make clean 



Performance Results : 
---------------------
1)

	Enter the number of messages to be communicated
	1000000

	The Application process summation is 500006.61 
	Performance of the Application process is 19539.00 Micro Seconds

	The Driver process summation is 500006.61 
	Performance of the Driver process is 52744.00 Micro Seconds



2)
	Enter the number of messages to be communicated
	2000000

	The Application process summation is 999935.44 
	Performance of the Application process is 27583.00 Micro Seconds


	The Driver process summation is 999935.44 
	Performance of the Driver process is 191.00 Micro Seconds



3)
	Enter the number of messages to be communicated
	3000000


	The Application process summation is 1499883.89 
	Performance of the Application process is 88558.00 Micro Seconds


	The Driver process summation is 1499883.89 
	Performance of the Driver process is 77584.00 Micro Seconds





