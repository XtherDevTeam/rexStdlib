
//
// Created by XIaokang00010 on 2023/1/5.
//

#ifndef REXSTDLIB_TIME_HPP
#define REXSTDLIB_TIME_HPP

#include <rex.hpp>

namespace rexStd::time {
    using namespace rex;

    namespace time {
        nativeFn(format, interpreter, args, passThisPtr);

        value::cxtObject getMethodsCxt(vint time);
    }

    nativeFn(rexTime, interpreter, args, passThisPtr);

    value::cxtObject getMethodsCxt();
}

#endif //REXSTDLIB_TIME_HPP
