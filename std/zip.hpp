//
// Created by XIaokang00010 on 2023/1/8.
//

#ifndef REXSTDLIB_ZIP_HPP
#define REXSTDLIB_ZIP_HPP

#include <rex.hpp>
#include "zip/zip.h"

namespace rexStd::zip {
    using namespace rex;

    struct callbackMsg {
        interpreter *in;
        std::shared_ptr<value> callbackT;
    };

    static int callbackAdaptOnExtract(const char *filename, void *arg);

    static size_t callbackAdaptOnExtractFile(void *arg, uint64_t offset, const void *data, size_t size);

    namespace file {
        namespace entryObject {
            value::cxtObject getMethodsCxt(zip_t *zip);

            nativeFn(close, interpreter, args, _);

            nativeFn(size, interpreter, args, _);

            nativeFn(crc32, interpreter, args, _);

            nativeFn(isDir, interpreter, args, _);

            nativeFn(extract, interpreter, args, _);

            nativeFn(name, interpreter, args, _);

            nativeFn(write, interpreter, args, _);

            nativeFn(read, interpreter, args, _);
        }
        namespace iter {
            value::cxtObject getMethodsCxt(const std::shared_ptr<value> &passThisPtr, vint count);

            nativeFn(next, interpreter, args, _);
        }

        value::cxtObject getMethodsCxt(zip_t *zip);

        nativeFn(entry, interpreter, args, _);

        nativeFn(entries, interpreter, args, _);

        nativeFn(Delete, interpreter, args, _);

        nativeFn(rexIter, interpreter, args, _);
    }

    nativeFn(open, interpreter, args, _);

    nativeFn(extract, interpreter, args, _);

    value::cxtObject getMethodsCxt();
}

#endif //REXSTDLIB_ZIP_HPP
