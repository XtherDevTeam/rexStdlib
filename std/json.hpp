//
// Created by XIaokang00010 on 2023/1/1.
//

#ifndef REXSTDLIB_JSON_HPP
#define REXSTDLIB_JSON_HPP

#include <rex.hpp>

namespace rexStd::json {
    using namespace rex;

    vsize loadObjectToJson(const vstr &src, value& dst, vsize pos);

    void dumpObjectToJson(rex::interpreter &in, value &v, vstr &dst);

    nativeFn(loads, interpreter, args, passThisPtr);

    nativeFn(dumps, interpreter, args, passThisPtr);

    value::cxtObject getMethodsCxt();
}

#endif //REXSTDLIB_JSON_HPP
