#pragma once

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016 Hans-Peter Gygax

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <string>
#include <Widget.hpp>
#include <Container.hpp>
#include <Root_widget.hpp>
#include <iostream>


namespace cppgui {

    // Widget<> implementation --------------------------------------

    Widget::Widget()
    #ifdef DEBUG
        : _id{nullptr}
    #endif
    {
    }

    /* void Widget::set_bounds(Bbox_cref bounds)
    {
        _bounds = bounds;
    } */

    /*
    void Widget::set_background_color(const RGBA &color)
    {
        _bkgnd_clr = color;
    }

    auto Widget::background_color() const -> RGBA
    {
        // TODO: stylesheets!
        return _bkgnd_clr;
    }
    */

    void Widget::move_by(const Point &d)
    {
        _position += d;

        // NOTE: this does not invalidate - must be done by caller!
    }

    void Widget::shift_by(const Point &d)
    {
        // TODO: this can be optimized using renderer/platform specific features ("2d acceleration", textures, etc.)
        move_by(d);
        invalidate();
    }

    void Widget::on_click(Click_handler handler)
    {
        assert(!_click_hndlr);
        _click_hndlr = handler;
    }

    void Widget::set_visible(bool vis)
    {
        _visible = vis;
    }

    void Widget::added_to_container(Container_base *cont)
    {
        _container = cont;
    }

    void Widget::removed_from_container(Container_base *)
    {
        _container = nullptr;
    }

    void Widget::take_focus()
    {
        pass_up_and_notify_focus();
    }

    auto Widget::root_widget() -> Root_widget * 
    { 
        return _container->container_root_widget(); 
    }

    void Widget::pass_up_and_notify_focus()
    {
        if (!has_focus())
        {
            //if (focussable())
            //{
                container()->set_focus_on_child(this);
                //gained_focus();
            //}
            //else {
            //    container()->switch_focused_child(nullptr);
            //}
        }
    }

    void Widget::gained_focus()
    {
        std::cout << typeid(*this).name() << " gained focus" << std::endl;
        invalidate();
    }

    void Widget::loosing_focus()
    {
        invalidate();
    }

    bool Widget::has_focus()
    {
        return container()->has_focus() && container()->focused_child() == this;
    }

    bool Widget::is_first_child()
    {
        return container()->children().front() == this;
    }

    bool Widget::is_last_child()
    {
        return container()->children().back() == this;
    }

    bool Widget::mouse_motion(const Point &p)
    {
        return false;
    }

    bool Widget::key_down(const Keycode &key)
    {
        return false;
    }

    void Widget::mouse_enter()
    {
        //std::cout << "Widget::mouse_enter()" << std::endl;
        _hovered = true;
        invalidate();
    }

    void Widget::mouse_exit()
    {
        //std::cout << "Widget::mouse_exit()" << std::endl;
        _hovered = false;
        invalidate();
    }

    auto Widget::absolute_position() -> Point
    {
        assert( container() ); // must not query before inserted into widget tree

        return container()->container_absolute_position() + position();
    }

    bool Widget::contains_point(const Point & point)
    {
        return _bounds.is_point_inside(point - _position);
    }

    bool Widget::mouse_button(const Point &pos, int button, Key_state state, Count clicks)
    {
        if (state == Key_state::pressed)
        {
            take_focus();

            root_widget()->capture_mouse(this);
        }
        else if (state == Key_state::released)
        {
            auto holder = root_widget()->mouse_holder();

            // Must be done now because click handler may change UI structure
            root_widget()->release_mouse();

            if (button == 1 && holder == this)
            {
                return mouse_click(pos, button, clicks);
                    // -> if the click was consumed, we consider the mouse button event
                    //      to be consumed as well
            }
        }

        return false; // the event was not consumed
    }

    bool Widget::mouse_click(const Point &pos, int button, Count count)
    {
        if (_click_hndlr)
        {
            if (_click_hndlr(pos, button, count)) return true;
        }

        take_focus();

        return false;
    }

    bool Widget::mouse_wheel(const Vector &v)
    {
        return false;
    }

    bool Widget::text_input(const char32_t *, size_t)
    {
        return false;
    }

    void Widget::change_visible(bool state)
    {
        if (state != visible())
        {
            set_visible(state);
            invalidate();
        }
    }

    void Widget::shift_horizontally(Position_delta delta)
    {
        // TODO: 
        //set_position({ position().x + delta, position().y });
        _position.x += delta;
        invalidate();
    }

