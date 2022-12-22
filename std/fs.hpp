//
// Created by XIaokang00010 on 2022/12/22.
//

#ifndef REXSTDLIB_FS_HPP
#define REXSTDLIB_FS_HPP

#include <rex.hpp>

namespace rexStd::fs {
    using namespace rex;

    nativeFn(open, interpreter, args, _);

    namespace file {
        nativeFn(read, interpreter, args, passThisPtr);

        nativeFn(write, interpreter, args, passThisPtr);

        nativeFn(eof, interpreter, args, passThisPtr);

        nativeFn(tell, interpreter, args, passThisPtr);

        nativeFn(seek, interpreter, args, passThisPtr);

        nativeFn(close, interpreter, args, _);

        value::cxtObject getMethodsCxt();

        value constructFileObject(FILE* fp = nullptr);
    }

    nativeFn(access, interpreter, args, _);

    value::cxtObject getMethodsCxt();
}

#endif //REXSTDLIB_FS_HPP
