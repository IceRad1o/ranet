//
// Created by Jialei Wang on 2019/3/29.
//

#ifndef RANET_TYPES_H
#define RANET_TYPES_H

#include <stdint.h>
#include <string.h>
#include <string>

#ifndef NDEBUG
#include <assert.h>
#endif

namespace ranet{
    using std::string;
    inline void memZero(void *p, size_t n)
    {
        memset(p, 0, n);
    }

    template <typename To, typename From>
    inline To implicit_cast(From const &f)
    {
        return f;
    }

    template <typename To, typename From>
    inline To down_cast(From *f)
    {
        if(false){
            implicit_cast<From*,0>(0);
        }
#if !defined(NDEBUG) && !defined(GOOGLE_PROtoBUF_NO_RTTI)
        // TOBEDONE
        assert(f == NULL);
#endif
        return static_cast<To>(f);
    }

}


#endif //RANET_TYPES_H
