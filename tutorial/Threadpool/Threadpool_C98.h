//
// Created by Jialei Wang on 2019/4/24.
//

/*This is a threadpool implemented by c98 oop
   * 只是一个线程池的overview 没有对POSIX api写wrapper函数来处理错误
   * 也没有对thread mutex condition 都进行RAII封装
   */
#ifndef CPLUS2_TEST_THREADPOOL_H
#define CPLUS2_TEST_THREADPOOL_H

#include <pthread.h>
#include <vector>
#include <queue>
#include <string>

class Task
{
public:
    Task() = default;
    Task(void *arg, const std::string taskname):arg_(arg),taskName_(taskname) { }
    virtual void taskrun() = 0;
    void setArg(void *arg)
    {
        arg_ = arg;
    }
    virtual ~Task() = default;

protected:
    void *arg_;
    std::string taskName_;
};


class Threadpool_C98
{
public:
    Threadpool_C98(size_t nums);
    ~Threadpool_C98();
    size_t add_task(Task *task);
    void stop();
    size_t size();
private:
    int createthreads();
    static void *thread_execute(void *arg);
private:
    bool isrunning;
    size_t thread_num;
    pthread_mutex_t _mutex;
    pthread_cond_t _condition;
    std::vector<pthread_t> threads;
    std::queue<Task*> taskqueue;
};
/*
 *一个小问题在于，线程去取task队列里取得任务来执行，那么线程池终止的时候，什么样是正确的操作？
 *在临界区保护的前提下，把running设置成false。在stop函数里对所有对线程join，即等待所有线程操作完成
 */
#endif //CPLUS2_TEST_THREADPOOL_H
