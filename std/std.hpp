//
// Created by XIaokang00010 on 2022/12/22.
//

#ifndef REXSTDLIB_STD_HPP
#define REXSTDLIB_STD_HPP

#include "fs.hpp"
#include "sqlite.hpp"

using namespace rex;

extern "C" void rexModInit(const managedPtr<environment> &env, const managedPtr<value> &mod);

#endif //REXSTDLIB_STD_HPP
