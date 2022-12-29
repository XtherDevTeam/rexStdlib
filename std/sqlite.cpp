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

    static int callback(void *cbMsg, int argc, char **argv, char **azColName) {
        return 0;
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
            auto db = (sqlite3 *) passThisPtr->members[L"__database__"]->basicValue.unknown;
            auto script = wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr());

            value receivedContent = {value::vecObject{}, rex::vecMethods::getMethodsCxt()};

            sqlite3_stmt *stmt;
            // parse sqlite3 statement
            if (sqlite3_prepare_v2(db, script.c_str(), -1, &stmt, 0)) {
                auto e = interpreter::makeErr(L"sqliteError", string2wstring(sqlite3_errmsg(db)));
                sqlite3_close(db);
                throw signalException(e);
            }

            while (sqlite3_step(stmt) == SQLITE_ROW) {
                receivedContent.getVec().push_back(managePtr(value{value::cxtObject{}}));
                auto line = receivedContent.getVec().back();

                int count = sqlite3_column_count(stmt);
                for (int i = 0;i < count;i++) {
                    auto &toInsert = line->members[string2wstring(sqlite3_column_name(stmt, i))];
                    int type = sqlite3_column_type(stmt, i);
                    switch (type) {
                        case SQLITE_INTEGER: {
                            toInsert = managePtr(value{(vint)sqlite3_column_int(stmt, i)});
                            break;
                        }
                        case SQLITE_FLOAT: {
                            toInsert = managePtr(value{(vdeci) sqlite3_column_double(stmt, i)});
                            break;
                        }
                        case SQLITE_TEXT: {
                            // covert to vbytes object directly, user can use `val.decode('charset')` to decode it.
                            toInsert = managePtr(value{(char*)sqlite3_column_text(stmt, i), rex::bytesMethods::getMethodsCxt()});
                            break;
                        }
                        case SQLITE_BLOB: {
                            int len = sqlite3_column_bytes(stmt, i);
                            toInsert = managePtr(value{vbytes(), rex::bytesMethods::getMethodsCxt()});
                            toInsert->getBytes().resize(len);
                            memcpy(toInsert->getBytes().data(), sqlite3_column_blob(stmt, i), len);
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
            }

            return receivedContent;
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
