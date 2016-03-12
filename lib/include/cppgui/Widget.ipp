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

    inline void Widget_base::mouse_enter()
    {
        _hovered = true;
        trigger_redraw();
    }

    inline void Widget_base::mouse_exit()
    {
        _hovered = false;
        trigger_redraw();
    }

    inline void Widget_base::trigger_redraw()
    {
        std::cerr << "Widget_base::trigger_redraw() not implemented yet" << std::endl;
    }

    template<class Config>
    auto Widget<Config>::rgba_to_native(Renderer *r, const Rgba_norm &color) -> Native_color
    {
        return map(r, color);
    }

    template<class Renderer>
    inline void Widget<Renderer>::fill(Renderer *r, const Native_color &color)
    {
        auto b{ rectangle() };
        r->fill_rect(b.pos.x, b.pos.y, b.ext.w, b.ext.h, color);
    }

} // ns cppgui