//
// Created by Jialei Wang on 2019/3/29.
//

#ifndef RANET_NONCOPYABLE_H
#define RANET_NONCOPYABLE_H

namespace ranet{
    class noncopyable{
    public:
        noncopyable(const noncopyable&) = delete;
        void operator=(const noncopyable&) = delete;
    protected:
        noncopyable() = default;
        ~noncopyable() = default;
    };
}

#endif //RANET_NONCOPYABLE_H