    void Widget::shift_vertically(Position_delta delta)
    {
        // TODO: 
        //set_position({ position().x, position().y + delta });
        _position.y += delta;
        invalidate();
    }

    void Widget::shift_up(Length length)
    {
        shift_vertically( - static_cast<Position_delta>(length) );
    }

    void Widget::shift_down(Length length)
    {
        shift_vertically( static_cast<Position_delta>(length) );
    }

    // Style

    inline auto Widget::button_face_color() -> RGBA
    {
        if (hovered())
        {
            return {0.8f, 0.8f, 0.8f, 1};
        }
        else {
            return {0.7f, 0.7f, 0.7f, 1};
        }
    }

    inline auto Widget::button_border_color() -> RGBA
    {
        if (hovered())
        {
            return {0.3f, 0.3f, 0.3f, 1};
        }
        else {
            return {0.1f, 0.1f, 0.1f, 1};
        }
    }

    auto Widget::button_border_width() -> int
    {
        if (/*is_default()*/ false) 
        {
            return 2;
        }
        else {
            return 1;
        }
    }

    void Widget::invalidate()
    {
        _container->child_invalidated(this);
    }

    void Widget::set_bounds(const Point &p, Bbox_cref b)
    {
        compute_layout(b);

        _position = p;
        _bounds = b;
    }

    /*
     * TODO: this function should be moved to the fonts namespace
     */
    auto Widget::advance_to_glyph_at(const Rasterized_font *font, const std::u32string &text, 
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

    auto Widget::visual_states() -> Widget_states
    {
        Widget_states states;

        if (disabled ()) states.set(Widget_state::disabled);
        if (hovered  ()) states.set(Widget_state::hovered);
        if (has_focus()) states.set(Widget_state::focused);

        return states;
    }


#ifdef NOT_DEFINED

    void Widget::fill_rect(Canvas *c, const Rectangle &rect, const Native_color &color)
    {
        c->fill_rect(rectrect.pos.x, rect.pos.y, rect.ext.w, rect.ext.h, color);
    }

    void Widget::fill_rect(Canvas *r, const Rectangle & rect, const Point & offs, const Native_color &color)
    {
        r->fill_rect(rect.pos.x + offs.x, rect.pos.y + offs.y, rect.ext.w, rect.ext.h, color);
    }

    void Widget::fill_rect(Canvas *r, const Point & pos, const Extents & ext, const Native_color &color)
    {
        r->fill_rect(pos.x, pos.y, ext.w, ext.h, color);
    }

    void Widget::fill(Canvas *r, const Point &offs, const Native_color &color)
    {
        fill_rect(r, rectangle(), offs, color);
    }

    #endif // NOT_DEFINED

    #ifndef CPPGUI_EXCLUDE_LAYOUT

    auto Widget::align_cultural_minor(Bbox_cref inner, Bbox_cref outer, Alignment align) -> Position_delta
    {
        #ifndef CPPGUI_CULTURE_WESTERN
        #error Non-western cultures not supported yet!
        #endif

        if (align == cultural_minor_start)
        {
            return 0;
        }
        if (align == cultural_minor_middle)
        {
            return (outer.width() - inner.width()) / 2; // + outer.x_min;
        }
        if (align == cultural_minor_end)
        {
            return outer.width() - inner.width();
        }

        assert(false);
        return 0;
    }

    #ifdef NOT_DEFINED

    // Layouter aspect ----------------------------------------------

    void Widget::set_rectangle(const Point &nw, const Point &se)
    {
        set_position(nw);
        set_extents(Extents::between_points(nw, se));
    }

    void Widget::set_rectangle_nw(const Point &pos, const Extents &ext)
    {
        set_position(pos);
        set_extents(ext);
    }

    void Widget::set_rectangle_ne(const Point &pos, const Extents &ext)
    {
        set_position({ pos.x - static_cast<Position>(ext.w), pos.y });
        set_extents(ext);
    }

    void Widget::set_rectangle_sw(const Point &pos, const Extents &ext)
    {
        set_position({ pos.x, pos.y - static_cast<Position>(ext.h) });
        set_extents(ext);
    }

    void Widget::set_rectangle_se(const Point &pos, const Extents &ext)
    {
        set_position({ pos.x - static_cast<Position>(ext.w), pos.y - static_cast<Position>(ext.h) });
        set_extents(ext);
    }

    #endif // NOT_DEFINED

    #endif // CPPGUI_EXCLUDE_LAYOUT

} // ns cppgui