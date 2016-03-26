#pragma once

#include "./Widget.hpp"
#include "./Container.hpp"
#include "Root_widget.hpp"

namespace cppgui {

    // Abstract_widget<> --------------------------------------------

    template <class Config, bool With_layout>
    inline void Abstract_widget<Config, With_layout>::set_position(const Position &pos)
    {
        _rect.pos = pos;
    }

    template <class Config, bool With_layout>
    inline void Abstract_widget<Config, With_layout>::set_extents(const Extents &ext)
    {
        _rect.ext = ext;
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::mouse_click(const Position &, int button, int count)
    {
        if (button == 1 && count == 1)
        {
            // TODO
        }
    }

#ifdef NOT_DEFINED

    template <class Config, bool With_layout>
    inline void Abstract_widget<Config, With_layout>::cleanup_render_resources(Renderer *r)
    {
        Config::Color_mapper::release_all_resources(r);
        Config::Font_mapper ::release_all_resources(r);
    }

#endif

    template <class Config, bool With_layout>
    inline auto Abstract_widget<Config, With_layout>::rgba_to_native(Renderer *r, const Color &color) -> Native_color
    {
        return get_resource(r, color);
    }

    template <class Config, bool With_layout>
    inline void Abstract_widget<Config, With_layout>::fill(Renderer *r, const Position &offs, const Native_color &color)
    {
        auto b{ rectangle() };

        r->fill_rect(offs.x + b.pos.x, offs.y + b.pos.y, b.ext.w, b.ext.h, color);
    }

    template<class Config, bool With_layout>
    auto Abstract_widget<Config, With_layout>::convert_position_to_inner(const Position &pos) -> Position
    {
        // TODO: subtract border and padding
        return pos;
    }

    template<class Config, bool With_layout>
    auto Abstract_widget<Config, With_layout>::advance_to_glyph_at(const Rasterized_font *font, const std::u32string &text, 
        size_t from, size_t to, Position &pos) -> const Glyph_control_box *
    {
        assert(to >= from);

        const Glyph_control_box *cbox = nullptr;

        for (auto i = from; ; i++)
        {
            cbox = & font->lookup_glyph(0, text[i])->cbox;
            if (i == to) break;
            pos.x += cbox->adv_x;
            pos.y += cbox->adv_y;
        }

        return cbox;
    }

    // Default_widget_updater<> aspect ------------------------------

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    inline void Default_widget_updater<Config, With_layout>::Aspect<Aspect_parent>::invalidate()
    {
        //auto c = static_cast<Abstract_container<Config, With_layout>*>(this->container());
        //c->child_invalidated(static_cast<Widget_t*>(this));
        auto c = this->container();
        c->child_invalidated(static_cast<Widget_t*>(this));
    }

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    inline void Default_widget_updater<Config, With_layout>::Aspect<Aspect_parent>::added_to_container(Abstract_container_t *cont)
    {
        _container = cont;
    }

    // Widget<> implementation --------------------------------------

    template<class Config, bool With_layouting>
    void Widget<Config, With_layouting>::on_click(Click_handler handler)
    {
        assert(!_click_hndlr);
        _click_hndlr = handler;
    }

    template<class Config, bool With_layout>
    inline void Widget<Config, With_layout>::mouse_enter()
    {
        _hovered = true;
        invalidate();
    }

    template<class Config, bool With_layout>
    inline void Widget<Config, With_layout>::mouse_exit()
    {
        _hovered = false;
        invalidate();
    }

    template<class Config, bool With_layouting>
    void Widget<Config, With_layouting>::mouse_click(const Position &pos, int button, int count)
    {
        if (_click_hndlr) _click_hndlr(pos, button, count);
    }

    template<class Config, bool With_layout>
    void Widget<Config, With_layout>::init()
    {
        this->init_layout();
        this->init();
    }

} // ns cppgui