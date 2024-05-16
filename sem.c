// C program to demonstrate working of Semaphores 
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <sys/stat.h>
#include <signal.h> 
#include <sys/wait.h>       /* For mode constants */
// #include <semaphore.h>
#include <stdlib.h>
# include <pthread.h>

void *test(void *ptr)
{
	while (1)
	{
	printf ("test\n");
	while (1);
	}
}


void *thread(sem_t *sem, sem_t *pr, int index, pthread_t *thread) 
{ 
	while (1)
	{
		// exit (1);
		pthread_create(thread, NULL, (void *)test, (void *)NULL);
		// sem_wait(sem);
		// sem_wait(pr);
		// write(1, "killo\n", 7);
		printf ("%d\n", index);
		// printf("Entered %d\n", index);
		// printf("Just Exiting %d\n", index);
		if (index == 2)
		{
			printf ("2 is going to exit rn\n");
			// sem_post(sem);
			exit (1);
		}
		// sem_post(pr);
		// sem_post(sem);
	}
    // exit (0);
	// return 0;
} 
#include <fcntl.h>


int main() 
{ 
	sem_t *sem = sem_open("/test", O_CREAT, 0644, 6);
	sem_t *pr = sem_open("/print", O_CREAT, 0644, 1);
	pthread_t thread_s[6];
	int j = 0;
	// while (j < 6)
	// {
	
	// }
	int arr[6];
	int i = 0;
	while (i < 6)
	{
		pid_t j = fork();
		if (j == 0)
		{
			// printf ("test\n");
			thread(sem, pr, i, &thread_s[i]);
			// exit (0);
		}
		else
		{
			arr[i] = j;
			i++;
		}
	}
	i = 0;
	// if ()
	while (1)
	{
		// printf ("hh1\n");
		wait(NULL);
		// printf ("hh2\n");
		while (i < 6)
		{
			kill(arr[i], SIGTERM);
			printf ("%d killed\n", i);
			i++;
		}
		usleep(50);
		break;
	}
	sem_close(sem);
sem_unlink("/test");
	return 0; 
} 
