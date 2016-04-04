#pragma once

#include <cassert>

#include <gpc/gui/renderer.hpp>

#include <gpc/fonts/rasterized_font.hpp>
#include <gpc/fonts/bounding_box.hpp>
#include <gpc/fonts/rasterized_glyph_cbox.hpp>

namespace cppgui {

    using Offset = int;
    using Length = unsigned int;

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
    };

    struct Rectangle {
        Position pos;
        Extents  ext;
        bool contains(const Position &p) const { return ext.contains(p - pos); }
        auto offset(const Position &offs) const -> Rectangle { return { pos.x + offs.x, pos.y + offs.y, ext.w, ext.h }; }
    };

    using Rasterized_font   = gpc::fonts::rasterized_font;
    using Font_size         = unsigned int;
    using Serialized_font   = std::pair<const uint8_t *, std::size_t>;

    using Text_bounding_box = gpc::fonts::bounding_box;
    using Glyph_control_box = gpc::fonts::rasterized_glyph_cbox;
    using Color             = gpc::gui::rgba_norm;

} // ns cppgui