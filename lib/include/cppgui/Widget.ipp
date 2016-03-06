#pragma once

#include "./Widget.hpp"

namespace cppgui {

    template<class Derived, class Renderer>
    inline void Widget<Derived, Renderer>::set_position(const Position &pos)
    {
        _rect.pos = pos;
    }

    template<class Derived, class Renderer>
    inline void Widget<Derived, Renderer>::set_extents(const Extents &ext)
    {
        _rect.ext = ext;
    }

    template<class Derived, class Renderer>
    auto Widget<Derived, Renderer>::rgba_to_native(const Rgba_norm &color) -> Native_color
    {
        return Renderer::rgba_to_native(color);
    }

    template<class Derived, class Renderer>
    void Widget<Derived, Renderer>::fill(Renderer *r, const Native_color &color)
    {
        auto b{ rectangle() };
        r->fill_rect(b.pos.x, b.pos.y, b.ext.w, b.ext.h, color);
    }

    template<class Derived, class Renderer>
    void Widget<Derived, Renderer>::update_layout()
    {
        Derived::Layouter::update(*static_cast<Derived*>(this));
    }

} // ns cppgui