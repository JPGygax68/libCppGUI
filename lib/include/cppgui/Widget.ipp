#pragma once

#include <iostream> // TODO: REMOVE!!

#include "./Widget.hpp"
#include "./Container.hpp"
#include "Root_widget.hpp"

namespace cppgui {

    // Abstract_widget<> --------------------------------------------

    template <class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::set_position(const Point &pos)
    {
        _rect.pos = pos;
    }

    template <class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::set_extents(const Extents &ext)
    {
        _rect.ext = ext;
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::mouse_click(const Point &, int button, int count)
    {
        if (button == 1 && count == 1)
        {
            // TODO
        }
    }

    template <class Config, bool With_layout>
    auto Abstract_widget<Config, With_layout>::rgba_to_native(Canvas_t *r, const Color &color) -> Native_color
    {
        return get_resource(r, color);
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::fill_rect(Canvas_t *r, const Rectangle &rect, const Native_color &color)
    {
        r->fill_rect(rect.pos.x, rect.pos.y, rect.ext.w, rect.ext.h, color);
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::fill_rect(Canvas_t * r, const Rectangle & rect, const Point & offs, const Native_color &color)
    {
        r->fill_rect(rect.pos.x + offs.x, rect.pos.y + offs.y, rect.ext.w, rect.ext.h, color);
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::fill_rect(Canvas_t * r, const Point & pos, const Extents & ext, const Native_color &color)
    {
        r->fill_rect(pos.x, pos.y, ext.w, ext.h, color);
    }

    template <class Config, bool With_layout>
    inline void Abstract_widget<Config, With_layout>::fill(Canvas_t *r, const Point &offs, const Native_color &color)
    {
        fill_rect(r, rectangle(), offs, color);

        //auto b{ rectangle() };
        //r->fill_rect(offs.x + b.pos.x, offs.y + b.pos.y, b.ext.w, b.ext.h, color);
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::draw_borders(Canvas_t *cv, const Point & offs, Width width, const Color &color)
    {
        draw_borders(cv, rectangle(), offs, width, color);
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::draw_borders(Canvas_t *cv, const Rectangle &rect, const Point &offs, 
        Width width, const Color &color)
    {
        draw_borders(cv, rect, offs, width, color, color, color, color);
    }

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::draw_borders(Canvas_t *r, const Rectangle & rect, const Point & offs, 
        Width width, const Color & top, const Color & right, const Color & bottom, const Color & left)
    {
        // TODO: this painting procedure does a "wrap" in clockwise fashion, without regard for corners
        fill_rect(r, offs + rect.pos + Point{(int) width, 0}, {rect.ext.w - width, width}, top);
        fill_rect(r, offs + rect.pos + Point{(int) (rect.ext.w - width), 0}, {width, rect.ext.h}, right);
        fill_rect(r, offs + rect.pos + Point{0, (int) (rect.ext.h - width)}, {rect.ext.w - width, width}, bottom);
        fill_rect(r, offs + rect.pos, {width, rect.ext.h}, left);
    }

    template<class Config, bool With_layout>
    auto Abstract_widget<Config, With_layout>::convert_position_to_inner(const Point &pos) -> Point
    {
        // TODO: subtract border and padding
        return pos;
    }

    template<class Config, bool With_layout>
    auto Abstract_widget<Config, With_layout>::advance_to_glyph_at(const Rasterized_font *font, const std::u32string &text, 
        size_t from, size_t to, Point &pos) -> const Glyph_control_box *
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

    template<class Config, bool With_layout>
    void Abstract_widget<Config, With_layout>::draw_stippled_inner_rect(Canvas_t *cnv, const Rectangle &, const Point & offs)
    {
        //cnv->draw_stipp()
    }

    // Default__Widget__Updater<> aspect ------------------------------

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    inline void Default__Widget__Updater<Config, With_layout>::Aspect<Aspect_parent>::invalidate()
    {
        //auto c = static_cast<Abstract_container<GUIConfig, With_layout>*>(this->container());
        //c->child_invalidated(static_cast<Widget_t*>(this));
        auto c = p()->container();
        c->child_invalidated(static_cast<Widget_t*>(this));
    }

    // Widget<> implementation --------------------------------------

    template<class Config, bool With_layout>
    void Widget<Config, With_layout>::set_background_color(const Color &color)
    {
        _bkgnd_clr = color;
    }

    template<class Config, bool With_layout>
    auto Widget<Config, With_layout>::background_color() const
    {
        // TODO: stylesheets!
        return _bkgnd_clr;
    }

    template<class Config, bool With_layouting>
    void Widget<Config, With_layouting>::on_click(Click_handler handler)
    {
        assert(!_click_hndlr);
        _click_hndlr = handler;
    }

    template<class Config, bool With_layout>
    void Widget<Config, With_layout>::set_visible(bool vis)
    {
        _visible = vis;
    }

    template<class Config, bool With_layout>
    inline void Widget<Config, With_layout>::added_to_container(Abstract_container_t *cont)
    {
        _container = cont;
    }

    template<class Config, bool With_layout>
    void Widget<Config, With_layout>::removed_from_container(Abstract_container_t *)
    {
        _container = nullptr;
    }

    template<class Config, bool With_layout>
    bool Widget<Config, With_layout>::take_focus()
    {
        if (!_focussable)
        {
            return false;
        }
        else
        {
            gained_focus();
            container()->child_has_obtained_focus(this);
            return true;
        }
    }

    template<class Config, bool With_layout>
    void Widget<Config, With_layout>::gained_focus()
    {
        invalidate();
    }

    template<class Config, bool With_layout>
    void Widget<Config, With_layout>::loosing_focus()
    {
        // container()->child_is_loosing_focus(); // TODO ?
        invalidate();
    }

    template<class Config, bool With_layout>
    void Widget<Config, With_layout>::key_down(const Keycode &key)
    {
        // Fallback behavior: try to handle locally, else let it "bubble up"

        if (!handle_key_down(key))
        {
            // Let the event "bubble" up
            container()->child_key_down(key);
        }
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
        std::cout << "Widget::mouse_exit()" << std::endl;
        _hovered = false;
        invalidate();
    }

    template<class Config, bool With_layouting>
    void Widget<Config, With_layouting>::mouse_click(const Point &pos, int button, int count)
    {
        if (_click_hndlr) _click_hndlr(pos, button, count);
    }

    template<class Config, bool With_layout>
    void Widget<Config, With_layout>::change_visible(bool state)
    {
        if (state != visible())
        {
            set_visible(state);
            invalidate();
        }
    }

    template<class Config, bool With_layout>
    void Widget<Config, With_layout>::shift_horizontally(Position_delta delta)
    {
        // TODO: 
        set_position({ position().x + delta, position().y });
        invalidate();
    }

    template<class Config, bool With_layout>
    void Widget<Config, With_layout>::shift_vertically(Position_delta delta)
    {
        // TODO: 
        set_position({ position().x, position().y + delta });
        invalidate();
    }

    template<class Config, bool With_layout>
    void Widget<Config, With_layout>::shift_up(Length length)
    {
        shift_vertically( - static_cast<Position_delta>(length) );
    }

    template<class Config, bool With_layout>
    void Widget<Config, With_layout>::shift_down(Length length)
    {
        shift_vertically( static_cast<Position_delta>(length) );
    }

    // Style

    template<class Config, bool With_layout>
    inline auto Widget<Config, With_layout>::button_face_color() -> Color
    {
        if (hovered())
        {
            return {0.8f, 0.8f, 0.8f, 1};
        }
        else {
            return {0.7f, 0.7f, 0.7f, 1};
        }
    }

    template<class Config, bool With_layout>
    inline auto Widget<Config, With_layout>::button_border_color() -> Color
    {
        if (hovered())
        {
            return {0.3f, 0.3f, 0.3f, 1};
        }
        else {
            return {0.1f, 0.1f, 0.1f, 1};
        }
    }

    template<class Config, bool With_layout>
    inline auto Widget<Config, With_layout>::button_border_width() -> int
    {
        if (/*is_default()*/ false) 
        {
            return 2;
        }
        else {
            return 1;
        }
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    void Widget__Layouter<Config, true>::Aspect<Aspect_parent>::set_padding(Width w)
    {
        set_padding({w, w, w, w});
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Widget__Layouter<Config, true>::Aspect<Aspect_parent>::set_padding(const std::initializer_list<Length> &padding)
    {
        std::copy(std::begin(padding), std::end(padding), std::begin(_padding));
    }

    template<class Config>
    template<class Aspect_parent>
    void Widget__Layouter<Config, true>::Aspect<Aspect_parent>::set_rectangle(const Point &nw, const Point &se)
    {
        p()->set_position(nw);
        p()->set_extents(Extents::between_points(nw, se));
    }

    template<class Config>
    template<class Aspect_parent>
    void Widget__Layouter<Config, true>::Aspect<Aspect_parent>::set_rectangle_nw(const Point &pos, const Extents &ext)
    {
        p()->set_position(pos);
        p()->set_extents(ext);
    }

    template<class Config>
    template<class Aspect_parent>
    void Widget__Layouter<Config, true>::Aspect<Aspect_parent>::set_rectangle_ne(const Point &pos, const Extents &ext)
    {
        p()->set_position({ pos.x - static_cast<Position>(ext.w), pos.y });
        p()->set_extents(ext);
    }

    template<class Config>
    template<class Aspect_parent>
    void Widget__Layouter<Config, true>::Aspect<Aspect_parent>::set_rectangle_sw(const Point &pos, const Extents &ext)
    {
        p()->set_position({ pos.x, pos.y - static_cast<Position>(ext.h) });
        p()->set_extents(ext);
    }

    template<class Config>
    template<class Aspect_parent>
    void Widget__Layouter<Config, true>::Aspect<Aspect_parent>::set_rectangle_se(const Point &pos, const Extents &ext)
    {
        p()->set_position({ pos.x - static_cast<Position>(ext.w), pos.y - static_cast<Position>(ext.h) });
        p()->set_extents(ext);
    }

    template<class Config>
    template<class Aspect_parent>
    void Widget__Layouter<Config, true>::Aspect<Aspect_parent>::compute_inner_rect()
    {
        auto ext = p()->extents();

        // TODO: adjust for border as well

        p()->_inner_rect = {
            static_cast<Position>(_padding[3]), static_cast<Position>(_padding[0]),
            ext.w - _padding[3] - _padding[1], ext.h - _padding[0] - _padding[2]
        };
    }

} // ns cppgui