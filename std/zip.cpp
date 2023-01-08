//
// Created by XIaokang00010 on 2023/1/8.
//

#include "zip.hpp"
#include <exceptions/signalException.hpp>
#include <exceptions/signalBreak.hpp>
#include <cstring>

namespace rexStd::zip {

    namespace file {
        namespace iter {
            value::cxtObject getMethodsCxt(const std::shared_ptr<value> &passThisPtr, vint count) {
                value::cxtObject result;
                result[L"next"] = managePtr(value{value::nativeFuncPtr{next}});

                result[L"__object__"] = passThisPtr;
                result[L"__items__"] = managePtr(value{(vint) count});
                result[L"__current__"] = managePtr(value{(vint) 0});
                return result;
            }

            nativeFn(next, interpreter, args, _) {
                if (!_->members[L"__object__"]->members[L"__entryWorking"]->getBool()) {
                    auto zip = (zip_t *) _->members[L"__object__"]->members[L"__zip__"]->basicValue.unknown;
                    auto &items = _->members[L"__items__"]->getInt();
                    auto &current = _->members[L"__current__"]->getInt();
                    if (current >= items)
                        throw signalBreak();
                    zip_entry_openbyindex(zip, current);
                    current++;
                    return entryObject::getMethodsCxt(zip);
                } else {
                    throw signalException(interpreter::makeErr(L"zipError", L"another entry is working"));
                }
            }
        }

        namespace entryObject {
            value::cxtObject getMethodsCxt(zip_t *zip) {
                value::cxtObject result;
                result[L"close"] = managePtr(value{value::nativeFuncPtr{close}});
                result[L"size"] = managePtr(value{value::nativeFuncPtr{size}});
                result[L"crc32"] = managePtr(value{value::nativeFuncPtr{crc32}});
                result[L"isDir"] = managePtr(value{value::nativeFuncPtr{isDir}});
                result[L"extract"] = managePtr(value{value::nativeFuncPtr{extract}});
                result[L"name"] = managePtr(value{value::nativeFuncPtr{name}});
                result[L"__zip__"] = managePtr(value{(unknownPtr) zip});

                return result;
            }

            nativeFn(close, interpreter, args, _) {
                auto zip = (zip_t *) _->members[L"__zip__"]->basicValue.unknown;
                if (auto res = zip_entry_close(zip); res >= 0) {
                    _->members[L"__entryWorking"]->getBool() = false;
                    return {};
                } else {
                    throw signalException(interpreter::makeErr(
                            L"zipError",
                            L"cannot close zip entry: " + string2wstring(zip_strerror(res))));
                }
            }

            nativeFn(size, interpreter, args, _) {
                auto zip = (zip_t *) _->members[L"__zip__"]->basicValue.unknown;
                return (vint) zip_entry_size(zip);
            }

            nativeFn(crc32, interpreter, args, _) {
                auto zip = (zip_t *) _->members[L"__zip__"]->basicValue.unknown;
                return (vint) zip_entry_crc32(zip);
            }

            nativeFn(isDir, interpreter, args, _) {
                auto zip = (zip_t *) _->members[L"__zip__"]->basicValue.unknown;
                return (vbool) zip_entry_isdir(zip);
            }

            nativeFn(extract, interpreter, args, _) {
                auto zip = (zip_t *) _->members[L"__zip__"]->basicValue.unknown;
                // set callback
                callbackMsg callbackInstance;
                callbackInstance.in = (rex::interpreter *) interpreter;
                callbackInstance.callbackT = args[0].isRef() ? args[0].refObj : managePtr(args[0]);

                // invoke
                if (auto res = zip_entry_extract(zip, callbackAdaptOnExtractFile, (void *) &callbackInstance);
                        res >= 0) {
                    return {};
                } else {
                    throw signalException(interpreter::makeErr(
                            L"zipError",
                            L"cannot extract file: " + string2wstring(zip_strerror(res))));
                }
            }

            nativeFn(name, interpreter, args, _) {
                auto zip = (zip_t *) _->members[L"__zip__"]->basicValue.unknown;
                return {string2wstring(zip_entry_name(zip)), rex::stringMethods::getMethodsCxt()};
            }
        }

        value::cxtObject getMethodsCxt(zip_t *zip) {
            value::cxtObject result;
            result[L"entry"] = managePtr(value{value::nativeFuncPtr{entry}});
            result[L"entries"] = managePtr(value{value::nativeFuncPtr{entries}});
            result[L"delete"] = managePtr(value{value::nativeFuncPtr{Delete}});
            result[L"rexIter"] = managePtr(value{value::nativeFuncPtr{rexIter}});

            result[L"__zip__"] = managePtr(value{(unknownPtr) zip});
            result[L"__entryWorking__"] = managePtr(value{(vbool) false});
            return result;
        }

