#include <future>
#include <stdio.h>
#include <sys/types.h>

bool consumer(int N)
{
    printf("0x%lx:%d\n", (long)pthread_self(), N);
    return true;
}

void producer()
{
    int i = 0;
    std::future<bool> fu[10];
    for (; i < 10; i++)
    {
        fu[i] = std::async(
                    std::launch::async | std::launch::deferred,
                    consumer,
                    i);
    }

    for (i = 0; i < 10; i++)
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
