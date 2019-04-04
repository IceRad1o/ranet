#pragma once
#include<vector>
#include<queue>
#include<mutex>


#include<utility>

class ThreadPool{
public:
    ThreadPool(size_t worker_num);
    void add(void (*func)(void*), void*arg);
private:
    size_t worker_num;
    pthread_mutex_t lock;
    pthread_cond_t notify;
}