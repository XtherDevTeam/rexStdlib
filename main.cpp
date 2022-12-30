#include <iostream>

#include <rex.hpp>

int main() {
    auto env = rex::getRexEnvironment();
    auto mod = rex::importNativeModuleEx(env, L"../dist/libstd_zipfile.dylib", {});
    return 0;
}
