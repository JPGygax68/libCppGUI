#pragma once

#include <cassert>

#include <gpc/gui/renderer.hpp>

#include <gpc/fonts/rasterized_font.hpp>
#include <gpc/fonts/bounding_box.hpp>
#include <gpc/fonts/rasterized_glyph_cbox.hpp>

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

    struct Extents {
        
        Length w, h;

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
        bool contains(const Position &p) const { return ext.contains(p - pos); }
        auto offset(const Position &offs) const -> Rectangle { return { pos.x + offs.x, pos.y + offs.y, ext.w, ext.h }; }
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
    };

    using Rasterized_font   = gpc::fonts::rasterized_font;
    using Font_size         = unsigned int;
    using Serialized_font   = std::pair<const uint8_t *, std::size_t>;

    using Text_bounding_box = gpc::fonts::bounding_box;
    using Glyph_control_box = gpc::fonts::rasterized_glyph_cbox;
    using Color             = gpc::gui::rgba_norm;
    using Mono_value        = gpc::gui::mono_norm;

    enum Alignment {
        none = 0,
        left, center, middle = center, right,
        top, bottom
        // TODO: additional values for culture-adaptive settings, or reuse the geometrical ones above ?
    };

    // TODO: use this everywhere!
    // TODO: rename to Simple_border ?
    // TODO: plural form ? (std::array<> for four cardinal directions)

    struct Border {
        Width       width;
        Color       color;
    };

} // ns cppgui

namespace std {

    inline auto max(const cppgui::Extents &ext1, const cppgui::Extents &ext2) -> cppgui::Extents {

        return { max(ext1.w, ext2.w), max(ext1.h, ext2.h) };
    }

} // ns std