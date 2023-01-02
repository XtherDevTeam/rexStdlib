#include <iostream>

#include <rex.hpp>
#include "std/std.hpp"

int main() {
//    auto env = rex::getRexEnvironment();
//    auto mod = rex::importNativeModuleEx(env, L"../dist/libstd_zipfile.dylib", {});
    using namespace rex;
    vstr str = L"{\n"
               "  \"nullValue\": null,\n"
               "  \"intValue\": 123,\n"
               "  \"deciValue\": 3.14,\n"
               "  \"boolValue\": true,\n"
               "  \"strValue\": \"hello, \\\"world\\\"\",\n"
               "  \"vecValue\": [1, 2, 3],\n"
               "  \"objValue\": {\n"
               "    \"key1\": \"value1\",\n"
               "    \"key2\": \"value2\"\n"
               "  }\n"
               "}";
    value result;
    rexStd::json::loadObjectToJson(str, result, 0);
    return 0;
}
