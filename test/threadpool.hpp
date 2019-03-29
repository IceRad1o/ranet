#include <queue>
#include <vector>
#include <utility>


class ThreadPool{
public:
    ThreadPool(size_t _worker_num);
    void add(void (*func)(void*), void *arg);
private:
    size_t worker_num;
    pthread_mutex_t lock;
    pthread_cond_t notify;
    std::vector<pthread_t> threads;
    std::queue<std::pair<void (*)(void*),void*>> queue;
    friend void *thread_pool_work(void *pool);
};