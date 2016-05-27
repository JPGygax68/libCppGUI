#pragma once

#include <map>

namespace cppgui {

    template<typename T> struct argument_type;
    template<typename T, typename U> struct argument_type<T(U*)> { typedef U type; };

    #define UNPACK_TYPE(t) typename cppgui::argument_type<void t>::type

} // ns cppgui