        nativeFn(entry, interpreter, args, _) {
            if (!_->members[L"__entryWorking"]->getBool()) {
                _->members[L"__entryWorking"]->getBool() = true;
                auto zip = (zip_t *) _->members[L"__zip__"]->basicValue.unknown;
                return entryObject::getMethodsCxt(zip);
            } else {
                throw signalException(interpreter::makeErr(L"zipError", L"Another entry are working"));
            }
        }

        nativeFn(entries, interpreter, args, _) {
            auto zip = (zip_t *) _->members[L"__zip__"]->basicValue.unknown;
            if (auto res = zip_entries_total(zip); res >= 0) {
                return (vint) res;
            } else {
                throw signalException(interpreter::makeErr(
                        L"zipError",
                        L"cannot get entries total: " + string2wstring(zip_strerror(res))));
            }
        }

        nativeFn(Delete, interpreter, args, _) {
            vbytes filename = wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr());

            auto zip = (zip_t *) _->members[L"__zip__"]->basicValue.unknown;
            char *wdnmd[] = {filename.data()};
            if (auto res = zip_entries_delete(zip, wdnmd, 1); res >= 0) {
                return {};
            } else {
                throw signalException(interpreter::makeErr(
                        L"zipError",
                        L"cannot delete entry: " + string2wstring(zip_strerror(res))));
            }
        }

        nativeFn(rexIter, interpreter, args, _) {
            auto zip = (zip_t *) _->members[L"__zip__"]->basicValue.unknown;
            if (auto res = zip_entries_total(zip); res >= 0) {
                return iter::getMethodsCxt(_, (vint) res);
            } else {
                throw signalException(interpreter::makeErr(
                        L"zipError",
                        L"cannot iterate entries: " + string2wstring(zip_strerror(res))));
            }
        }
    }


    value::cxtObject getMethodsCxt() {
        value::cxtObject result;
        result[L"open"] = managePtr(value{value::nativeFuncPtr{open}});
        result[L"extract"] = managePtr(value{value::nativeFuncPtr{extract}});
        result[L"ZIP_DEFAULT_COMPRESSION_LEVEL"] = managePtr(value{(vint) ZIP_DEFAULT_COMPRESSION_LEVEL});
        result[L"M_WRITE"] = managePtr(value{(vint) 1});
        result[L"M_READ"] = managePtr(value{(vint) 2});
        result[L"M_APPEND"] = managePtr(value{(vint) 3});
        result[L"M_DELETE"] = managePtr(value{(vint) 4});
        return result;
    }

    nativeFn(open, interpreter, args, _) {
        vbytes filename = wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr());
        auto compressionLvl = (int) (args[1].isRef() ? args[1].getRef().getInt() : args[1].getInt());
        auto openMode = (int) (args[2].isRef() ? args[2].getRef().getInt() : args[2].getInt());
        switch (openMode) {
            case 1:
                openMode = 'w';
                break;
            case 2:
                openMode = 'r';
                break;
            case 3:
                openMode = 'a';
                break;
            case 4:
                openMode = 'd';
                break;
            default:
                throw signalException(interpreter::makeErr(L"zipError", L"invalid open mode"));
        }
        if (zip_t *zip = zip_open(filename.c_str(), compressionLvl, (char) openMode); zip != nullptr) {
            return file::getMethodsCxt(zip);
        } else {
            throw signalException(interpreter::makeErr(L"zipError", L"cannot open file"));
        }
    }

    nativeFn(extract, interpreter, args, _) {
        vbytes filename = wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr());
        vbytes path = wstring2string(args[1].isRef() ? args[1].getRef().getStr() : args[1].getStr());
        // set callback
        callbackMsg callbackInstance{};
        callbackInstance.in = (rex::interpreter *) interpreter;
        callbackInstance.callbackT = args[2].isRef() ? args[2].refObj : managePtr(args[2]);

        if (auto res = zip_extract(filename.c_str(), path.c_str(), callbackAdaptOnExtract, (void *) &callbackInstance);
                res != 0) {
            throw signalException(
                    interpreter::makeErr(L"zipError", L"cannot extract file: " + string2wstring(zip_strerror(res))));
        }
        return {};
    }

    int callbackAdaptOnExtract(const char *filename, void *arg) {
        auto instance = (callbackMsg *) arg;
        instance->in->invokeFunc(instance->callbackT, {{string2wstring(filename), rex::stringMethods::getMethodsCxt()}},
                                 {});
        return 0;
    }

    size_t callbackAdaptOnExtractFile(void *arg, uint64_t offset, const void *data, size_t size) {
        auto instance = (callbackMsg *) arg;
        value param = {"", bytesMethods::getMethodsCxt()};
        param.getBytes().resize(size);
        memcpy(param.getBytes().data(), data, size);
        instance->in->invokeFunc(instance->callbackT, {param}, {});
        return size;
    }
}
