//
// Created by Jialei Wang on 2019/4/23.
//

#ifndef RANET_NONCOPYABLE_H
#define RANET_NONCOPYABLE_H

namespace ranet
{
    class Noncopyable{
    public:
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable& operator=(const Noncopyable&) = delete;
    protected:
        Noncopyable() = default;
        ~Noncopyable() = default;
    };
}



#endif //RANET_NONCOPYABLE_H
