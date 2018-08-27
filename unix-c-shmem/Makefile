all: prodcon

prodcon: sem_shm_ops.o prodcon.o sem_shm_ops.h
	cc -o prodcon sem_shm_ops.o prodcon.o

prodcon.o: prodcon.c
	cc -c prodcon.c

sem_shm_ops.o: sem_shm_ops.c sem_shm_ops.h
	cc -c sem_shm_ops.c

clean:
	rm -f *.o prodcon
