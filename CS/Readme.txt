/*=============================================================================

CS 50300 Operating Systems Lab3 - Inter-Process Communication Engines (SystemV Version)

SystemV.c Source code		

Kartik Sooji (ksooji)

=============================================================================*/


The purpose of the lab is to perform communication using SystemV IPC(Shared memory)


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
The physical location of the shared memory is acquired. For this memory to be shared fork() technique is used where application process
acts as child process and driver process acts a parent. Hence driver writes and the application reads.


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

	Summation of the Random Numbers at Client : 500006.6101
	Performance of the Client is 127753.000000 Micro Seconds


	Summation of Random Numbers at Server : 500005.9694
	Performance of the Server is 456870.000000 Miro Seconds


