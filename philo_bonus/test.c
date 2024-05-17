#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

// int main(int argc, char const *argv[])
// {
//     unlink("/hehehe");
//     sem_t *test = sem_open("/hehehe" ,O_CREAT, 0777, 1);
//     pid_t p = fork();
    
//     if (p == 0)
//     {

//         printf ("heh\n");
//         sem_wait(test);
//         while (1);
//     }
//     else if (p > 0)
//     {
//         usleep(100);
//         printf ("reeees %d\n", sem_wait(test));
//     }
//     return 0;
// }
int main(int argc, char const *argv[])
{
    sem_unlink("/hehehe");
    sem_t *test = sem_open("/hehehe" ,O_CREAT, 0777, 1);
    if (test == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    pid_t p = fork();
    if (p < 0) {
        perror("fork");
        sem_close(test);
        sem_unlink("/hehehe");
        return 1;
    }
    
    if (p == 0)
    {
        sem_wait(test);
            printf ("heh\n");
        // else
        //     printf ("NO\n");
        // if (sem_wait(test) == -1) {
        //     perror("sem_wait");
        //     return 1;
        // }
        while (1)
        {
        // printf ("reeees\n");
            
        }
    }
    else if (p > 0)
    {
        usleep(1000);
        // printf ("%d ",sem_wait(test));
        if (!sem_wait(test))
            printf ("heh\n");
        else
            printf ("NO\n");
        }

    

    sem_close(test);

    return 0;
}