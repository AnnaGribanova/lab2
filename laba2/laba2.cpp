#include <iostream>
#include "ThreadPool.h"

void myFunction() 
{
	printf("myFunction\n");
}

int main()
{
	ThreadPool pool;

	for (int i = 0; i < 50; ++i)
	{
		pool.enqueue(bind(myFunction));
	}

	while (!pool.empty());
}