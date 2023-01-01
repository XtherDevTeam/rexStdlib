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
        cxt[L"http"] = managePtr(value{http::getMethodsCxt()});
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
            passThisPtr->members[L"__sendFlag__"]->getInt() = args[0].isRef() ? args[0].getRef().getInt()
                                                                              : args[0].getInt();
            return {};
        }

        nativeFn(setRecvFlags, interpreter, args, passThisPtr) {
            passThisPtr->members[L"__recvFlag__"]->getInt() = args[0].isRef() ? args[0].getRef().getInt()
                                                                              : args[0].getInt();
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

    namespace http {
        nativeFn(parseHttpHeader, interpreter, args, passThisPtr) {
            value result{};
            result.members[L"sections"] = managePtr(value{value::cxtObject{}});

            vstr &headers = args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr();
            split(headers, vstr{L"\r\n"}, [&](const vstr &line, vsize index) {
                if (index) {
                    if (auto it = line.find(L": "); it != vstr::npos) {
                        vstr first = line.substr(0, it);
                        vstr second = line.substr(it + 2);
                        result.members[L"sections"]->members[first] = managePtr(
                                value{second, rex::stringMethods::getMethodsCxt()});
                    } else {
                        throw signalException(interpreter::makeErr(L"httpError", L"invalid headers"));
                    }
                } else {
                    // parse the first line
                    if (line.starts_with(L"HTTP/")) {
                        // response
                        result.members[L"version"] = managePtr(
                                value{line.substr(5, 3), rex::stringMethods::getMethodsCxt()});
                        result.members[L"statusCode"] = managePtr(value{(vint) std::stol(line.substr(9, 3))});
                        result.members[L"statusText"] = managePtr(value{(vint) std::stol(line.substr(13))});
                        result.members[L"type"] = managePtr(value{L"response", rex::stringMethods::getMethodsCxt()});
                    } else {
                        // request
                        vec<vstr> res;
                        split(line, vstr{L" "}, res);
                        result.members[L"method"] = managePtr(value{res[0], rex::stringMethods::getMethodsCxt()});
                        result.members[L"target"] = managePtr(value{res[1], rex::stringMethods::getMethodsCxt()});
                        result.members[L"version"] = managePtr(
                                value{res[2].substr(5), rex::stringMethods::getMethodsCxt()});
                        result.members[L"type"] = managePtr(value{L"request", rex::stringMethods::getMethodsCxt()});
                    }
                }
            });
            return result;
        }

        nativeFn(parseUrl, interpreter, args, passThisPtr) {
            vstr url = args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr();
            if (auto it = url.find(L"://"); it != vstr::npos) {
                vstr protocol = url.substr(0, it);
                vstr host = url.substr(it + 3);
                vstr target{L"/"};
                managedPtr<value> port;
                if (auto vit = host.find(L'/'); vit != vstr::npos) {
                    target = host.substr(vit);
                    host = host.substr(0, vit);
                }
                if (auto vit = host.find(L':'); vit != vstr::npos) {
                    try {
                        port = managePtr(value{(vint) (std::stol(host.substr(vit + 1)))});
                        if (port->getInt() > 65535)
                            throw signalException(interpreter::makeErr(L"httpError", L"invalid port number"));
                    } catch (const std::invalid_argument &ex) {
                        throw signalException(interpreter::makeErr(L"httpError", L"invalid port number"));
                    }
                } else {
                    port = managePtr(value{});
                }
                return {value::cxtObject{
                        {L"protocol", managePtr(value{protocol, rex::stringMethods::getMethodsCxt()})},
                        {L"host",     managePtr(value{host, rex::stringMethods::getMethodsCxt()})},
                        {L"target",   managePtr(value{target, rex::stringMethods::getMethodsCxt()})},
                        {L"port",     port}
                }};
            } else {
                throw signalException(interpreter::makeErr(L"httpError", L"invalid url"));
            }
        }

        value::cxtObject getMethodsCxt() {
            value::cxtObject result;
            result[L"parseHttpHeader"] = managePtr(value{value::nativeFuncPtr{parseHttpHeader}});
            result[L"parseUrl"] = managePtr(value{value::nativeFuncPtr{parseUrl}});
            return result;
        }
    }
}