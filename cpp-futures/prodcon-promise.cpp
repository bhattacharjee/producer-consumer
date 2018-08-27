#include <stdio.h>
#include <future>
#include <unistd.h>

/*
 * This is not a very good example of how to use promises. Promises
 * are meant to be used as barriers rather than as arrays to pass data.
 *
 * A better example of the use of promises is here:
 * https://en.cppreference.com/w/cpp/thread/promise
 *
 * - Every promise has an associated future.
 * - One promise can only be fulfilled once. Trying to fulil a promise twice
 * 	results in an exception
 */


void consumer(std::future<int>* fut)
{
    int i = 0;
    for (; i < 10; i++)
    {
        printf("Consumer got %d\n", (int)fut[i].get());
        sleep(2);
    }
}

void producer(std::promise<int>* prom)
{
    int i = 0;
    for (; i < 10; i++)
    {
        printf("Producer producing %d\n", i);
        prom[i].set_value(i);
        sleep(1);
    }
}

int main ()
{
    int i = 0;
    std::promise<int> prom[10];                      // create promise
    std::future<int> fut[10];

    for (; i < 10; ++i)
    {
        fut[i] = prom[i].get_future();    // engagement with future
    }

    std::thread th1 (consumer, fut);  // send future to new thread

    producer(prom);

    th1.join();
    return 0;
}
