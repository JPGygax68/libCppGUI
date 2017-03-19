#pragma once

#include <cppgui_config.hpp>
#include "./fonts/Bounding_box.hpp"
#include "./Extents.hpp"


namespace cppgui {
    
    struct Extents;


    class Bbox: public fonts::Bounding_box {
    public:

        /*
        * When initialized from an Extents struct, the bounding box is positioned at (0, 0),
        * the width is assigned to x_max, and the height to y_max - the ascendent - while
        * the descendent is set to 0.
        */
    #ifdef CPPGUI_Y_AXIS_DOWN
        Bbox(const Extents &e, Alignment h_ref, Alignment v_ref);
    #else
    #error Upward Y axis not supported yet
    #endif

        Bbox(const Text_bounding_box &from): Text_bounding_box{from} {}

        Bbox(Position x_min, Position x_max, Position y_min, Position y_max):
            fonts::Bounding_box{x_min, x_max, y_min, y_max} {}

        Bbox() = default;

        auto extents() const { return Extents{ - x_min + x_max, y_max + - y_min }; }

        static auto empty() -> Bbox { return { 0, 0, 0, 0 }; }

        /*
        * Returns a bounding box that is completely collapsed, i.e. the boundaries of
        * which are all negative with respect to their orientation (e.g. x_min is the
        * maximum positive value, x_max the maximum negative value, etc.).
        * The purpose of such a box is to serve as the starting point to find the
        * smallest containing box for a series of contained boxes.
        */
        static auto collapsed() -> Bbox;

        auto origin() const -> Vector;

        auto operator +(const Point &d) const -> Bbox;

        auto expand(Width w) const -> Bbox;

        bool contains_point(const Point &p) const;

        auto append_at_bottom(Bbox_cref bbox, Alignment align = horizontal_middle) -> Bbox_ref;

        auto append_at_bottom(Length h) -> Bbox_ref;

        auto append_to_right(Bbox_cref b, Alignment valign = vertical_baseline) -> Bbox_ref;

        auto append_to_right(Length w) -> Bbox_ref;

        auto cut_from_top(Length h) -> Bbox;

        auto cut_from_bottom(Length h) -> Bbox;

        auto cut_from_right(Length w) -> Bbox;

        auto merge(Bbox_cref b) -> Bbox;

        auto operator |=(Bbox_cref b) -> Bbox_ref;

        auto operator |(Bbox_cref b) const -> Bbox;

        /*
        * TODO: support specifying horizontal and vertical alignment
        */
        auto position_inside_rectangle(const Rectangle &r) const -> Point;

        //auto expand_to(Bbox_cref cont, Alignment h_ref = center, Alignment v_ref = middle) -> Bbox_ref;
        auto expand_to(const Extents &, Alignment h_ref = center, Alignment v_ref = middle) -> Bbox_ref;
    };

} // ns cppgui