#include <iostream>

#include "std/json.hpp"

int main() {
    std::string s = "{\n"
    "  \"name\": \"std\",\n"
    "  \"description\": \"reXscript Standard Library\",\n"
    "  \"version\": \"0.1\",\n"
    "  \"dependencies\": {}\n"
    "}";
    rex::value res{};
    auto result = rexStd::json::loadObjectToJson(rex::string2wstring(s), res, 0);
    return 0;
}
