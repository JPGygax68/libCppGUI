#pragma once

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016 Hans-Peter Gygax

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

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

