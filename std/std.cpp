//
// Created by XIaokang00010 on 2022/12/22.
//

#include "std.hpp"

void rexModInit(const managedPtr<environment> &env, const managedPtr<value> &mod) {
    mod->members[L"time"] = managePtr(value{rexStd::time::getMethodsCxt()});
    mod->members[L"fs"] = managePtr(value{rexStd::fs::getMethodsCxt()});
    mod->members[L"sqlite"] = managePtr(value{rexStd::sqlite::getMethodsCxt()});
    mod->members[L"net"] = managePtr(value{rexStd::net::getMethodsCxt()});
    mod->members[L"json"] = managePtr(value{rexStd::json::getMethodsCxt()});
    mod->members[L"zip"] = managePtr(value{rexStd::zip::getMethodsCxt()});
}
