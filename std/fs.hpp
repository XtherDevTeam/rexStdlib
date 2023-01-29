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

        nativeFn(rexFree, interpreter, args, _);

        value::cxtObject getMethodsCxt();

        value constructFileObject(FILE* fp = nullptr);
    }

    nativeFn(access, interpreter, args, _);

    nativeFn(stat, interpreter, args, _);

    nativeFn(unlink, interpreter, args, _);

    nativeFn(mkdir, interpreter, args, _);

    nativeFn(mkdirs, interpreter, args, _);

    nativeFn(remove, interpreter, args, _);

    nativeFn(removeAll, interpreter, args, _);

    nativeFn(listDir, interpreter, args, _);

    nativeFn(realpath, interpreter, args, _);

    nativeFn(getFileDir, interpreter, args, _);

    nativeFn(copy, interpreter, args, _);

    nativeFn(temp, interpreter, args, _);

    value::cxtObject getMethodsCxt();
}

#endif //REXSTDLIB_FS_HPP
