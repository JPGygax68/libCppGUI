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

#include <array>
#include <cassert>

namespace cppgui {

    // 1D ----------------------------

    using Position = int;
    using Position_delta = Position;
    using Length = unsigned int;
    using Width = Length;

    /** 1-dimensional equivalent of Rectangle
    */
    struct Range {
        Position p;
        Length   l;
        void define(Position p1, Position p2) { 
            assert(p2 > p1);
            p = p1, l = static_cast<Length>(p2 - p1);
        }
        auto start() const { return p; }
        auto end  () const { return p + static_cast<Position>(l); }
    };

    // 2D ----------------------------

    struct Extents;

    struct Point {

        Position x, y;

        auto operator + (const Point &delta) const { return Point{ x + delta.x, y + delta.y }; }
        auto operator - (const Point &delta) const { return Point{ x - delta.x, y - delta.y }; }
        auto& operator += (const Point &delta) { x += delta.x, y += delta.y; return *this; }

    };

    using Vector = Point;

    using Extents_delta = Point; // TODO: define specialized class

    using Text_origin = Point;

    struct Extents {

        Length w, h;

        Extents(): w { 0 }, h { 0 } {}
        Extents(Length w_, Length h_): w { w_ }, h { h_ } {}
        Extents(const Extents &from): w { from.w }, h { from.h } {}
        Extents(Extents &&) = default;

        auto operator = (const Extents &) -> Extents & = default;
        auto operator = (Extents &&) -> Extents & = default;

        auto right_edge () const { return static_cast<Position>(w); }
        auto bottom_edge() const { return static_cast<Position>(h); }

        bool contains(const Point &pos) const { 

            return pos.x >= 0 && pos.y >= 0 
                && static_cast<Length>(pos.x) < w && static_cast<Length>(pos.y) < h; 
        }

        bool can_contain(const Extents &rect)
        {
            return rect.w < w && rect.h < h;
        }

        auto operator + (const Extents_delta &delta) const -> Extents {

            assert(static_cast<Position>(w) + delta.x >= 0);
            assert(static_cast<Position>(h) + delta.y >= 0);

            return { 
                static_cast<Length>(static_cast<Position>(w) + delta.x), 
                static_cast<Length>(static_cast<Position>(h) + delta.y) 
            };
        }

        auto operator - (const Extents_delta &delta) const -> Extents {

            assert(static_cast<Position>(w) - delta.x >= 0);
            assert(static_cast<Position>(h) - delta.y >= 0);

            return { 
                static_cast<Length>(static_cast<Position>(w) - delta.x), 
                static_cast<Length>(static_cast<Position>(h) - delta.y) 
            };
        }

        auto right () { return static_cast<Position>(w); }
        auto bottom() { return static_cast<Position>(h); }

        static auto between_points(const Point &from, const Point &to) -> Extents {

            return { static_cast<Length>(to.x - from.x), static_cast<Length>(to.y - from.y) };
        }
    };

    inline auto operator + (const Point &pos, const Extents &ext) -> Point { 

        return { pos.x + static_cast<Position>(ext.w), pos.y + static_cast<Position>(ext.h) }; 
    }

    inline auto operator - (const Point &pos, const Extents &ext) -> Point { 

        return { pos.x - static_cast<Position>(ext.w), pos.y - static_cast<Position>(ext.h) }; 
    }

    using Padding = std::array<Width, 4>;
    using Margins = Padding;

    struct Rectangle {

        Point pos;
        Extents  ext;

        Rectangle(): pos {}, ext {} {}
        Rectangle(const Point &pos_, const Extents &ext_): pos {pos_}, ext { ext_ } {}
        Rectangle(const Extents &ext_): pos {}, ext {ext_} {}
        Rectangle(const Rectangle &) = default;
        Rectangle(Rectangle &&) = default;
        Rectangle(Position x, Position y, Length w, Length h): pos { x, y}, ext { w, h } {}

        auto operator = (const Rectangle &) -> Rectangle & = default;
        auto operator = (Rectangle &&) -> Rectangle & = default;

        bool contains(const Point &p) const { return ext.contains(p - pos); }

        /** Access conveniences.
        */
        ///@{

        auto width () const { return ext.w; }
        auto height() const { return ext.h; }
        auto position() -> Point   & { return pos; }
        auto extents () -> Extents & { return ext; }

        ///@}

        /** Computation conveniences.
        */
        ///@{

