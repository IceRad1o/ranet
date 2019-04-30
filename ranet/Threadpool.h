//
// Created by Jialei Wang on 2019/4/23.
//

#ifndef RANET_THREADPOOL_H
#define RANET_THREADPOOL_H

#include <pthread.h>
#include <vector>
#include <queue>
namespace ranet{
    class Threadpool
    {
    public:

    private:
        std::vector<pthread_t> workers;
    };
}
#endif //RANET_THREADPOOL_H
