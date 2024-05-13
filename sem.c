// C program to demonstrate working of Semaphores 
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <sys/stat.h>
#include <signal.h>        /* For mode constants */
// #include <semaphore.h>


// sem_t mutex; 

void *thread(sem_t *sem, int index) 
{ 
	//wait 
	while (1)
	{
		sem_wait(sem); 
		printf("Entered %d\n", index); 

		//critical section 
		// sleep(4); 
		// usleep(1000);
		//signal 
		printf("Just Exiting %d\n", index);
	}
	// sem_post(&sem); 
    // exit (0);
	return 0;
} 
#include <fcntl.h>


int main() 
{ 
	sem_t *sem = sem_open("/test", O_CREAT, 0644, 3);
	int arr[6];
	int i = 0;
	while (i < 6)
	{
		pid_t j = fork();
		if (j == 0)
		{
			thread(sem, i);
			// exit (0);
		}
		else
		{
			arr[i] = j;
			i++;
		}
	}
	i = 0;
	usleep(1000);
	while (1);
	
	while (i < 6)
	{
		kill(arr[i], SIGKILL);
		i++;
	}
	sem_close(sem);
sem_unlink("/test");
	return 0; 
} 
