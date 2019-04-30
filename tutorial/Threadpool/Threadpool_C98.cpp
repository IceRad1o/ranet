//
// Created by Jialei Wang on 2019/4/24.
//

#include "Threadpool_C98.h"

Threadpool_C98::Threadpool_C98(size_t nums) :thread_num(nums),isrunning(true){
    createthreads();
}

Threadpool_C98::~Threadpool_C98() {
    stop();
    while(!taskqueue.empty())
    {
        Task *task = taskqueue.front();
        delete task;
        taskqueue.pop();
    }
}

size_t Threadpool_C98::size() {
    return taskqueue.size();
}

// used by constructor.
int Threadpool_C98::createthreads() {
    // 预留足够的空间
    threads.reserve(thread_num);
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_condition, NULL);
    for(int i = 0; i < thread_num; ++i){
        // 把当前线程池object作为参数传入static的execute函数中
        pthread_create(&threads[i], NULL, &thread_execute, (void *)this);
    }
    return 0;
}
size_t Threadpool_C98::add_task(Task *task){
    pthread_mutex_lock(&_mutex);
    taskqueue.push(task);
    size_t size = taskqueue.size();
    pthread_cond_signal(&_condition);
    pthread_mutex_unlock(&_mutex);
    return size;
}

void Threadpool_C98::stop() {
    if(!isrunning) {
        return;
    }
    pthread_mutex_lock(&_mutex);
    isrunning = false;
    pthread_mutex_unlock(&_mutex);

    pthread_cond_broadcast(&_condition);

    for(int i = 0; i < thread_num ; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_condition);
}

void *Threadpool_C98::thread_execute(void *arg) {
    // Tip:if not c98, can use auto:c++11 to avoid duplicating the type name;
    Threadpool_C98 *pool = static_cast<Threadpool_C98*>(arg);
    pthread_t tid = pthread_self();
    while(pool->isrunning){
        pthread_mutex_lock(&pool->_mutex);
        while(pool->taskqueue.empty()){
            pthread_cond_wait(&pool->_condition, &pool->_mutex);
        }
        Task *task = pool->taskqueue.front();
        pool->taskqueue.pop();
        pthread_mutex_unlock(&pool->_mutex);
        if(!task) { printf("thread %lu will exit", tid); }
        task->taskrun(); // real execute
    }
}
