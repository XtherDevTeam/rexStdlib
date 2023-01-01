//
// Created by XIaokang00010 on 2022/12/30.
//

#ifndef REXSTDLIB_NET_HPP
#define REXSTDLIB_NET_HPP

#include <rex.hpp>

namespace rexStd::net {
    using namespace rex;

    nativeFn(resolve, interpreter, args, passThisPtr);

    nativeFn(rexSocket, interpreter, args, passThisPtr);

    namespace socket {
        nativeFn(connect, interpreter, args, passThisPtr);

        nativeFn(bind, interpreter, args, passThisPtr);

        nativeFn(listen, interpreter, args, passThisPtr);

        nativeFn(accept, interpreter, args, passThisPtr);

        nativeFn(send, interpreter, args, passThisPtr);

        nativeFn(recv, interpreter, args, passThisPtr);

        nativeFn(close, interpreter, args, passThisPtr);

        nativeFn(setSendFlags, interpreter, args, passThisPtr);

        nativeFn(setRecvFlags, interpreter, args, passThisPtr);

        nativeFn(getSendFlags, interpreter, args, passThisPtr);

        nativeFn(getRecvFlags, interpreter, args, passThisPtr);

        value::cxtObject getMethodsCxt(int fd);
    }

    namespace http {
        nativeFn(parseHttpHeader, interpreter, args, passThisPtr);

        value::cxtObject getMethodsCxt();
    }

    value::cxtObject getMethodsCxt();
}

#endif //REXSTDLIB_NET_HPP
