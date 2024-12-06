#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>

using namespace std;

class ThreadPool 
{
public:
    ThreadPool();
    ~ThreadPool();

    void enqueue(const function<void()>& job);
    bool empty();

private:
    vector<thread> workers;
    queue<function<void()>> tasks;

    mutex queueMutex;
    condition_variable condition;
    bool stop;
};