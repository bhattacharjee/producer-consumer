#include <futures>
#include <stdio.h>

bool consumer(int N)
{
    printf("%d\n", N);
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
		printf("Consumer %d is done\n", i);
    }
}
