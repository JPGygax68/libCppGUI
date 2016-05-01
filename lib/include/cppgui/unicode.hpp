#pragma once

#include <string>
#include <codecvt>

namespace cppgui {

    inline auto utf8_to_utf32(const std::string &text) -> std::u32string 
    {
        #if _MSC_VER == 1900
        std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> conv;
        auto inter = conv.from_bytes(text);
        std::u32string converted { reinterpret_cast<const char32_t*>(inter.data()), inter.size() };
        #else
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        auto converted = conv.from_bytes(text);
        #endif

        return converted;
    }

} // ns cppgui

