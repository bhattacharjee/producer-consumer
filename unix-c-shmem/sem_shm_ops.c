#define SHM_KEY 0x100
#include "sem_shm_ops.h"
#include <errno.h>
#include <stdio.h>

/*
 * Ideally, one should be able to create a shared memory of arbitrary size.
 * However, in WSL it seems that it is only possible to create a shared memory
 * that is four bytes long.
 * This implementation runs both on regular linux and WSL.
 */
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

/*
 * The semget() API creates an array of semaphores. In UNIX/Linux the array
 * can be of arbitrary length.
 * The current implementation uses two different semget() calls using two
 * different keys. Ideally, we should be able to use a single semget()
 * call with the same key to create an array of two semaphores, and use them.
 *
 * However, semget() in WSL is broken and it only creates a semaphore with
 * one element. This program runs on both WSL and regular linux, hence
 * there are two keys used.
 */
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
