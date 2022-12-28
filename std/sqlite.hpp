//
// Created by XIaokang00010 on 2022/12/28.
//

#ifndef REXSTDLIB_SQLITE_HPP
#define REXSTDLIB_SQLITE_HPP

#include <rex.hpp>
#include "sqlite/sqlite3.h"

namespace rexStd::sqlite {
    using namespace rex;

    struct callbackMsgT {
        managedPtr<value> moduleCxt;
        managedPtr<value> callbackFunc;
        sqlite3_stmt *stmt;
        value received;
    };

    // now you don't need to define it again
    inline map<vint, callbackMsgT> callbackTasks;

    inline vint callbackTaskCnt{1};

    static int callback(void *cbMsg, int argc, char **argv, char **azColName);


    nativeFn(open, interpreter, args, passThisPtr);

    namespace database {
        nativeFn(close, interpreter, args, passThisPtr);

        nativeFn(executeScript, interpreter, args, passThisPtr);

        nativeFn(execute, interpreter, args, passThisPtr);

        nativeFn(begin, interpreter, args, passThisPtr);

        nativeFn(commit, interpreter, args, passThisPtr);

        nativeFn(rollback, interpreter, args, passThisPtr);

        value::cxtObject getMethodsCxt(sqlite3 *db);
    }

    value::cxtObject getMethodsCxt();
}

#endif //REXSTDLIB_SQLITE_HPP
