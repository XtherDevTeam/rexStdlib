//
// Created by XIaokang00010 on 2023/1/1.
//

#include "json.hpp"
#include <exceptions/signalException.hpp>

namespace rexStd::json {
    nativeFn(loads, interpreter, args, passThisPtr) {
        return {};
    }

    nativeFn(dumps, interpreter, args, passThisPtr) {
        value val{L"", rex::stringMethods::getMethodsCxt()};
        dumpObjectToJson(*static_cast<rex::interpreter *>(interpreter), args[0].isRef() ? args[0].getRef() : args[0], val.getStr());
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
}