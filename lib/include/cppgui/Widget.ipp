#pragma once

#include <iostream> // TODO: REMOVE!!

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
    inline void Abstract_widget<Config, With_layout>::cleanup_render_resources(Canvas_t *r)
    {
        Config::Color_mapper::release_all_resources(r);
        Config::Font_mapper ::release_all_resources(r);
    }

#endif

    template <class Config, bool With_layout>
    inline auto Abstract_widget<Config, With_layout>::rgba_to_native(Canvas_t *r, const Color &color) -> Native_color
    {
        return get_resource(r, color);
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::fill_rect(Canvas_t *r, const Rectangle &rect, const Native_color &color)
    {
        r->fill_rect(rect.pos.x, rect.pos.y, rect.ext.w, rect.ext.h, color);
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::fill_rect(Canvas_t * r, const Rectangle & rect, const Position & offs, const Native_color &color)
    {
        r->fill_rect(rect.pos.x + offs.x, rect.pos.y + offs.y, rect.ext.w, rect.ext.h, color);
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::fill_rect(Canvas_t * r, const Position & pos, const Extents & ext, const Native_color &color)
    {
        r->fill_rect(pos.x, pos.y, ext.w, ext.h, color);
    }

    template <class Config, bool With_layout>
    inline void Abstract_widget<Config, With_layout>::fill(Canvas_t *r, const Position &offs, const Native_color &color)
    {
        fill_rect(r, rectangle(), offs, color);

        //auto b{ rectangle() };
        //r->fill_rect(offs.x + b.pos.x, offs.y + b.pos.y, b.ext.w, b.ext.h, color);
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::draw_borders(Canvas_t *cv, const Rectangle &rect, const Position &offs, 
        unsigned int width, const Color &color)
    {
        draw_borders(cv, rect, offs, width, color, color, color, color);
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::draw_borders(Canvas_t *r, const Rectangle & rect, const Position & offs, 
        unsigned int width, const Color & top, const Color & right, const Color & bottom, const Color & left)
    {
        // TODO: this painting procedure does a "wrap" in clockwise fashion, without regard for corners
        fill_rect(r, offs + rect.pos + Position{(int) width, 0}, {rect.ext.w - width, width}, top);
        fill_rect(r, offs + rect.pos + Position{(int) (rect.ext.w - width), 0}, {width, rect.ext.h}, right);
        fill_rect(r, offs + rect.pos + Position{0, (int) (rect.ext.h - width)}, {rect.ext.w - width, width}, bottom);
        fill_rect(r, offs + rect.pos, {width, rect.ext.h}, left);
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

    // Default_Widget_Updater<> aspect ------------------------------

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    inline void Default_Widget_Updater<Config, With_layout>::Aspect<Aspect_parent>::invalidate()
    {
        //auto c = static_cast<Abstract_container<GUIConfig, With_layout>*>(this->container());
        //c->child_invalidated(static_cast<Widget_t*>(this));
        auto c = this->container();
        c->child_invalidated(static_cast<Widget_t*>(this));
    }

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    inline void Default_Widget_Updater<Config, With_layout>::Aspect<Aspect_parent>::added_to_container(Abstract_container_t *cont)
    {
        _container = cont;
    }

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    void Default_Widget_Updater<Config, With_layout>::Aspect<Aspect_parent>::removed_from_container(Abstract_container_t *cont)
    {
        _container = nullptr;
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
        std::cout << "Widget::mouse_enter()" << std::endl;
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

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    inline void Widget_layouter<Config, true>::Aspect<Aspect_parent>::set_padding(const std::initializer_list<Length> &padding)
    {
        std::copy(std::begin(padding), std::end(padding), std::begin(_padding));
    }

    template<class Config>
    template<class Aspect_parent>
    void Widget_layouter<Config, true>::Aspect<Aspect_parent>::compute_inner_rect()
    {
        auto ext = p()->extents();

        // TODO: adjust for border as well

        p()->_inner_rect = {
            static_cast<Offset>(_padding[3]), static_cast<Offset>(_padding[0]),
            ext.w - _padding[3] - _padding[1], ext.h - _padding[0] - _padding[2]
        };
    }

} // ns cppgui