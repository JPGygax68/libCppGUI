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
                { p()->get_distance(3), p()->get_distance(0) }, 
                { ext.w - p()->get_distance(3) - p()->get_distance(1), ext.h - p()->get_distance(0) - p()->get_distance(2) } 
            };
        }

        static constexpr auto border_rectangle(const Extents &ext) -> Rectangle
        {
            // TODO: margins!
            return { { 0, 0 }, ext };
        }

        constexpr auto get_margin(int /*border*/) const { return 0; }

        constexpr auto get_border_width(int /*border*/) const { return 0; }
        
        // TODO: indirect via protected method (does not need default values for parameters) ?
        constexpr auto get_border_color(int border, bool enabled = true, bool hovered = false, bool focused = false) const { return Color{ 0, 0, 0, 1}; }
        
        constexpr auto get_padding(int /*border*/) const { return 0; }
        
        constexpr auto get_distance(int border) const { return p()->get_margin(border) + p()->get_border_width(border) + p()->get_padding(border); }

    private:
        auto p() { return static_cast<Impl*>(this); }
        auto p() const { return static_cast<const Impl*>(this); }
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
    struct Fixed_padding_box_model: Box_model<Fixed_padding_box_model<Width, Parent>, Parent>
    {
        static constexpr bool has_padding() { return true; }

        static constexpr auto get_padding(int /*border*/) { return Width; }
    };

    template<Width BorderWidth, Width PaddingWidth, class Parent>
    struct Fixed_border_and_padding_box_model: 
        Box_model<Fixed_border_and_padding_box_model<BorderWidth, PaddingWidth, Parent>, 
            Parent>
    {
        static constexpr bool has_border () { return true; }
        static constexpr bool has_padding() { return true; }

        static constexpr auto get_border_width(int /*border*/) { return BorderWidth; }

        auto get_border_color(int border, bool /*enabled*/ = true, bool hovered = false, bool /*focused*/ = false) const
        {
            if (hovered)
                return Color{ 0, 0, 0.5f, 1};
            else
                return Color{ 0, 0, 0, 1};
        }

        static constexpr auto get_padding(int /*border*/) { return PaddingWidth; }
    };
} // ns cppgui