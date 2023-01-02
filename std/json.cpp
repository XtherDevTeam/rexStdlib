//
// Created by XIaokang00010 on 2023/1/1.
//

#include "json.hpp"
#include <exceptions/signalException.hpp>

namespace rexStd::json {
    nativeFn(loads, interpreter, args, passThisPtr) {
        value dst{};
        loadObjectToJson(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr(), dst, 0);
        return dst;
    }

    nativeFn(dumps, interpreter, args, passThisPtr) {
        value val{L"", rex::stringMethods::getMethodsCxt()};
        dumpObjectToJson(*static_cast<rex::interpreter *>(interpreter), args[0].isRef() ? args[0].getRef() : args[0],
                         val.getStr());
        return val;
    }

    value::cxtObject getMethodsCxt() {
        value::cxtObject cxt;
        cxt[L"loads"] = managePtr(value{value::nativeFuncPtr{loads}});
        cxt[L"dumps"] = managePtr(value{value::nativeFuncPtr{dumps}});

        return cxt;
    }

    void dumpObjectToJson(rex::interpreter &in, value &v, vstr &dst) {
        switch (v.kind) {
            case value::vKind::vNull:
            case value::vKind::vInt:
            case value::vKind::vDeci:
            case value::vKind::vBool:
            case value::vKind::vStr:
            case value::vKind::vBytes:
            case value::vKind::vVec:
            case value::vKind::vRef:
                dst += v;
                break;
            case value::vKind::vObject:
                if (auto it = v.members.find(L"rexStr"); it != v.members.end()) {
                    dst += in.invokeFunc(it->second, {}, managePtr(v));
                } else {
                    dst += v;
                }
                break;
            default:
                throw signalException(interpreter::makeErr(L"jsonError", L"unserializable values"));
        }
    }

    vsize loadObjectToJson(const vstr &src, value &dst, vsize pos) {
        while (src[pos] and src[pos] < '!') pos++;
        switch (src[pos]) {
            case '{': {
                pos++;
                dst = {value::cxtObject{}};
                while (true) {
                    value left{}, right{};
                    pos = loadObjectToJson(src, left, pos);
                    if (left.kind != rex::value::vKind::vStr)
                        throw signalException(interpreter::makeErr(L"jsonError", L"expected vStr as left value"));

                    while (src[pos] and src[pos] < '!') pos++;
                    if (src[pos] != ':')
                        throw signalException(interpreter::makeErr(L"jsonError", L"expected `:` after left value"));
                    pos++;
                    while (src[pos] < '!') pos++;

                    pos = loadObjectToJson(src, right, pos);
                    while (src[pos] and src[pos] < '!') pos++;

                    if (src[pos] == ',') {
                        pos++;
                    }
                    // end the parsing of a pair
                    dst.members[left.getStr()] = managePtr(right);
                    if (src[pos] == '}') {
                        pos++;
                        break;
                    }
                }
                break;
            }
            case '[': {
                pos++;
                dst = {value::vecObject{}, rex::vecMethods::getMethodsCxt()};
                while (true) {
                    managedPtr<value> temp = managePtr(value{});
                    pos = loadObjectToJson(src, *temp, pos);
                    dst.getVec().push_back(temp);
                    while (src[pos] and src[pos] < '!') pos++;
                    if (src[pos] == ',') {
                        pos++;
                    }
                    if (src[pos] == ']') {
                        pos++;
                        break;
                    }
                }
                break;
            }
            case '"': {
                // string
                dst = {L"", rex::stringMethods::getMethodsCxt()};
                pos++;
                while (src[pos] and src[pos] != '"') {
                    if (src[pos] == '\\') {
                        pos++;
                        dst.getStr() += '\\';
                    }
                    dst.getStr() += src[pos];
                    pos++;
                }
                pos++;
                std::wistringstream ss{dst.getStr()};
                dst.getStr().clear();
                parseString(ss, dst.getStr());
                break;
            }
            case '0'...'9': {
                // integer
                bool isDeci = false;
                vstr tempStr;
                tempStr += src[pos];
                pos++;
                while (isdigit(src[pos])) {
                    tempStr += src[pos];
                    pos++;
                }
                if (src[pos] == '.') {
                    isDeci = true;
                    tempStr += src[pos];
                    pos++;
                    while (isdigit(src[pos])) {
                        tempStr += src[pos];
                        pos++;
                    }
                }
                dst = isDeci ? value{(vdeci)std::stod(tempStr)} : value{(vint)std::stoll(tempStr)};
                break;
            }
            case 't': {
                vsize cur = pos;
                while (isalpha(src[pos]))
                    pos++;
                if (src.substr(cur, pos - cur) == L"true") {
                    dst = true;
                } else {
                    throw signalException(interpreter::makeErr(L"jsonError", L"invalid values"));
                }
                break;
            }
            case 'f': {
                vsize cur = pos;
                while (isalpha(src[pos]))
                    pos++;
                if (src.substr(cur, pos - cur) == L"false") {
                    dst = false;
                } else {
                    throw signalException(interpreter::makeErr(L"jsonError", L"invalid values"));
                }
                break;
            }
            default: {
                throw signalException(interpreter::makeErr(L"jsonError", L"invalid values"));
            }
        }
        return pos;
    }
}