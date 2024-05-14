#include "philo_bonus.h"
int main()
{
    sem_unlink("/forks");
    sem_unlink("/print");
    return 0;
}