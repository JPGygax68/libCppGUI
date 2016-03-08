#pragma once

#include "./Widget.hpp"

namespace cppgui {

    inline void Widget_base::set_position(const Position &pos)
    {
        _rect.pos = pos;
    }

    inline void Widget_base::set_extents(const Extents &ext)
    {
        _rect.ext = ext;
    }

    template<class Renderer>
    auto Widget<Renderer>::rgba_to_native(const Rgba_norm &color) -> Native_color
    {
        return Renderer::rgba_to_native(color);
    }

    template<class Renderer>
    void Widget<Renderer>::fill(Renderer *r, const Native_color &color)
    {
        auto b{ rectangle() };
        r->fill_rect(b.pos.x, b.pos.y, b.ext.w, b.ext.h, color);
    }

} // ns cppgui