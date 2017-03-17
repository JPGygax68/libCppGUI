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
        explicit Bbox(const Extents &e);
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
        * which are all negative with respect to their orientatio (e.g. x_min is the
        * maximum positive value, x_max the maximum negative value, etc.).
        * The purpose of such a box is to serve as the starting point to find the
        * smallest containing box for a series of contained boxes.
        */
        static auto collapsed() -> Bbox 
        {
            using limits = std::numeric_limits<decltype(x_min)>;

            return { limits::max(), limits::min(), limits::max(), limits::min() };
        }

        auto operator + (const Point &d) const -> Bbox
        {
            auto r{ *this };
            r.x_min -= d.x;
        #ifdef CPPGUI_Y_AXIS_DOWN
            r.y_max += d.y;
        #else
        #error Upward Y axis not supported yet
        #endif
            return r;
        }

        auto expand(Width w) const -> Bbox
        {
            auto d = static_cast<Position_delta>(w);

            return { x_min - d, x_max + d, y_min - d, y_max + d };
        }

        bool is_point_inside(const Point &p) const
        {
        #ifdef CPPGUI_Y_AXIS_DOWN

            // Important: by convention, Y is positive-down in the CppGUI coordinate system; but 
            // Y is positive-up in typography, which means that y_min extends *below* the baseline
            // with negative numbers while y_max goes grows up from the baseline with positive
            // numbers.
            return p.x >= x_min && p.x < x_max && p.y >= - y_max && p.y < - y_min;

        #else
        #error Upward Y axis not supported yet
        #endif
        }

        auto& append_at_bottom(Bbox_cref bbox, Alignment align = horizontal_middle)
        {
            y_min -= bbox.height();

            if (align == horizontal_middle)
            {
                if (bbox.width() > width())
                {
                    auto dw = bbox.width() - width();
                    auto dl = dw / 2;
                    auto dr = dw - dl;
                    x_min -= dl, x_max += dr;
                }
            }
            else if (align == horizontal_origin)
            {
                if (bbox.x_min < x_min) x_min = bbox.x_min;
                if (bbox.x_max > x_max) x_max = bbox.x_max;
            }

            return *this;
        }

        auto& append_at_bottom(Length h)
        {
            y_min -= h;
            return *this;
        }

        auto& append_to_right(Bbox_cref b, Alignment valign = vertical_baseline)
        {
            x_max += - b.x_min + b.x_max;
            // TODO: define an operation merge_vertical() ?
            if (valign == vertical_baseline)
            {
                if (b.y_max > y_max) y_max = b.y_max;
                if (b.y_min < y_min) y_min = b.y_min;
            }
            else if (valign == top)
            {
                auto h = std::max(height(), b.height());
                y_min = - (h - y_max);
            } 
            else 
                assert(false);

            return *this;
        }

        auto& append_to_right(Length w)
        {
            x_max += w;
            return *this;
        }

        auto cut_from_top(Length h) -> Bbox
        {
            auto cutoff{ *this };
            y_max -= h;
            cutoff.y_min = y_max;
            return cutoff;

            //y_max -= h;
            //return *this;
        }

        auto cut_from_bottom(Length h) -> Bbox
        {
            auto cutoff{ *this };
            y_min += h;
            cutoff.y_max = y_min;
            return cutoff;
            //y_min += h;
            //return *this;
        }

        auto cut_from_right(Length w) -> Bbox
        {
            auto cutoff{*this};
            x_max -= w;
            cutoff.x_min = x_max;
            return cutoff;
        }

        auto& merge(Bbox_cref b)
        {
            if (b.x_min < x_min) x_min = b.x_min;
            if (b.x_max > x_max) x_max = b.x_max;
            if (b.y_min < y_min) y_min = b.y_min;
            if (b.y_max > y_max) y_max = b.y_max;
            return *this;
        }

        auto& operator |= (Bbox_cref b)
        {
            merge(b);
            return *this;
        }

        auto operator | (Bbox_cref b)
        {
            auto r{*this};
            r.merge(b);
            return r;
        }

        /*
        * TODO: support specifying horizontal and vertical alignment
        */
        auto position_inside_rectangle(const Rectangle &r) const -> Point;

    };

} // ns cppgui