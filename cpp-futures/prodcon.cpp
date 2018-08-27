#include <future>
#include <stdio.h>
#include <sys/types.h>

#define NUM_CONSUMERS 10

bool consumer(int N)
{
    printf(
            "0x%lx:%d\n",
            (long)pthread_self(),
            N);
    return true;
}

void producer()
{
    int i = 0;
    std::future<bool> fu[NUM_CONSUMERS];
    for (; i < NUM_CONSUMERS; i++)
    {
        fu[i] = std::async(
                    std::launch::async | std::launch::deferred,
                    consumer,
                    i);
    }

    for (i = 0; i < NUM_CONSUMERS; i++)
    {
        bool done = fu[i].get();
		printf(
			"0x%lx: Consumer %d is done\n",
			(long)pthread_self(),
			i);
    }
}

int main()
{
	producer();
}
