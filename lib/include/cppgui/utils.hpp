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

    template<typename T1, typename T2>
    struct copy_constness
    {
        using type = std::conditional_t<std::is_const<T1>::value, std::add_const_t<T2>, T2>;
    };

    template<typename T1, typename T2> using copy_constness_t = typename copy_constness<T1, T2>::type;

    /** Specialized application of SFINAE: enable a return type if the specified class is, or is derived from, a specified
        base class, AND also apply the constness of the class to the return type.
     */

    template<typename Class, typename Base, typename Return>
    struct enable_if_class_ref_copy_constness
    {
        using type = std::enable_if_t<
            std::is_base_of<Base, std::remove_const_t<Class>>::value, 
            copy_constness_t<std::remove_reference_t<Class>, 
            Return>>;
    };

    template<typename Class, typename Base, typename Return> 
    using enable_if_class_ref_copy_constness_t = typename enable_if_class_ref_copy_constness<Class, Base, Return>::type;

    // Even more specific use case

    template<typename Class, typename Base, typename Return> 
    using enable_member_ref_for_class_t = typename enable_if_class_ref_copy_constness<Class, Base, Return>::type &;

} // ns cppgui
