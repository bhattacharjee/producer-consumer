#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "sem_shm_ops.h"

#define SEM_KEY1 0x200
#define SEM_KEY2 0x300




void producer(int* ptr, int semid1, int semid2)
{
    int i = 0;
    for (; i < 10; i++)
    {
        sem_wait(semid1);
        *ptr = i;
        sem_signal(semid2);
        sleep(1);
    }
}


void consumer(int* ptr, int semid1, int semid2)
{
    int i = 0;
    for (; i < 10; i++)
    {
        sem_wait(semid1);
        printf("%d\n", *ptr);
        sem_signal(semid2);
    }
}

int main(int argc, char **argv)
{
    int* ptr;
    int semid1, semid2;

    ptr = (int*)shmem_create();
    if (!ptr)
        exit(1);

    semid1 = sem_create(SEM_KEY1);
    if (-1 == semid1)
        exit(1);
    sem_set(semid1, 1);

    semid2 = sem_create(SEM_KEY2);
    if (-1 == semid2)
        exit(1);
    sem_set(semid2, 0);

    if (fork())
        producer(ptr, semid1, semid2);
    else
        consumer(ptr, semid2, semid1);

    /*
     * This leaks the seamphores and the shared memory.
     *
     * TODO: cleanup the semaphores and the shared memory
     * before exiting.
     */
}
