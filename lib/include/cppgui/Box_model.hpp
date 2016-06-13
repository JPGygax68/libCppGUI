#pragma once

#include "./geometry.hpp"

namespace cppgui
{
    
    template<class Impl, class Parent>
    struct Box_model: Parent
    {
        static constexpr bool has_border () { return false; };
        static constexpr bool has_padding() { return false; };

        static constexpr auto inner_rectangle(const Extents &ext) { return ext; }
        static constexpr auto get_border_width(int border) { return 0; }
        static constexpr auto get_padding(int border) { return 0; }
    };
    
    template<class Parent>
    struct Simple_box_model: Box_model<Simple_box_model<Parent>, Parent>
    {
        //static constexpr auto inner_rectangle(const Extents &ext) { return { Extents{ 0, 0 }, ext }; }
        //static constexpr auto get_border_width(int border) { return 0; }
        //static constexpr auto get_padding(int border) { return 0; }
    };

    template<Width Padding, class Parent>
    struct Simple_padded_box_model: Box_model<Simple_box_model<Parent>, Parent>
    {
        static constexpr bool has_padding() { return true; }

        static constexpr auto inner_rectangle(const Extents &ext)
        {
            return { Extents{ 0, 0 }, { ext.w - 2 * Padding, ext.h - 2 * Padding } };
        }
        static constexpr auto get_border_width(int /*border*/) { return 0; }
        static constexpr auto get_padding(int /*border*/) { return Padding; }
    };

} // ns cppgui