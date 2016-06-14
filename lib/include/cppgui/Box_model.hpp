#pragma once

#include "./geometry.hpp"

namespace cppgui
{
    
    template<class Impl, class Parent>
    struct Box_model: Parent
    {
        static constexpr bool has_border () { return false; };
        static constexpr bool has_padding() { return false; };

        // static constexpr auto inner_rectangle(const Extents &ext) { return ext; }

        constexpr auto inner_rectangle(const Extents &ext) const -> Rectangle
        {
            return { 
                { 0, 0 }, 
                { ext.w - p()->get_padding(3) - p()->get_padding(1), ext.h - p()->get_padding(0) - p()->get_padding(2) } 
            };
        }

        static constexpr auto get_border_width(int border) { return 0; }
        static constexpr auto get_padding(int border) { return 0; }

    private:
        auto p() { return static_cast<Impl*>(this); }
    };
    
    template<class Parent>
    struct Simple_box_model: Box_model<Simple_box_model<Parent>, Parent>
    {
        //static constexpr auto inner_rectangle(const Extents &ext) { return { Extents{ 0, 0 }, ext }; }
        //static constexpr auto get_border_width(int border) { return 0; }
        //static constexpr auto get_padding(int border) { return 0; }
    };

    /** This box model has no border, no margin and only a fixed-width padding.
     */
    template<Width Width, class Parent>
    struct Fixed_padding_box_model: Box_model<Simple_box_model<Parent>, Parent>
    {
        static constexpr bool has_padding() { return true; }

        // static constexpr auto get_border_width(int /*border*/) { return 0; }
        
        static constexpr auto get_padding(int /*border*/) { return Width; }
    };

} // ns cppgui