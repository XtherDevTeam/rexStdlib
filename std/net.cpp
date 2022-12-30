//
// Created by XIaokang00010 on 2022/12/30.
//

#include <exceptions/signalException.hpp>
#include "net.hpp"
#include "net/socket.hpp"

namespace rexStd::net {
    nativeFn(resolve, interpreter, args, passThisPtr) {
        const vstr &domain = args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr();
        return {
                string2wstring(libnet::resolve(wstring2string(domain))),
                rex::stringMethods::getMethodsCxt()
        };
    }

    nativeFn(rexSocket, interpreter, args, passThisPtr) {
        try {
            return socket::getMethodsCxt(libnet::socket());
        } catch (const std::runtime_error &error) {
            throw signalException(interpreter::makeErr(L"netError", string2wstring(error.what())));
        }
    }

    value::cxtObject getMethodsCxt() {
        value::cxtObject cxt;
        cxt[L"resolve"] = managePtr(value{value::nativeFuncPtr{resolve}});
        cxt[L"socket"] = managePtr(value{value::nativeFuncPtr{rexSocket}});
        return cxt;
    }

    namespace socket {
        value::cxtObject getMethodsCxt(int fd) {
            value::cxtObject cxt;
            cxt[L"connect"] = managePtr(value{value::nativeFuncPtr{connect}});
            cxt[L"bind"] = managePtr(value{value::nativeFuncPtr{bind}});
            cxt[L"listen"] = managePtr(value{value::nativeFuncPtr{listen}});
            cxt[L"accept"] = managePtr(value{value::nativeFuncPtr{accept}});
            cxt[L"send"] = managePtr(value{value::nativeFuncPtr{send}});
            cxt[L"recv"] = managePtr(value{value::nativeFuncPtr{recv}});
            cxt[L"close"] = managePtr(value{value::nativeFuncPtr{close}});
            cxt[L"setSendFlags"] = managePtr(value{value::nativeFuncPtr{setSendFlags}});
            cxt[L"getSendFlags"] = managePtr(value{value::nativeFuncPtr{getSendFlags}});
            cxt[L"setRecvFlags"] = managePtr(value{value::nativeFuncPtr{setRecvFlags}});
            cxt[L"getRecvFlags"] = managePtr(value{value::nativeFuncPtr{getRecvFlags}});
            cxt[L"__fd__"] = managePtr(value{(vint) fd});
            cxt[L"__sendFlag__"] = managePtr(value{(vint) 0});
            cxt[L"__recvFlag__"] = managePtr(value{(vint) 0});
            return cxt;
        }

        nativeFn(connect, interpreter, args, passThisPtr) {
            vint &socketFd = passThisPtr->members[L"__fd__"]->getInt();
            const vstr &ipAddr = args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr();
            unsigned short port{(unsigned short) (args[1].isRef() ? args[1].getRef().getInt() : args[1].getInt())};
            try {
                libnet::socketConnect((int) socketFd, wstring2string(ipAddr), port);
                return {};
            } catch (const std::runtime_error &error) {
                throw signalException(interpreter::makeErr(L"netError", string2wstring(error.what())));
            }
        }

        nativeFn(bind, interpreter, args, passThisPtr) {
            vint &socketFd = passThisPtr->members[L"__fd__"]->getInt();
            const vstr &ipAddr = args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr();
            unsigned short port{(unsigned short) (args[1].isRef() ? args[1].getRef().getInt() : args[1].getInt())};
            try {
                libnet::socketBind((int) socketFd, wstring2string(ipAddr), port);
                return {};
            } catch (const std::runtime_error &error) {
                throw signalException(interpreter::makeErr(L"netError", string2wstring(error.what())));
            }
        }

        nativeFn(listen, interpreter, args, passThisPtr) {
            vint &socketFd = passThisPtr->members[L"__fd__"]->getInt();
            try {
                if (args.empty()) {
                    libnet::socketListen((int) socketFd);
                } else {
                    vint backlog{args[0].isRef() ? args[0].getRef().getInt() : args[0].getInt()};
                    libnet::socketListen((int) socketFd, (int) backlog);
                }
                return {};
            } catch (const std::runtime_error &error) {
                throw signalException(interpreter::makeErr(L"netError", string2wstring(error.what())));
            }
        }

        nativeFn(accept, interpreter, args, passThisPtr) {
            vint &socketFd = passThisPtr->members[L"__fd__"]->getInt();
            struct sockaddr_in clientAddr{};
            socklen_t clientAddrLen{};
            try {
                return {getMethodsCxt(libnet::socketAccept((int) socketFd, clientAddr, clientAddrLen))};
            } catch (const std::runtime_error &error) {
                throw signalException(interpreter::makeErr(L"netError", string2wstring(error.what())));
            }
        }

        nativeFn(send, interpreter, args, passThisPtr) {
            vint &socketFd = passThisPtr->members[L"__fd__"]->getInt();
            vint &sendFlag = passThisPtr->members[L"__sendFlag__"]->getInt();
            const vbytes &msg = args[0].isRef() ? args[0].getRef().getBytes() : args[0].getBytes();
            try {
                libnet::socketSend((int) socketFd, msg, (int) sendFlag);
                return {};
            } catch (const std::runtime_error &error) {
                throw signalException(interpreter::makeErr(L"netError", string2wstring(error.what())));
            }
        }

        nativeFn(recv, interpreter, args, passThisPtr) {
            vint size{args[0].isRef() ? args[0].getRef().getInt() : args[0].getInt()};
            vint &socketFd = passThisPtr->members[L"__fd__"]->getInt();
            vint &recvFlag = passThisPtr->members[L"__recvFlag__"]->getInt();
            value result{"", rex::bytesMethods::getMethodsCxt()};

            result.getBytes().resize(size);
            try {
                libnet::socketRecv((int) socketFd, result.getBytes(), (int) recvFlag);
                return result;
            } catch (const std::runtime_error &error) {
                throw signalException(interpreter::makeErr(L"netError", string2wstring(error.what())));
            }
        }

        nativeFn(setSendFlags, interpreter, args, passThisPtr) {
            passThisPtr->members[L"__sendFlag__"]->getInt() = args[0].isRef() ? args[0].getRef().getInt() : args[0].getInt();
            return {};
        }

        nativeFn(setRecvFlags, interpreter, args, passThisPtr) {
            passThisPtr->members[L"__recvFlag__"]->getInt() = args[0].isRef() ? args[0].getRef().getInt() : args[0].getInt();
            return {};
        }

        nativeFn(getSendFlags, interpreter, args, passThisPtr) {
            return passThisPtr->members[L"__sendFlag__"]->getInt();
        }

        nativeFn(getRecvFlags, interpreter, args, passThisPtr) {
            return passThisPtr->members[L"__recvFlag__"]->getInt();
        }

        nativeFn(close, interpreter, args, passThisPtr) {
            vint &socketFd = passThisPtr->members[L"__fd__"]->getInt();
            try {
                libnet::socketClose((int) socketFd);
                return {};
            } catch (const std::runtime_error &error) {
                throw signalException(interpreter::makeErr(L"netError", string2wstring(error.what())));
            }
        }
    }
}