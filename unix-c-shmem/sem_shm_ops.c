#define SHM_KEY 0x100
#include "sem_shm_ops.h"

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
