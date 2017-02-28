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

#include <iostream> // TODO: REMOVE!!

#include <Widget.hpp>


#include <cppgui/Abstract_container.hpp>
#include <cppgui/Container_base.hpp>
#include <cppgui/Root_widget.hpp>

#include <cppgui/Canvas.hpp>

namespace cppgui {

    // Widget<> implementation --------------------------------------

    Widget::Widget():
        _focussable{ true }
    {
    }

    void Widget::set_background_color(const Color &color)
    {
        _bkgnd_clr = color;
    }

    auto Widget::background_color() const
    {
        // TODO: stylesheets!
        return _bkgnd_clr;
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

    void Widget::added_to_container(Abstract_container_t *cont)
    {
        _container = cont;
    }

    void Widget::removed_from_container(Abstract_container_t *)
    {
        _container = nullptr;
    }

    void Widget::take_focus()
    {
        pass_up_and_notify_focus();
    }

    auto Widget::root_widget() -> ::cppgui::Root_widget*
        { return _container->container_root_widget(); }

    void Widget::pass_up_and_notify_focus()
    {
        if (!has_focus())
        {
            if (_focussable)
            {
                container()->switch_focused_child(this);
                gained_focus();
            }
            else {
                container()->switch_focused_child(nullptr);
            }
        }
    }

    void Widget::gained_focus()
    {
        invalidate();
    }

    void Widget::loosing_focus()
    {
        invalidate();
    }

    bool Widget::has_focus()
    {
        return container()->container_has_focus() && container()->focused_child() == this;
    }

    bool Widget::is_first_child()
    {
        return container()->children().front() == this;
    }

    bool Widget::is_last_child()
    {
        return container()->children().back() == this;
    }

    void Widget::key_down(const Keycode &key)
    {
        // Fallback behavior: try to handle locally, else let it "bubble up"

        if (!handle_key_down(key))
        {
            // Let the event "bubble" up
            container()->child_key_down(key);
        }
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

    void Widget::mouse_button(const Point &pos, int button, Key_state state, Count clicks)
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
                mouse_click(pos, button, clicks);
            }
        }
    }

    void Widget::mouse_click(const Point &pos, int button, Count count)
    {
        if (_click_hndlr) _click_hndlr(pos, button, count);
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
        set_position({ position().x + delta, position().y });
        invalidate();
    }

    void Widget::shift_vertically(Position_delta delta)
    {
        // TODO: 
        set_position({ position().x, position().y + delta });
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

    inline auto Widget::button_face_color() -> Color
    {
        if (hovered())
        {
            return {0.8f, 0.8f, 0.8f, 1};
        }
        else {
            return {0.7f, 0.7f, 0.7f, 1};
        }
    }

    inline auto Widget::button_border_color() -> Color
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

} // ns cppgui