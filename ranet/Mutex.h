//
// Created by Jialei Wang on 2019/3/29.
//

#ifndef RANET_MUTEX_H
#define RANET_MUTEX_H

#include <pthread.h>
#include <assert.h>
#include "noncopyable.h"

#if defined(__clang__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE__(x)  __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)
#endif

#define CAPABILITY(x) \
    THREAD_ANNOTATION_ATTTIBUTE__(capability(x))

namespace ranet{
    class  MutexLock : noncopyable
    {
    public:
        MutexLock():holder_(0)
        {

        }
        ~MutexLock(){}
    private:

        friend class Condition;
    };
}
#endif //RANET_MUTEX_H
