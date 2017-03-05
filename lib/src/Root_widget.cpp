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

#include <iostream>

#include <cppgui/Root_widget.hpp>


namespace cppgui {

    /*
    template<class Config, bool With_layout>
    Root_widget<Config, With_layout>::Root_widget(Canvas_t *cv)
    {
        _canvas = cv;
    }
    */

    /*
    template<class GUIConfig, bool WithLayout>
    void Root_widget<GUIConfig, WithLayout>::add_child(Widget<GUIConfig, WithLayout> *child)
    {
        _add_child(child);
        child->added_to_container(static_cast<Abstract_container_t*>(this));
    }
    */

    void Root_widget::set_canvas(Canvas *cv)
    {
        _canvas = cv;

        //_canvas->init(); // TODO: parameters ?
            // removed for now: init done by root_widget() user
    }

    void Root_widget::init()
    {
        init_child_resources();

        Widget::init();
    }

    void Root_widget::cleanup()
    {
        //TODO: cleanup_children_resources();

        //_canvas->cleanup();
            // see init()
    }

    void Root_widget::compute_view_from_data()
    {
        compute_child_views();

        Widget::compute_view_from_data();
    }

    /* void Root_widget::set_focus_to(Widget *widget)
    {
        switch_focused_child(widget);
        if (widget) widget->gained_focus(); // TODO: why is this not done in switch_focused_child() ?
    } */

    void Root_widget::push_cursor(Cursor_handle cursor)
    {
        _cursor_stack.push( Mouse_adapter::get_current_cursor() );

        Mouse_adapter::set_cursor(cursor);
    }

    void Root_widget::pop_cursor()
    {
        // All cursors but the top one must be freed after use
        if (_cursor_stack.size() > 1)
        {
            Mouse_adapter::free_cursor( Mouse_adapter::get_current_cursor() );
        }

        Mouse_adapter::set_cursor(_cursor_stack.top());

        _cursor_stack.pop();
    }

    void Root_widget::mouse_motion(const Point &pos)
    {
        lock();

        if (_mouse_holder)
        {
            _mouse_holder->mouse_motion(pos - _capture_offset);
        }

        // Note: there should be no harm in repeating a mouse motion event (contrary to e.g. mouse button events)
        //  (we need to repeat the event so as to support drag-and-drop, e.g. highlighting potential targets)
        // TODO: this will trigger mouse_exit() even if the mouse was captured - is that correct/acceptable ?
        container_mouse_motion(pos);

        unlock();
    }

    void Root_widget::mouse_button(const Point &pos, int button, Key_state state, Count clicks)
    {
        lock();

        #ifndef NOT_DEFINED

        if (_mouse_holder)
        {
            _mouse_holder->mouse_button(pos - _capture_offset, button, state, clicks);
        }
        else
        {
            this->container_mouse_button(pos, button, state, clicks); // Abstract_container TODO: better name ?
        }

        #else

        this->container_mouse_button(pos, button, state, clicks); // Abstract_container TODO: better name ?

        #endif

        unlock();
    }

    /*
    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::mouse_click(const Point &pos, int button, int count)
    {
        this->lock();
        container_mouse_click(pos, button, count);
        this->unlock();
    }
    */

    void Root_widget::mouse_wheel(const Vector &dir)
    {
        lock();
        container_mouse_wheel(dir);
        unlock();
    }

    void Root_widget::text_input(const char32_t *text, size_t size)
    {
        if (focused_child())
        {
            lock();
            focused_child()->text_input(text, size);
            unlock();
        }
    }

    void Root_widget::key_down(const Keycode &key)
    {
        lock();

        /*
        if (focused_child())
        {
            focused_child()->key_down(key);
        }
        */
        Container::key_down( key );

        // TODO: ensure this is called in case of an exception
        unlock();
    }

    void Root_widget::render()
    {
        render(_canvas, {0, 0});
    }

    void Root_widget::child_key_down(const Keycode &key)
    {
        if (!handle_key_down(key))
        {
            std::cerr << "Root_widget_base::child_key_down(): could not handle key event" << std::endl;
        }
    }

    void Root_widget::capture_mouse(Widget *holder)
    {
        // assert(!_mouse_holder);

        if (!_mouse_holder)
        {
            _mouse_holder = holder;

            // The mouse holder expects mouse positions as relative to its own origin,
            // so we get its absolute position here and store it as the "capture offset"
            Point offset = holder->position();
            for (auto cont = static_cast<Container*>(holder)->container(); cont != this; cont = static_cast<Container*>(cont)->container())
            {
                offset += static_cast<Container*>(cont)->position();
            }

            _capture_offset = offset;
        }
    }

    void Root_widget::release_mouse()
    {
        if (_mouse_holder)
        {
            assert(_mouse_holder);
            _mouse_holder = nullptr;
        }
    }

    auto Root_widget::container_absolute_position() -> Point
    {
        return position();
    }

    inline void Root_widget::render(Canvas *cv, const Point &offs)
    {
        auto pos = offs + this->position();

        if (_bkgnd_clr[3] > 0)
        {
            cv->clear(cv->rgba_to_native(_bkgnd_clr));
        }
        
        for (auto& child : this->children())
        {
            if (child->visible()) child->render(cv, pos);
        }
    }

    // Updater aspect -----------------------------------------------

    void Root_widget::invalidate()
    {
        _on_invalidated();
    }

    // Layouter aspect ----------------------------------------------

    void Root_widget::init_layout()
    {
        init_children_layout();
    }

    auto Root_widget::get_minimal_bounds() -> Bounding_box
    {
        return {}; // TODO: better solution ?
    }

    void Root_widget::set_bounds(const Point & p, const Bounding_box & b)
    {
        Container::set_bounds(p, b);
    }

    void Root_widget::insert_child(Widget *child, const Point & p, const Bounding_box & b)
    {
        add_child(child);

        child->init_layout();
        child->set_bounds(p, b);
        child->init();

        invalidate();
    }

    void Root_widget::drop_child(Widget *child)
    {
        if (contains_widget( mouse_holder() ))
        {
            release_mouse();
        }

        remove_child(child); 

        invalidate();
    }

} // ns cppgui

