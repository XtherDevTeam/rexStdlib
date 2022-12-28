//
// Created by XIaokang00010 on 2022/12/28.
//

#include <exceptions/signalException.hpp>
#include <cstring>
#include "sqlite.hpp"

namespace rexStd::sqlite {
    nativeFn(open, interpreter, args, passThisPtr) {
        sqlite3 *db;
        vbytes uri = wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr());
        if (sqlite3_open(uri.data(), &db)) {
            throw signalException(interpreter::makeErr(
                    L"sqliteError", L"Unable to open database: " + string2wstring(sqlite3_errmsg(db))));
        } else {
            return {database::getMethodsCxt(db)};
        }
    }

    value::cxtObject getMethodsCxt() {
        value::cxtObject result;
        result[L"open"] = managePtr(value{(value::nativeFuncPtr) open});
        return result;
    }

    int callback(void *cbMsg, int argc, char **argv, char **azColName) {
        if (cbMsg) {
            auto taskId = (vint) cbMsg;
            callbackTasks[taskId].received = {value::vecObject{}, rex::vecMethods::getMethodsCxt()};
            callbackTasks[taskId].received.getVec().push_back(managePtr(value{value::cxtObject{}}));
            auto line = callbackTasks[taskId].received.getVec().back();
            // parse this callback
            for (int i = 0; i < argc; i++) {
                auto &toInsert = line->members[string2wstring(azColName[i])];
                char *rhs = argv[i];
                int type = sqlite3_column_type(callbackTasks[taskId].stmt, i);
                // covert sqlite data structures to rex::value
                switch (type) {
                    case SQLITE_INTEGER: {
                        toInsert = managePtr(value{(vint) atoi(rhs)});
                        break;
                    }
                    case SQLITE_FLOAT: {
                        toInsert = managePtr(value{(vdeci) atof(rhs)});
                        break;
                    }
                    case SQLITE_TEXT: {
                        // covert to vbytes object directly, user can use `val.decode('charset')` to decode it.
                        toInsert = managePtr(value{rhs, rex::bytesMethods::getMethodsCxt()});
                        break;
                    }
                    case SQLITE_BLOB: {
                        int len = sqlite3_column_bytes(callbackTasks[taskId].stmt, i);
                        toInsert = managePtr(value{vbytes(), rex::bytesMethods::getMethodsCxt()});
                        toInsert->getBytes().resize(len);
                        memcpy(toInsert->getBytes().data(), rhs, len);
                        break;
                    }
                    case SQLITE_NULL:
                    default: {
                        // make clang-tidy happy :-)
                        toInsert = managePtr(value{});
                        break;
                    }
                }
            }
            return 0;
        } else {
            return 0;
        }
    }

    namespace database {
        nativeFn(close, interpreter, args, passThisPtr) {
            auto db = (sqlite3 *) passThisPtr->members[L"__database__"]->basicValue.unknown;
            if (sqlite3_close(db)) {
                throw signalException(interpreter::makeErr(
                        L"sqliteError", L"Unable to close database: " + string2wstring(sqlite3_errmsg(db))));
            } else {
                return {};
            }
        }

        nativeFn(executeScript, interpreter, args, passThisPtr) {
            auto in = static_cast<rex::interpreter *>(interpreter);
            auto db = (sqlite3 *) passThisPtr->members[L"__database__"]->basicValue.unknown;
            auto script = wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr());
            char *errMsg;
            if (sqlite3_exec(db, script.c_str(), callback, nullptr, &errMsg)) {
                auto e = interpreter::makeErr(
                        L"sqliteError", L"Unable to execute script: " + string2wstring(errMsg));
                sqlite3_free(errMsg);
                throw signalException(e);
            } else {
                return {};
            }
        }

        nativeFn(execute, interpreter, args, passThisPtr) {
            auto in = static_cast<rex::interpreter *>(interpreter);
            auto db = (sqlite3 *) passThisPtr->members[L"__database__"]->basicValue.unknown;
            auto script = wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr());
            auto taskId = callbackTaskCnt++;
            char *errMsg;

            sqlite3_stmt *stmt;
            // parse sqlite3 statement
            if (sqlite3_prepare_v2(db, script.c_str(), -1, &stmt, 0)) {
                try {
                    in->invokeFunc(callbackTasks[taskId].callbackFunc,
                                   {true,
                                    value{string2wstring(sqlite3_errmsg(db)), rex::stringMethods::getMethodsCxt()}},
                                   {});
                    callbackTasks.erase(taskId);
                    return {};
                } catch (...) {
                    // 防止有傻逼他妈的天天搁着throw，先释放资源
                    callbackTasks.erase(taskId);
                    throw;
                }
            }

            callbackTasks[taskId] = {
                    in->moduleCxt,
                    args[1].isRef() ? args[1].refObj : managePtr(args[1]),
                    stmt,
                    {}
            };

            if (sqlite3_exec(db, script.c_str(), callback, (void *) taskId, &errMsg)) {
                try {
                    in->invokeFunc(callbackTasks[taskId].callbackFunc,
                                   {true, value{string2wstring(errMsg), rex::stringMethods::getMethodsCxt()}},
                                   {});
                    sqlite3_free(errMsg);
                    callbackTasks.erase(taskId);
                    return {};
                } catch (...) {
                    // 防止有傻逼他妈的天天搁着throw，先释放资源
                    callbackTasks.erase(taskId);
                    throw;
                }
            } else {
                try {
                    in->invokeFunc(callbackTasks[taskId].callbackFunc,
                                   {false, callbackTasks[taskId].received},
                                   {});
                    callbackTasks.erase(taskId);
                    return {};
                } catch (...) {
                    // 防止有傻逼他妈的天天搁着throw，先释放资源
                    callbackTasks.erase(taskId);
                    throw;
                }
            }
        }

        nativeFn(begin, interpreter, args, passThisPtr) {
            auto db = (sqlite3 *) passThisPtr->members[L"__database__"]->basicValue.unknown;
            char *errMsg;
            if (sqlite3_exec(db, "BEGIN", nullptr, nullptr, &errMsg)) {
                auto e = interpreter::makeErr(
                        L"sqliteError", L"Unable to execute script: " + string2wstring(errMsg));
                sqlite3_free(errMsg);
                throw signalException(e);
            } else {
                return {};
            }
        }

        nativeFn(commit, interpreter, args, passThisPtr) {
            auto db = (sqlite3 *) passThisPtr->members[L"__database__"]->basicValue.unknown;
            char *errMsg;
            if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, &errMsg)) {
                auto e = interpreter::makeErr(
                        L"sqliteError", L"Unable to execute script: " + string2wstring(errMsg));
                sqlite3_free(errMsg);
                throw signalException(e);
            } else {
                return {};
            }
        }

        nativeFn(rollback, interpreter, args, passThisPtr) {
            auto db = (sqlite3 *) passThisPtr->members[L"__database__"]->basicValue.unknown;
            char *errMsg;
            if (sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg)) {
                auto e = interpreter::makeErr(
                        L"sqliteError", L"Unable to execute script: " + string2wstring(errMsg));
                sqlite3_free(errMsg);
                throw signalException(e);
            } else {
                return {};
            }
        }

        value::cxtObject getMethodsCxt(sqlite3 *db) {
            value::cxtObject cxt;
            cxt[L"close"] = managePtr(value{(value::nativeFuncPtr) close});
            cxt[L"executeScript"] = managePtr(value{(value::nativeFuncPtr) executeScript});
            cxt[L"execute"] = managePtr(value{(value::nativeFuncPtr) execute});
            cxt[L"commit"] = managePtr(value{(value::nativeFuncPtr) commit});
            cxt[L"rollback"] = managePtr(value{(value::nativeFuncPtr) rollback});
            cxt[L"begin"] = managePtr(value{(value::nativeFuncPtr) begin});

            cxt[L"__database__"] = managePtr(value{(unknownPtr) db});
            return cxt;
        }
    }
}