        auto top_left() const 
        { 
            return pos; 
        }
        auto top_right() const -> Point
        { 
            return { pos.x + static_cast<Position>(ext.w), pos.y }; 
        }
        auto bottom_right() const -> Point 
        { 
            return { pos.x + static_cast<Position>(ext.w), pos.y + static_cast<Position>(ext.h) }; 
        }
        auto bottom_left() const -> Point 
        { 
            return { pos.x, pos.y + static_cast<Position>(ext.h) }; 
        }
        auto left() const 
        { 
            return pos.x; 
        }
        auto top() const 
        { 
            return pos.y; 
        }
        auto right() const 
        { 
            return pos.x + static_cast<Position>(ext.w); 
        }
        auto bottom() const 
        { 
            return pos.y + static_cast<Position>(ext.h); 
        }

        ///@}

        /** Adding a Point to a rectangle will move its position without changing its extents.
        */
        ///@{

        auto operator + (const Point &offs) const -> Rectangle { return { pos.x + offs.x, pos.y + offs.y, ext.w, ext.h }; }
        auto operator - (const Point &offs) const -> Rectangle { return { pos.x - offs.x, pos.y - offs.y, ext.w, ext.h }; }

        ///@}

        /* 
        auto shrink(Length l) const -> Rectangle { return shrink({ l, l }); }

        auto shrink(const Extents &delta) const -> Rectangle { 

        return { 
        pos.x + static_cast<Position>(delta.w), pos.y + static_cast<Position>(delta.h), 
        ext.w - 2 * delta.w, ext.h - 2 * delta.h 
        }; 
        }
        */

        /** shrink() methods: make rectangle smaller
        */
        ///@{

        void shrink(Length l) 
        { 
            shrink( Extents{ l, l } ); 
        }
        void shrink(const Extents &delta)
        { 
            pos.x += (Position_delta) delta.w, pos.y += (Position_delta) delta.h;
            ext.w -= 2 * delta.w, ext.h -= 2 * delta.h;
        }
        void shrink(const Padding &padding)
        {
            pos.x += static_cast<Position>(padding[3]), pos.y += static_cast<Position>(padding[0]);
            ext.w -= padding[3] + padding[1], ext.h -= padding[0] + padding[2];
        }

        ///@}

        /** operator - variants: equivalent to shrink(..) methods except that they return 
        a new rectangle instance, leaving the original unchanged.
        */
        ///@{

        auto operator - (Width w) const -> Rectangle
        {
            return { 
                pos.x + (Position_delta) w, pos.y + (Position_delta) w, 
                ext.w - 2 * w, ext.h - 2 * w
            }; 
        }
        auto operator - (const Extents &delta) const -> Rectangle 
        { 
            return { 
                pos.x + static_cast<Position>(delta.w), pos.y + static_cast<Position>(delta.h), 
                ext.w - 2 * delta.w, ext.h - 2 * delta.h 
            }; 
        }
        auto operator - (const Padding &padding) const -> Rectangle 
        { 
            return { 
                pos.x + static_cast<Position>(padding[3]), pos.y + static_cast<Position>(padding[0]), 
                ext.w - padding[3] - padding[1], ext.h - padding[0] - padding[2]
            }; 
        }

        ///@}

        /** grow(): make the rectangle bigger
        */
        //@{

        void grow(Width w) 
        { 
            grow( Extents{ w, w } ); 
        }
        void grow(const Extents &delta)
        { 
            pos.x -= static_cast<Position>(delta.w), pos.y -= static_cast<Position>(delta.h);
            ext.w += 2 * delta.w, ext.h += 2 * delta.h;
        }
        void grow(const Margins &margins)
        {
            pos.x -= static_cast<Position>(margins[3]), pos.y -= static_cast<Position>(margins[0]);
            ext.w += margins[3] + margins[1], ext.h += margins[0] + margins[2];
        }

        ///@}

        /** operator + variants: equivalent to grow(..) methods except that they return 
        a new rectangle instance, leaving the original unchanged.
        */
        ///@{

        auto operator + (Length l) const -> Rectangle 
        { 
            return *this + Extents{ l, l }; 
        }
        auto operator + (const Extents &delta) const -> Rectangle 
        { 
            return { 
                pos.x - static_cast<Position>(delta.w), pos.y - static_cast<Position>(delta.h), 
                ext.w + 2 * delta.w, ext.h + 2 * delta.h 
            }; 
        }
        auto operator + (const Padding &padding) const -> Rectangle 
        { 
            return { 
                pos.x - static_cast<Position>(padding[3]), pos.y - static_cast<Position>(padding[0]), 
                ext.w + padding[3] + padding[1], ext.h + padding[0] + padding[1]
            }; 
        }

        ///@}

    };

} // ns cppgui

namespace std {

    inline auto max(const cppgui::Extents &a, const cppgui::Extents &b) -> cppgui::Extents
    {
        return { max(a.w, b.w), max(a.h, b.h) };
    }

} // ns std

