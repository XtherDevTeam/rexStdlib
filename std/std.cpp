//
// Created by XIaokang00010 on 2022/12/22.
//

#include "std.hpp"

void rexModInit(const managedPtr<environment> &env, const managedPtr<value> &mod) {
    mod->members[L"fs"] = managePtr(value{rexStd::fs::getMethodsCxt()});
    mod->members[L"zipfile"] = managePtr(value{value::cxtObject{}}); // reserve a place to mount zipfile
}
