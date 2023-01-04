//
// Created by XIaokang00010 on 2022/12/30.
//

#include <exceptions/signalException.hpp>
#include "net.hpp"
#include "net/socket.hpp"
#include "json.hpp"
#include "fs.hpp"

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
            value result{value::cxtObject{}};
            result.members[L"sections"] = managePtr(value{value::cxtObject{}});

            vstr &headers = args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr();
            split(headers, vstr{L"\r\n"}, [&](const vstr &line, vsize index) {
                if (index) {
                    if (line.empty()) return;
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
                        try {
                            // response
                            result.members[L"version"] = managePtr(
                                    value{line.substr(5, 3), rex::stringMethods::getMethodsCxt()});
                            result.members[L"statusCode"] = managePtr(value{(vint) std::stol(line.substr(9, 3))});
                            result.members[L"statusText"] = managePtr(
                                    value{line.substr(13), rex::stringMethods::getMethodsCxt()});
                            result.members[L"type"] = managePtr(
                                    value{L"response", rex::stringMethods::getMethodsCxt()});
                        } catch (...) {
                            throw signalException(interpreter::makeErr(L"httpError", L"invalid first line"));
                        }
                    } else {
                        try {
                            // request
                            vec<vstr> res;
                            split(line, vstr{L" "}, res);
                            result.members[L"method"] = managePtr(value{res[0], rex::stringMethods::getMethodsCxt()});
                            result.members[L"target"] = managePtr(value{res[1], rex::stringMethods::getMethodsCxt()});
                            result.members[L"version"] = managePtr(
                                    value{res[2].substr(5), rex::stringMethods::getMethodsCxt()});
                            result.members[L"type"] = managePtr(value{L"request", rex::stringMethods::getMethodsCxt()});
                        } catch (...) {
                            throw signalException(interpreter::makeErr(L"httpError", L"invalid first line"));
                        }
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
                        host = host.substr(0, vit);
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

        nativeFn(generateHttpHeader, interpreter, args, passThisPtr) {
            value headerObject = args[0].isRef() ? args[0].getRef() : args[0];
            value result{L"", rex::stringMethods::getMethodsCxt()};
            if (headerObject[L"type"]->getStr() == L"response") {
                result.getStr() += L"HTTP/" + headerObject[L"version"]->getStr() + L" " +
                                   std::to_wstring(headerObject[L"statusCode"]->getInt()) + L" " +
                                   headerObject[L"statusText"]->getStr() + L"\r\n";
            } else if (headerObject[L"type"]->getStr() == L"request") {
                result.getStr() +=
                        headerObject[L"method"]->getStr() + L" " + headerObject[L"target"]->getStr() + L" HTTP/" +
                        headerObject[L"version"]->getStr() + L"\r\n";
            } else {
                throw signalException(interpreter::makeErr(L"httpError", L"invalid http headers object"));
            }

            for (auto &section: headerObject[L"sections"]->members) {
                result.getStr() += section.first + L" " + section.second->getStr() + L"\r\n";
            }
            result.getStr() += L"\r\n";
            return result;
        }

        nativeFn(open, interpreter, args, passThisPtr) {
            auto in = static_cast<rex::interpreter *>(interpreter);

            managedPtr<value> socketObject = managePtr(rexSocket(interpreter, {}, {}));

            vstr method = args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr();
            value parsedUrl = parseUrl(interpreter, {args[1].isRef() ? args[1].getRef() : args[1]}, passThisPtr);
            value optionalArgs{value::cxtObject{}};
            if (args.size() == 3) {
                optionalArgs = args[2].isRef() ? args[2].getRef() : args[2];
            }

            // make socket connection
            // resolve ip
            value ipAddr = resolve(interpreter, {*parsedUrl[L"host"]}, {});
            if (parsedUrl[L"protocol"]->getStr() == L"http") {
                // check and patch the default port in http connection
                if (parsedUrl[L"port"]->kind == rex::value::vKind::vNull)
                    *parsedUrl[L"port"] = vint{80};

                in->invokeFunc(socketObject->members[L"connect"], {ipAddr, *parsedUrl[L"port"]}, socketObject);
            } else {
                // not implemented: https
                throw signalException(
                        interpreter::makeErr(L"httpError", L"Not implemented: 哪个带善人帮忙做一下https啊求求了😭😭😭"));
            }

            // check and patch the present arguments
            if (auto it = optionalArgs.members.find(L"stream"); it != optionalArgs.members.end()) {
                if (it->second->getBool()) {
                    // check and patch the chunkSize
                    if (!optionalArgs.members.contains(L"chunkSize")) {
                        optionalArgs.members[L"chunkSize"] = managePtr(value{(vint) 1048576});
                    }
                }
            } else {
                optionalArgs.members[L"stream"] = managePtr(value{false});
            }
            // patch the `body` content, and get the `Content-Length`, covert them to `vBytes` if they can
            if (!optionalArgs.members.contains(L"sections"))
                optionalArgs.members[L"sections"] = managePtr(value{value::cxtObject{}});

            if (!optionalArgs.members[L"sections"]->members.contains(L"User-Agent"))
                optionalArgs.members[L"sections"]->members[L"User-Agent"] = managePtr(
                        value{L"reXscriptLibhttp;JerryChou;", rex::stringMethods::getMethodsCxt()});

            if (!optionalArgs.members[L"sections"]->members.contains(L"Host"))
                optionalArgs.members[L"sections"]->members[L"Host"] = managePtr(*parsedUrl[L"host"]);

            if (!optionalArgs.members.contains(L"body"))
                // if not exist, make it exists.
                optionalArgs.members[L"body"] = managePtr(value{});
            if (!optionalArgs.members.contains(L"chunkSize"))
                // if not exist, make it exists.
                optionalArgs.members[L"chunkSize"] = managePtr(value{(vint) 1048576});

            switch (optionalArgs[L"body"]->kind) {
                case rex::value::vKind::vNull: {
                    break;
                }
                case rex::value::vKind::vBytes: {
                    optionalArgs[L"sections"]->members[L"Content-Length"] =
                            managePtr(value{std::to_wstring(optionalArgs[L"body"]->getBytes().size()),
                                            rex::stringMethods::getMethodsCxt()});
                    break;
                }
                case rex::value::vKind::vStr: {
                    // covert to bytes
                    optionalArgs[L"body"] = managePtr(
                            value{wstring2string(optionalArgs[L"body"]->getStr()), bytesMethods::getMethodsCxt()});
                    optionalArgs[L"sections"]->members[L"Content-Length"] =
                            managePtr(value{std::to_wstring(optionalArgs[L"body"]->getBytes().size()),
                                            rex::stringMethods::getMethodsCxt()});
                    break;
                }
                case rex::value::vKind::vObject: {
                    // check if this is a file
                    if (optionalArgs[L"body"]->members.contains(L"rexFile")) {
                        // no need to covert to vBytes
                        optionalArgs[L"sections"]->members[L"Content-Length"] =
                                managePtr(value{std::to_wstring(optionalArgs[L"body"]->members[L"length"]->getInt()),
                                                rex::stringMethods::getMethodsCxt()});
                        break;
                    }
                    // no need to create an else branch, just let it fallback to default
                }
                default: {
                    vstr dst;
                    json::dumpObjectToJson(*static_cast<rex::interpreter *>(interpreter), *optionalArgs[L"body"], dst);
                    optionalArgs[L"body"] = managePtr(value{wstring2string(dst), rex::bytesMethods::getMethodsCxt()});
                    optionalArgs[L"sections"]->members[L"Content-Length"] =
                            managePtr(value{std::to_wstring(optionalArgs[L"body"]->getBytes().size()),
                                            rex::stringMethods::getMethodsCxt()});
                }
            }

            // make up the request raw bytes
            // first line
            in->invokeFunc(socketObject->members[L"send"], {value{
                    wstring2string(method) + " " + wstring2string(parsedUrl[L"target"]->getStr()) + " HTTP/1.1\r\n",
                    rex::bytesMethods::getMethodsCxt()}}, socketObject);
            // sections
            for (auto &section: optionalArgs[L"sections"]->members) {
                in->invokeFunc(socketObject->members[L"send"], {value{
                        wstring2string(section.first) + ": " + wstring2string(section.second->getStr()) + "\r\n",
                        rex::bytesMethods::getMethodsCxt()}}, socketObject);
            }
            in->invokeFunc(socketObject->members[L"send"], {value{"\r\n", rex::bytesMethods::getMethodsCxt()}},
                           socketObject);

            // make up the request body if present
            switch (optionalArgs[L"body"]->kind) {
                case rex::value::vKind::vBytes: {
                    // bytes
                    in->invokeFunc(socketObject->members[L"send"], {*optionalArgs[L"body"]}, socketObject);
                    break;
                }
                case rex::value::vKind::vObject: {
                    // file
                    vint remainSize = optionalArgs[L"body"]->members[L"length"]->getInt();
                    while (remainSize > 0) {
                        vint chunkSize = remainSize > 1048576 ? 1048576 : remainSize;

                        in->invokeFunc(socketObject->members[L"send"],
                                       {in->invokeFunc(optionalArgs[L"body"]->members[L"recv"], {chunkSize},
                                                       optionalArgs[L"body"])}, socketObject);
                        remainSize -= chunkSize;
                    }
                    break;
                }
                default: {
                    break;
                }
            }

            value result{response::getMethodsCxt(socketObject, optionalArgs)};
            in->invokeFunc(result.members[L"recvHeaders"], {}, managePtr(result));
            return result;
        }

        value::cxtObject getMethodsCxt() {
            value::cxtObject result;
            result[L"parseHttpHeader"] = managePtr(value{value::nativeFuncPtr{parseHttpHeader}});
            result[L"parseUrl"] = managePtr(value{value::nativeFuncPtr{parseUrl}});
            result[L"generateHttpHeader"] = managePtr(value{value::nativeFuncPtr{generateHttpHeader}});
            result[L"open"] = managePtr(value{value::nativeFuncPtr{open}});
            return result;
        }

        namespace response {
            value::cxtObject getMethodsCxt(const managedPtr<value> &socketObject, value &requestOptArgs) {
                value::cxtObject result;
                result[L"recv"] = managePtr(value{value::nativeFuncPtr{recv}});
                result[L"recvHeaders"] = managePtr(value{value::nativeFuncPtr{recvHeaders}});

                result[L"body"] = managePtr(value{"", bytesMethods::getMethodsCxt()});
                result[L"headers"] = managePtr(value{});
                result[L"__socket__"] = socketObject;
                result[L"__optionalArgs__"] = managePtr(requestOptArgs);
                return result;
            }

            nativeFn(recvHeaders, interpreter, args, passThisPtr) {
                auto in = static_cast<rex::interpreter *>(interpreter);

                auto &socketObject = passThisPtr->members[L"__socket__"];
                vbytes result;
                while (true) {
                    result += in->invokeFunc(socketObject->members[L"recv"], {(vint) 1},
                                             socketObject).getBytes();
                    if (result.ends_with("\r\n\r\n"))
                        break;
                }
                passThisPtr->members[L"headers"] = managePtr(parseHttpHeader(
                        interpreter, {{string2wstring(result), rex::stringMethods::getMethodsCxt()}}, {}));
                return passThisPtr;
            }

            nativeFn(recv, interpreter, args, passThisPtr) {
                auto in = static_cast<rex::interpreter *>(interpreter);

                auto &socketObject = passThisPtr->members[L"__socket__"];
                auto &sections = passThisPtr->members[L"headers"]->members[L"sections"];

                if (auto it = sections->members.find(L"Transfer-Encoding");
                        it != sections->members.end() and it->second->getStr().find(L"chunked") != vstr::npos) {
                    // 我他妈不想适配这个啊操 日你妈傻逼 chunked transfer encoding
                    while (true) {
                        vbytes result;
                        while (true) {
                            result += in->invokeFunc(socketObject->members[L"recv"], {(vint) 1},
                                                     socketObject).getBytes();
                            if (result.ends_with("\r\n"))
                                break;
                        }
                        vint chunkSize = std::stoll(result, nullptr, 16);
                        if (chunkSize == 0) {
                            // skip the CRLF
                            in->invokeFunc(
                                    socketObject->members[L"recv"], {(vint) 2}, socketObject).getBytes();
                            break;
                        }
                        while (chunkSize > 0) {
                            auto &&chunk =
                                    in->invokeFunc(socketObject->members[L"recv"], {chunkSize}, socketObject);
                            if (args.empty()) {
                                passThisPtr->members[L"body"]->getBytes() += chunk.getBytes();
                            } else {
                                auto ptrToFunc = args[0].isRef() ? args[0].refObj : managePtr(args[0]);
                                in->invokeFunc(ptrToFunc, {chunk}, {});
                            }
                            chunkSize -= (vint) chunk.getBytes().size();
                        }
                        in->invokeFunc(
                                socketObject->members[L"recv"], {(vint) 2}, socketObject).getBytes(); // skip the CRLF
                    }
                    return passThisPtr;
                } else if (it = sections->members.find(L"Content-Length"); it != sections->members.end()) {
                    vint remainSize = std::stoll(it->second->getStr());
                    vint chunkSize = passThisPtr->members[L"__optionalArgs__"]->members[L"chunkSize"]->getInt();

                    if (args.empty()) {
                        // no args means receive all and put them to response.body

                        while (remainSize > 0) {
                            vint readSize = remainSize > chunkSize ? chunkSize : remainSize;
                            auto &&buf = in->invokeFunc(socketObject->members[L"recv"], {readSize}, socketObject);
                            passThisPtr->members[L"body"]->getBytes() += buf.getBytes();
                            remainSize -= (vint) buf.getBytes().size();
                        }
                    } else {
                        auto ptrToFunc = args[0].isRef() ? args[0].refObj : managePtr(args[0]);
                        while (remainSize > 0) {
                            vint readSize = remainSize > chunkSize ? chunkSize : remainSize;
                            auto &&buf = in->invokeFunc(socketObject->members[L"recv"], {readSize}, socketObject);
                            in->invokeFunc(ptrToFunc, {buf}, {});
                            remainSize -= (vint) buf.getBytes().size();
                        }
                    }
                    return passThisPtr;
                } else {
                    // you don't need to receive the fucking data because there's no data in the body to receive unless they are chunked. whatever, I don't give a fuck.
                    return passThisPtr;
                }
            }
        }
    }
}