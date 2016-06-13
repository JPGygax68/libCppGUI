#pragma once

#include "./geometry.hpp"

namespace cppgui
{
    
    template<class Impl, class Parent>
    struct Box_model: Parent
    {
        static constexpr bool has_border() { static_assert(false, "Box_model::has_border()"); };

        static constexpr auto inner_rectangle(const Extents &) { static_assert(false, "Box_model::inner_rectangle()"); }
        static constexpr auto get_border_width(int border) { static_assert(false, "Box_model::get_border_width()"); }
    };
    
    template<class Parent>
    struct Simple_box_model: Box_model<Simple_box_model<Parent>, Parent>
    {
        static constexpr bool has_border() { return false; }

        static constexpr auto inner_rectangle(const Extents &ext) { return { Extents{ 0, 0 }, ext }; }
        static constexpr auto get_border_width(int border) { return 0; }
    };

} // ns cppgui