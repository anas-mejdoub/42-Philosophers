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


#include <stdio.h>
#include <semaphore.h>

int main() {
    // char sem_name[5];
    // for (int i = 1; i <= 200; i++) {
    //     sprintf(sem_name, "/%d", i);
    // }
        sem_unlink("/1");
        sem_unlink("/2");
        sem_unlink("/3");
        sem_unlink("/4");
    return 0;
}
