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

    nativeFn(rexSecureSocket, interpreter, args, passThisPtr);

    namespace utils {
        void send(interpreter *in, const managedPtr<value> &socketObject, const vstr &data);

        void send(interpreter *in, const managedPtr<value> &socketObject, const vbytes &data);
    }

    namespace socket {
        nativeFn(connect, interpreter, args, passThisPtr);

        nativeFn(bind, interpreter, args, passThisPtr);

        nativeFn(listen, interpreter, args, passThisPtr);

        nativeFn(accept, interpreter, args, passThisPtr);

        nativeFn(send, interpreter, args, passThisPtr);

        nativeFn(recv, interpreter, args, passThisPtr);

        nativeFn(close, interpreter, args, passThisPtr);

        nativeFn(rexFree, interpreter, args, passThisPtr);

        nativeFn(setSendFlags, interpreter, args, passThisPtr);

        nativeFn(setRecvFlags, interpreter, args, passThisPtr);

        nativeFn(getSendFlags, interpreter, args, passThisPtr);

        nativeFn(getRecvFlags, interpreter, args, passThisPtr);

        value::cxtObject getMethodsCxt(int fd);
    }

    namespace secureSocket {
        nativeFn(connect, interpreter, args, passThisPtr);

        nativeFn(send, interpreter, args, passThisPtr);

        nativeFn(recv, interpreter, args, passThisPtr);

        nativeFn(close, interpreter, args, passThisPtr);

        nativeFn(rexFree, interpreter, args, passThisPtr);

        value::cxtObject getMethodsCxt(int fd);
    }

    namespace http {
        namespace response {
            nativeFn(recvHeaders, interpreter, args, passThisPtr);

            nativeFn(recv, interpreter, args, passThisPtr);

            value::cxtObject getMethodsCxt(const managedPtr<value> &socketObject, value &requestOptArgs);
        }

        namespace sendFormCallableObject {
            nativeFn(rexInvoke, interpreter, args, passThisPtr);

            value::cxtObject getMethodsCxt(value &boundary, value &args);
        }

        nativeFn(parseHttpHeader, interpreter, args, passThisPtr);

        nativeFn(parseUrl, interpreter, args, passThisPtr);

        nativeFn(generateHttpHeader, interpreter, args, passThisPtr);

        nativeFn(open, interpreter, args, passThisPtr);

        nativeFn(sendForm, interpreter, args, passThisPtr);

        value::cxtObject getMethodsCxt();
    }

    value::cxtObject getMethodsCxt();
}

#endif //REXSTDLIB_NET_HPP
