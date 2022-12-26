//
// Created by XIaokang00010 on 2022/12/26.
//

#ifndef REXSTDLIB_WORKAROUND_HPP
#define REXSTDLIB_WORKAROUND_HPP

#include <filesystem>
#include "share/share.hpp"

namespace rex::workaround {
    using pathType = std::filesystem::path::string_type;

    // A simple workaround for different path::value_type
    template<typename T>
    pathType strToPathType(const std::basic_string<T> &str) {
        if constexpr (std::is_same_v<pathType::value_type , typename std::basic_string<T>::value_type>) {
            return str;
        } else if constexpr (std::is_same_v<pathType::value_type , vbyte> &&
                std::is_same_v<typename std::basic_string<T>::value_type , vchar>) {
            return rex::wstring2string(str);
        } else if constexpr (std::is_same_v<pathType::value_type , vchar> &&
                             std::is_same_v<typename std::basic_string<T>::value_type , vbyte>) {
            return rex::string2wstring(str);
        } else {
            return {};
        }
    }
}

#endif //REXSTDLIB_WORKAROUND_HPP
