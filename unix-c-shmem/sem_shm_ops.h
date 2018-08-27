#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>


int* shmem_create();
int sem_create(key_t key);
int sem_set(int semid, int value);
int sem_wait(int semid);
int sem_signal(int semid);
