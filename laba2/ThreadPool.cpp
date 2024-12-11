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
    unsigned int tCount = thread::hardware_concurrency();
    if (tCount == 0)
        tCount = 1;
    for (unsigned int i = 0; i < tCount; ++i) 
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

                    if (this->stop) 
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
    cout << "ThreadPool started, thread count: " << tCount << endl;
}

ThreadPool::~ThreadPool() 
{
    stop = true;
    condition.notify_all();

    cout << "ThreadPool destroying..." << endl;
    for (thread& worker : workers) 
    {
        if (worker.joinable())
            worker.join();
    }
    cout << "ThreadPool destroyed" << endl;
}

void ThreadPool::enqueue(const function<void()>& job) 
{
    {
        lock_guard<mutex> lock(queueMutex);
        tasks.emplace(job);
    }
    condition.notify_one();
}

bool ThreadPool::empty() 
{
    return tasks.empty();
}