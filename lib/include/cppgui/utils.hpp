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

namespace cppgui {

    template<typename T> struct argument_type;
    template<typename T, typename U> struct argument_type<T(U*)> { typedef U type; };

    #define UNPACK_TYPE(t) typename cppgui::argument_type<void t>::type

    template<typename T>
    T clamp(const T &value, const T &low, const T &high)
    {
        if      (value < low ) return low; 
        else if (value > high) return high;
        else                   return value;
    }

} // ns cppgui
