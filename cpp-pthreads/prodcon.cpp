#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <stdio.h>

std::queue<int> g_queue;
pthread_cond_t  condition;
pthread_mutex_t lock;

void* producer(void* arg)
{
    int i = 0;
    for (; i < 10; i++)
    {
        if (0 == pthread_mutex_lock(&lock))
        {
            g_queue.push(i);
            pthread_cond_signal(&condition);
            pthread_mutex_unlock(&lock);
        }
        sleep(1);
    }
    printf("Producer finished\n");
    return NULL;
}

void* consumer(void* arg)
{
    int i = 0, j;
    for (; i < 10; i++)
    {
        if (0 == pthread_mutex_lock(&lock))
        {
            while (g_queue.empty())
                pthread_cond_wait(&condition, &lock);
            j = g_queue.front();
            g_queue.pop();
            pthread_mutex_unlock(&lock);

            printf("Received %d\n", j);
            sleep(2);
        }
    }
    printf("Consumer finished\n");
    return NULL;
}

int main(int argc, char** argv)
{
    pthread_t h_producer, h_consumer;
    void *retval;

    pthread_create(&h_producer, NULL, producer, NULL);
    pthread_create(&h_consumer, NULL, consumer, NULL);
    pthread_join(h_producer, &retval);
    pthread_join(h_consumer, &retval);
}
