//
// Created by XIaokang00010 on 2022/12/22.
//

#include <unistd.h>

#include "fs.hpp"
#include "exceptions/signalException.hpp"

namespace rexStd::fs {
    nativeFn(open, interpreter, args, _) {
        vbytes filePath{wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr())};
        vbytes flag{wstring2string(args[1].isRef() ? args[1].getRef().getStr() : args[1].getStr())};
        FILE *fp = fopen(filePath.data(), flag.data());

        if (!fp) {
            throw signalException{
                    interpreter::makeErr(L"fsError", L"Unable to open file: [Errno " + std::to_wstring(errno) + L"]")};
        } else {
            return file::constructFileObject(fp);
        }
    }

    value file::constructFileObject(FILE *fp) {
        value v{getMethodsCxt()};
        v.members[L"rexFile"] = managePtr(value{(unknownPtr) fp});
        if (fseek(fp, 0, SEEK_END) == -1)
            throw signalException{interpreter::makeErr(L"fsError",
                                                       L"Cannot access filestream: [Errno " + std::to_wstring(errno) +
                                                       L"]")};
        vint len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        v.members[L"length"] = managePtr(value{len});
        return v;
    }

    nativeFn(file::read, interpreter, args, passThisPtr) {
        auto fp = (FILE *) passThisPtr->members[L"rexFile"]->basicValue.unknown;
        if (!fp)
            throw signalException{interpreter::makeErr(L"fsError", L"Invalid member: `rexFile` == nullptr")};
        vint readLen = args[0].isRef() ? args[0].getRef().getInt() : args[0].getInt();
        vbytes dest(readLen, '\0');
        fread(dest.data(), readLen, 1, fp);
        return {dest, bytesMethods::getMethodsCxt()};
    }

    nativeFn(file::write, interpreter, args, passThisPtr) {
        auto fp = (FILE *) passThisPtr->members[L"rexFile"]->basicValue.unknown;
        if (!fp)
            throw signalException{interpreter::makeErr(L"fsError", L"Invalid member: `rexFile` == nullptr")};

        vbytes src(args[0].isRef() ? args[0].getRef().getBytes() : args[0].getBytes(), '\0');
        if (fwrite(src.data(), src.size(), 1, fp) == -1)
            throw signalException{interpreter::makeErr(L"fsError",
                                                       L"Cannot access filestream: [Errno " + std::to_wstring(errno) +
                                                       L"]")};

        return {};
    }

    nativeFn(file::eof, interpreter, args, passThisPtr) {
        auto fp = (FILE *) passThisPtr->members[L"rexFile"]->basicValue.unknown;
        if (!fp)
            throw signalException{interpreter::makeErr(L"fsError", L"Invalid member: `rexFile` == nullptr")};

        return {!feof(fp)};
    }

    nativeFn(file::tell, interpreter, args, passThisPtr) {
        auto fp = (FILE *) passThisPtr->members[L"rexFile"]->basicValue.unknown;
        if (!fp)
            throw signalException{interpreter::makeErr(L"fsError", L"Invalid member: `rexFile` == nullptr")};

        return {(vint) ftell(fp)};
    }

    nativeFn(file::seek, interpreter, args, passThisPtr) {
        auto fp = (FILE *) passThisPtr->members[L"rexFile"]->basicValue.unknown;
        if (!fp)
            throw signalException{interpreter::makeErr(L"fsError", L"Invalid member: `rexFile` == nullptr")};

        if (fseek(fp, args[0].isRef() ? args[0].getRef().getInt() : args[0].getInt(), SEEK_SET) == -1)
            throw signalException{interpreter::makeErr(L"fsError",
                                                       L"Cannot access filestream: [Errno " + std::to_wstring(errno) +
                                                       L"]")};

        return {};
    }

    nativeFn(file::close, interpreter, args, passThisPtr) {
        auto fp = (FILE *) passThisPtr->members[L"rexFile"]->basicValue.unknown;
        if (!fp)
            throw signalException{interpreter::makeErr(L"fsError", L"Invalid member: `rexFile` == nullptr")};

        if (fclose(fp) == -1)
            throw signalException{interpreter::makeErr(L"fsError",
                                                       L"Cannot access filestream: [Errno " + std::to_wstring(errno) +
                                                       L"]")};

        return {};
    }

    value::cxtObject file::getMethodsCxt() {
        value::cxtObject cxt;
        cxt[L"read"] = managePtr(value{(value::nativeFuncPtr) read});
        cxt[L"write"] = managePtr(value{(value::nativeFuncPtr) write});
        cxt[L"tell"] = managePtr(value{(value::nativeFuncPtr) tell});
        cxt[L"seek"] = managePtr(value{(value::nativeFuncPtr) seek});
        cxt[L"close"] = managePtr(value{(value::nativeFuncPtr) close});
        cxt[L"eof"] = managePtr(value{(value::nativeFuncPtr) eof});
        return cxt;
    }

    value::cxtObject getMethodsCxt() {
        value::cxtObject cxt;
        cxt[L"open"] = managePtr(value{(value::nativeFuncPtr) open});

        cxt[L"stat"] = managePtr(value{(value::nativeFuncPtr) stat});
        cxt[L"access"] = managePtr(value{(value::nativeFuncPtr) access});
        cxt[L"unlink"] = managePtr(value{(value::nativeFuncPtr) unlink});
        cxt[L"mkdir"] = managePtr(value{(value::nativeFuncPtr) mkdir});
        cxt[L"mkdirs"] = managePtr(value{(value::nativeFuncPtr) mkdirs});
        cxt[L"F_OK"] = managePtr(value{(vint) F_OK});
        cxt[L"W_OK"] = managePtr(value{(vint) W_OK});
        cxt[L"R_OK"] = managePtr(value{(vint) R_OK});
        cxt[L"X_OK"] = managePtr(value{(vint) X_OK});

        return cxt;
    }

    nativeFn(access, interpreter, args, _) {
        vbytes filePath{wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr())};
        vint flag = args[1].isRef() ? args[1].getRef().getInt() : args[1].getInt();
        return {::access(filePath.c_str(), (int) flag) == 0};
    }

    nativeFn(stat, interpreter, args, _) {
        vbytes filePath{wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr())};
        std::error_code ec;
        std::filesystem::file_status stat = std::filesystem::status(filePath, ec);
        if (ec)
            throw signalException{
                    interpreter::makeErr(L"fsError", L"Unable to stat file: [Errno " + std::to_wstring(ec.value()) + L"]" +
                                                     string2wstring(ec.message()))};
        value ret{};
        switch (stat.type()) {
            case std::filesystem::file_type::none:
                ret = value{L"none", rex::stringMethods::getMethodsCxt()};
                break;
            case std::filesystem::file_type::not_found:
                ret = value{L"notFound", rex::stringMethods::getMethodsCxt()};
                break;
            case std::filesystem::file_type::regular:
                ret = value{L"regular", rex::stringMethods::getMethodsCxt()};
                break;
            case std::filesystem::file_type::directory:
                ret = value{L"directory", rex::stringMethods::getMethodsCxt()};
                break;
            case std::filesystem::file_type::symlink:
                ret = value{L"symlink", rex::stringMethods::getMethodsCxt()};
                break;
            case std::filesystem::file_type::block:
                ret = value{L"block", rex::stringMethods::getMethodsCxt()};
                break;
            case std::filesystem::file_type::character:
                ret = value{L"character", rex::stringMethods::getMethodsCxt()};
                break;
            case std::filesystem::file_type::fifo:
                ret = value{L"fifo", rex::stringMethods::getMethodsCxt()};
                break;
            case std::filesystem::file_type::socket:
                ret = value{L"socket", rex::stringMethods::getMethodsCxt()};
                break;
            case std::filesystem::file_type::unknown:
                ret = value{L"unknown", rex::stringMethods::getMethodsCxt()};
                break;
        }
        return ret;
    }

    nativeFn(unlink, interpreter, args, _) {
        vbytes filePath{wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr())};
        std::error_code ec;
        if (!std::filesystem::remove(filePath, ec))
            throw signalException{
                    interpreter::makeErr(L"fsError", L"Unable to unlink file: [Errno " + std::to_wstring(ec.value()) + L"]" +
                            string2wstring(ec.message()))};
        return {};
    }

    nativeFn(mkdir, interpreter, args, _) {
        vbytes filePath{wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr())};
        std::error_code ec;
        if (std::filesystem::create_directory(filePath, ec); ec)
            throw signalException{
                    interpreter::makeErr(L"fsError", L"Unable to create directory: [Errno " + std::to_wstring(ec.value()) + L"]" +
                                                     string2wstring(ec.message()))};
        return {};
    }

    nativeFn(mkdirs, interpreter, args, _) {
        vbytes filePath{wstring2string(args[0].isRef() ? args[0].getRef().getStr() : args[0].getStr())};
        std::error_code ec;
        if (std::filesystem::create_directories(filePath, ec); ec)
            throw signalException{
                    interpreter::makeErr(L"fsError", L"Unable to create directories: [Errno " + std::to_wstring(ec.value()) + L"]" +
                                                     string2wstring(ec.message()))};
        return {};
    }
}