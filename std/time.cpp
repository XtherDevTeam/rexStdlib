//
// Created by XIaokang00010 on 2023/1/5.
//

#include <ctime>
#include "time.hpp"

namespace rexStd::time {
    namespace time {
        value::cxtObject getMethodsCxt(vint time) {
            auto timestamp = (time_t) time;
            tm t = *std::localtime((const time_t *) (&timestamp));

            value::cxtObject result;
            result[L"format"] = managePtr(value{value::nativeFuncPtr{format}});
            result[L"year"] = managePtr(value{(vint) t.tm_year + 1900});
            result[L"month"] = managePtr(value{(vint) t.tm_mon + 1});
            result[L"day"] = managePtr(value{(vint) t.tm_mday});
            result[L"hour"] = managePtr(value{(vint) t.tm_hour});
            result[L"minute"] = managePtr(value{(vint) t.tm_min});
            result[L"second"] = managePtr(value{(vint) t.tm_sec});

            result[L"timestamp"] = managePtr(value{time});
            return result;
        }

        nativeFn(format, interpreter, args, passThisPtr) {
            auto timestamp = (time_t) passThisPtr->members[L"timestamp"]->getInt();
            tm t = *localtime((const time_t *) &timestamp);
            auto &&format =
                    wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr());
            vbytes result(64, '\0');
            std::strftime(result.data(), 64, format.c_str(), &t);
            return {string2wstring(result.data()), rex::stringMethods::getMethodsCxt()};
        }
    }

    value::cxtObject getMethodsCxt() {
        value::cxtObject result;
        result[L"time"] = managePtr(value{value::nativeFuncPtr{rexTime}});
        return result;
    }

    nativeFn(rexTime, interpreter, args, passThisPtr) {
        if (args.empty()) {
            return time::getMethodsCxt(std::time(nullptr));
        } else {
            return time::getMethodsCxt(args[0].getInt());
        }
    }
}
