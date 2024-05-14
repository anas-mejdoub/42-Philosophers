#include "philo_bonus.h"
int main()
{
    sem_unlink("/forks");
    sem_unlink("/print");
    sem_unlink("/1");
    sem_unlink("/2");
    sem_unlink("/3");
    return 0;
}