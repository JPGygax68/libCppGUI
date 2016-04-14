#pragma once

#include <cassert>

namespace cppgui {

    using Offset = int;
    using Length = unsigned int;
    using Width = Length;

    struct Extents;

    struct Position {

        Offset x, y;

        auto operator + (const Position &delta) const { return Position{ x + delta.x, y + delta.y }; }
        auto operator - (const Position &delta) const { return Position{ x - delta.x, y - delta.y }; }
        auto& operator += (const Position &delta) { x += delta.x, y += delta.y; return *this; }

    };

    using Position_delta = Position;

    using Extents_delta = Position; // TODO: define specialized class

    using Text_origin = Position;

    struct Extents {

        Length w, h;

        Extents(): w { 0 }, h { 0 } {}
        Extents(Length w_, Length h_): w { w_ }, h { h_ } {}
        Extents(const Extents &from): w { from.w }, h { from.h } {}
        Extents(Extents &&) = default;

        auto operator = (const Extents &) -> Extents & = default;
        auto operator = (Extents &&) -> Extents & = default;

        bool contains(const Position &pos) const { 

            return pos.x >= 0 && pos.y >= 0 
                && static_cast<Length>(pos.x) < w && static_cast<Length>(pos.y) < h; 
        }

        auto operator + (const Extents_delta &delta) const -> Extents {

            assert(static_cast<Offset>(w) + delta.x >= 0);
            assert(static_cast<Offset>(h) + delta.y >= 0);

            return { 
                static_cast<Length>(static_cast<Offset>(w) + delta.x), 
                static_cast<Length>(static_cast<Offset>(h) + delta.y) 
            };
        }

        auto operator - (const Extents_delta &delta) const -> Extents {

            assert(static_cast<Offset>(w) - delta.x >= 0);
            assert(static_cast<Offset>(h) - delta.y >= 0);

            return { 
                static_cast<Length>(static_cast<Offset>(w) - delta.x), 
                static_cast<Length>(static_cast<Offset>(h) - delta.y) 
            };
        }

        static auto between_points(const Position &from, const Position &to) -> Extents {

            return { static_cast<Length>(to.x - from.x), static_cast<Length>(to.y - from.y) };
        }
    };

    inline auto operator + (const Position &pos, const Extents &ext) -> Position { 

        return { pos.x + static_cast<Offset>(ext.w), pos.y + static_cast<Offset>(ext.h) }; 
    }

    inline auto operator - (const Position &pos, const Extents &ext) -> Position { 

        return { pos.x - static_cast<Offset>(ext.w), pos.y - static_cast<Offset>(ext.h) }; 
    }

    struct Rectangle {

        Position pos;
        Extents  ext;

        Rectangle(): pos {}, ext {} {}
        Rectangle(const Extents &ext_): pos {}, ext {ext_} {}
        Rectangle(const Rectangle &) = default;
        Rectangle(Rectangle &&) = default;
        Rectangle(Offset x, Offset y, Length w, Length h): pos { x, y}, ext { w, h } {}

        auto operator = (const Rectangle &) -> Rectangle & = default;
        auto operator = (Rectangle &&) -> Rectangle & = default;

        bool contains(const Position &p) const { return ext.contains(p - pos); }

        auto top_left() const { return pos; }
        auto top_right() const -> Position { return { pos.x + static_cast<Offset>(ext.w), pos.y }; }
        auto bottom_right() const -> Position { return { pos.x + static_cast<Offset>(ext.w), pos.y + static_cast<Offset>(ext.h) }; }
        auto bottom_left() const -> Position { return { pos.x, pos.y + static_cast<Offset>(ext.h) }; }
        auto left() const { return pos.x; }
        auto top() const { return pos.y; }
        auto right() const { return pos.y + static_cast<Offset>(ext.w); }
        auto bottom() const { return pos.y + static_cast<Offset>(ext.h); }
        auto width() const { return ext.w; }
        auto height() const { return ext.h; }

        auto offset(const Position &offs) const -> Rectangle { return { pos.x + offs.x, pos.y + offs.y, ext.w, ext.h }; }

        auto shrink(Length l) const -> Rectangle { return shrink({ l, l }); }

        auto shrink(const Extents &delta) const -> Rectangle { 

            return { 
                pos.x + static_cast<Offset>(delta.w), pos.y + static_cast<Offset>(delta.h), 
                ext.w - 2 * delta.w, ext.h - 2 * delta.h 
            }; 
        }

        auto grow(Length l) const -> Rectangle { return grow({ l, l }); }

        auto grow(const Extents &delta) const -> Rectangle { 

            return { 
                pos.x - static_cast<Offset>(delta.w), pos.y - static_cast<Offset>(delta.h), 
                ext.w + 2 * delta.w, ext.h + 2 * delta.h 
            }; 
        }

    };

} // ns cppgui

