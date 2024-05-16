
// #include <pthread.h>
// int balance = 0;
// pthread_mutex_t mutex;
// // write the new balance (after as simulated 1/4 second delay)
// void write_balance(int new_balance)
// {
//   usleep(250000);
//   balance = new_balance;
// }

// // returns the balance (after a simulated 1/4 seond delay)
// int read_balance()
// {
//   usleep(250000);
//   return balance;
// }

// // carry out a deposit
// void* deposit(void *amount)
// {
//   // lock the mutex
//   pthread_mutex_lock(&mutex);

//   // retrieve the bank balance
//   int account_balance = read_balance();

//   // make the update locally
//   account_balance += *((int *) amount);

//   // write the new bank balance
//   write_balance(account_balance);

//   // unlock to make the critical section available to other threads
//   pthread_mutex_unlock(&mutex);

//   return NULL;
// }

// int main()
// {
//   // mutex variable
  

//   // output the balance before the deposits
//   int before = read_balance();
//   printf("Before: %d\n", before);

//   // we'll create two threads to conduct a deposit using the deposit function
//   pthread_t thread1;
//   pthread_t thread2;

//   // initialize the mutex
//   pthread_mutex_init(&mutex, NULL);

//   // the deposit amounts... the correct total afterwards should be 500
//   int deposit1 = 300;
//   int deposit2 = 200;

//   // create threads to run the deposit function with these deposit amounts
//   pthread_create(&thread1, NULL, deposit, (void*) &deposit1);
//   pthread_create(&thread2, NULL, deposit, (void*) &deposit2);

//   // join the threads
//   pthread_join(thread1, NULL);
//   pthread_join(thread2, NULL);

//    // destroy the mutex
//   pthread_mutex_destroy(&mutex);

//   // output the balance after the deposits
//   int after = read_balance();
//   printf("After: %d\n", after);

//   return 0;
// }
#include "./philo_bonus/philo_bonus.h"
// int main(int argc, char const *argv[])
// {
//   t_philos * head;

//   t_philos *ph;
//   t_philos *ph2;
//   // t_philos *ph;

//   ph = malloc(sizeof(t_philos));
//   ph2 = malloc(sizeof(t_philos));
//   ph->index = 0;
//   ph->next = ph2;
//   ph2->index = 2;
//   ph2->next = NULL;
//   head = ph;
//   printf ("size %d\n", sizeof(t_philos));
//   printf ("%p \n", &head->index);
//   printf ("%p \n", head + 208);
//   return 0;
// }
int main() {
    char sem_name[10];
    for (int i = 1; i <= 200; i++) {
        sprintf(sem_name, "/%d", i);
        sem_unlink(sem_name);
    }
    return 0;
}