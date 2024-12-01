#include "ThreadPool.h"

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>

using namespace std;

ThreadPool::ThreadPool() : stop(false) 
{
    for (unsigned int i = 0; i < thread::hardware_concurrency(); ++i) 
    {
        workers.emplace_back([this] {
            for (;;) {
                function<void()> task;

                {
                    unique_lock<mutex> lock(this->queueMutex);
                    this->condition.wait(lock, [this] 
                        {
                        return this->stop || !this->tasks.empty();
                    });

                    if (this->stop && this->tasks.empty()) 
                    {
                        return;
                    }

                    task = move(this->tasks.front());
                    this->tasks.pop();
                }

                task(); 
            }
        });
    }
}

ThreadPool::~ThreadPool() 
{
    stop = true;
    condition.notify_all();

    for (thread& worker : workers) 
    {
        if (worker.joinable())
            worker.join();
    }
}

void ThreadPool::enqueue(const function<void()>& job) 
{
    {
        unique_lock<mutex> lock(queueMutex);
        tasks.emplace(job);
    }
    condition.notify_one();
}