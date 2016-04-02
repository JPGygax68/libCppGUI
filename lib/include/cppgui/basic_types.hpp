#pragma once

#include <gpc/gui/renderer.hpp>

#include <gpc/fonts/rasterized_font.hpp>
#include <gpc/fonts/bounding_box.hpp>
#include <gpc/fonts/rasterized_glyph_cbox.hpp>

namespace cppgui {

    struct Position {
        int x, y;
        auto operator + (const Position &offs) const { return Position{ x + offs.x, y + offs.y }; }
        auto operator - (const Position &offs) const { return Position{ x - offs.x, y - offs.y }; }
        auto& operator += (const Position &offs) { x += offs.x, y += offs.y; return *this; }
    };

    struct Extents {
        unsigned int w, h;
        bool contains(const Position &pos) const { 
            return pos.x >= 0 && pos.y >= 0 
                && (unsigned int) pos.x < w && (unsigned int) pos.y < h; 
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
    using Text_bounding_box = gpc::fonts::bounding_box;
    using Glyph_control_box = gpc::fonts::rasterized_glyph_cbox;
    using Color             = gpc::gui::rgba_norm;

} // ns cppgui