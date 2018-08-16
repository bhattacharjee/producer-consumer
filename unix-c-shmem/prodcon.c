#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>

#define SHM_KEY 0x100
#define SEM_KEY1 0x200
#define SEM_KEY2 0x300



int* shmem_create()
{
    int *bucket;
    int shmid;

    if (-1 == (shmid = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0777)))
    {
        perror("shmget");
        return NULL;
    }

    if (-1 == (long)(bucket = shmat(shmid, NULL, 0)))
    {
        perror("shmat");
        return NULL;
    }

    return bucket;
}

int sem_create(key_t key)
{
    int semid;

    if (-1 == (semid = semget(key, 1, IPC_CREAT | 0777)))
    {
        perror("semget");
    }

    return semid;
}

int sem_set(int semid, int value)
{
    if (-1 == semctl(semid, 0, SETVAL, value))
    {
        perror("semctl");
        return -1;
    }
    return 0;
}

int sem_wait(int semid)
{
    struct sembuf sobs = {0};
    sobs.sem_op = -1;
    if(-1 == semop(semid, &sobs, 1))
    {
        perror("semop");
    }
}

int sem_signal(int semid)
{

    struct sembuf sobs = {0};
    sobs.sem_op = 1;
    if (-1 == semop(semid, &sobs, 1))
    {
        perror("semop");
    }
}

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
    
    ptr = shmem_create();
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
}
