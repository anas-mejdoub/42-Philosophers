#include <stdio.h>
#include <semaphore.h>

int main() {
    char sem_name[10];
    for (int i = 1; i <= 200; i++) {
        sprintf(sem_name, "/%d", i);
        sem_unlink(sem_name);
    }
    return 0;
}