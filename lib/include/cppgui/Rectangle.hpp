#pragma once

#include "./basic_types.hpp"
#include "./Extents.hpp"


namespace cppgui {
    
    struct Rectangle {
        Point           pos;
        Extents         ext;

        Rectangle(Bbox_cref b);

        Rectangle(const Rectangle &) = default;
        Rectangle(Rectangle &&) = default;
        Rectangle(const Extents &ext_): pos{0, 0}, ext{ext_} {}
        Rectangle() = default;
        Rectangle & operator = (const Rectangle &) = default;

        explicit Rectangle(const Point &p, const Extents &e): pos{p}, ext{e} {}
        explicit Rectangle(Point &&p, Extents &&e): pos{p}, ext{e} {}
        explicit Rectangle(Position x, Position y, Length w, Length h): pos{x, y}, ext{w, h} {}

        auto width () const { return ext.w; }
        auto height() const { return ext.h; }

        auto x1() const { return pos.x; }
        auto x2() const { return pos.x + ext.w; }
        auto y1() const { return pos.y; }
        auto y2() const { return pos.y + ext.h; }

        auto& operator += (const Point &p) { pos += p; return *this; }

        auto operator +(const Point &p) -> Rectangle;

        auto inflate(Width x, Width y) -> Rectangle &;

        bool contains(const Point &p) const;

        bool fully_contains(const Rectangle &r) const;

        bool contains_full_width_of(const Rectangle &r) const;

        bool contains_full_height_of(const Rectangle &r) const;

        bool intersects_vertically_with(const Rectangle &r) const;
    };

}