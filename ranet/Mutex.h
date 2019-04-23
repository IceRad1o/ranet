//
// Created by Jialei Wang on 2019/4/23.
//

#ifndef RANET_MUTEX_H
#define RANET_MUTEX_H

#include <pthread.h>
#include "Noncopyable.h"

namespace ranet
{

class MutexLock: public Noncopyable
{
public:
    MutexLock()
    {
        pthread_mutex_init(&mutex, NULL);
    }
    ~MutexLock(){
        pthread_mutex_destroy(&mutex);
    }
    void lock(){
        pthread_mutex_lock(&mutex);
    }
    void unlock(){
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_t *getPthreadMutex(){
        return &mutex;
    }
private:
    pthread_mutex_t mutex;
};

class MutexLockGuard:public Noncopyable
{
public:
    MutexLockGuard(MutexLock& mutexlock): _mutexlock(mutexlock)
    {
        _mutexlock.lock();
    }
    ~MutexLockGuard(){
        _mutexlock.unlock();
    }
private:
    MutexLock& _mutexlock;
};

} //namespace ranet
#endif //RANET_MUTEX_H
