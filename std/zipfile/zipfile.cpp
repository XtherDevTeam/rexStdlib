// This file is auto generated by FFI generator for reXscript.
#include "cstyles/zip.c"
#include "rex.hpp"
using namespace rex;
struct cStr {
std::string str{};
cStr(const std::string &str) : str(str) {}
operator char*() {
return str.data();
}
};

nativeFn(_rexExport_close,interpreter,args,passThisPtr) {
zip_t * arg0 = (zip_t *)((args[0].isRef() ? args[0].getRef() : args[0]).basicValue.unknown);
zip_close(arg0);
return {};
}
nativeFn(_rexExport_entryClose,interpreter,args,passThisPtr) {
zip_t * arg0 = (zip_t *)((args[0].isRef() ? args[0].getRef() : args[0]).basicValue.unknown);
auto rexCallResult = zip_entry_close(arg0);
return (value){(vint)(rexCallResult)};
}
nativeFn(_rexExport_entryOpen,interpreter,args,passThisPtr) {
zip_t * arg0 = (zip_t *)((args[0].isRef() ? args[0].getRef() : args[0]).basicValue.unknown);
cStr arg1 = (cStr)(rex::wstring2string((args[1].isRef() ? args[1].getRef() : args[1]).getStr()));
auto rexCallResult = zip_entry_open(arg0,arg1);
return (value){(vint)(rexCallResult)};
}
nativeFn(_rexExport_entryWrite,interpreter,args,passThisPtr) {
zip_t * arg0 = (zip_t *)((args[0].isRef() ? args[0].getRef() : args[0]).basicValue.unknown);
void * arg1 = (void *)((args[1].isRef() ? args[1].getRef() : args[1]).getBytes().data());
int64_t arg2 = (int64_t)((args[2].isRef() ? args[2].getRef() : args[2]).getInt());
auto rexCallResult = zip_entry_write(arg0,arg1,arg2);
return (value){(vint)(rexCallResult)};
}
nativeFn(_rexExport_open,interpreter,args,passThisPtr) {
cStr arg0 = (cStr)(rex::wstring2string((args[0].isRef() ? args[0].getRef() : args[0]).getStr()));
int arg1 = (int)((args[1].isRef() ? args[1].getRef() : args[1]).getInt());
int arg2 = (int)((args[2].isRef() ? args[2].getRef() : args[2]).getInt());
auto rexCallResult = zip_open(arg0,arg1,arg2);
return (value){(unknownPtr)(rexCallResult)};
}
extern "C" void rexModInit(const managedPtr<environment> &env, const managedPtr<value> &mod) {
mod->members[L"close"] = managePtr(value{(value::nativeFuncPtr) _rexExport_close});
mod->members[L"entryClose"] = managePtr(value{(value::nativeFuncPtr) _rexExport_entryClose});
mod->members[L"entryOpen"] = managePtr(value{(value::nativeFuncPtr) _rexExport_entryOpen});
mod->members[L"entryWrite"] = managePtr(value{(value::nativeFuncPtr) _rexExport_entryWrite});
mod->members[L"open"] = managePtr(value{(value::nativeFuncPtr) _rexExport_open});
}

