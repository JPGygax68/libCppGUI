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

#include "./Widget.hpp"

namespace cppgui {

    template <class Config, bool With_layout, class Parent> struct Abstract_container__Layouter;

    /** Container functionality (ability to contain Widgets).
    */
    template <class Config, bool With_layout>
    class Abstract_container: public Config::template Abstract_container_Container_updater<Nil_struct>,
        public Abstract_container__Layouter<Config, With_layout, Nil_struct>
    {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Keyboard = typename Config::Keyboard;
        using Keycode  = typename Keyboard::Keycode;

        Abstract_container(): _hovered_child { nullptr } {}

        void set_initial_focus(Widget_t *)
        {
            _focused_child = child;
        }

        auto& children() { return _children; }

        /** Called when a key_down event could not be handled by the child it was sent to
            (from container_key_down()) and needs to "bubble" back up.
         */
        virtual void child_key_down(const Keycode &) = 0;

        virtual bool container_has_focus() = 0;     

        virtual void container_take_focus(Widget_t *child)
        {
            if (child != _focused_child)
            {
                if (_focused_child)
                {
                    _focused_child->loosing_focus(); // TODO: support veto-ing loss of focus ?
                }

                _focused_child = child;
            }
        }
        
        auto focused_child() -> Widget_t * { return _focused_child; }

        auto first_child() -> Widget_t * { assert(!_children.empty()); return _children.front(); }
        auto last_child () -> Widget_t * { assert(!_children.empty()); return _children.back (); }

        auto child_index(Widget_t *child) -> Index
        {
            using namespace std;

            return distance(begin(_children), find(begin(_children), end(_children), child) );
        }

        template<class Pred> auto scan_children_forward (Index from, Pred pred) -> Index
        {
            for (auto i = from; i < (Index) _children.size(); i ++)
            {
                if (pred(_children[i])) return i;
            }

            return (Index) _children.size();
        }
        template<class Pred> auto scan_children_backward(Index from, Pred pred) -> Index
        {
            for (Index i = from; i >= 0; i--)
            {
                if (pred(_children[i])) return i;
            }

            return - 1;
        }

    protected:

        void add_child(Widget_t *child)
        {
            _children.push_back(child);
            child->added_to_container(this);
        }
        // TODO: should removal methods be moved to optional aspect ?
        void remove_child(Widget_t *child)
        {
            if (child == _hovered_child)
            {
                _hovered_child->mouse_exit();
                _hovered_child = nullptr;
            }

            if (child == _focused_child)
            {
                _focused_child = nullptr;
            }

            child->removed_from_container(this);

            auto it = std::find(std::begin(_children), std::end(_children), child);
            assert(it != std::end(_children));
            _children.erase(it);
        }
        
        void remove_all_children()
        {
            for (auto child: _children)
            {
                remove_child(child);
            }
        }

        auto child_at(const Point &pos) -> Widget_t *
        {
            auto child = std::find_if(std::begin(_children), std::end(_children), [&](auto child) { 
                return child->visible() && child->rectangle().contains(pos); 
            });

            return child != std::end(_children) ? *child : nullptr;
        }

        void init_child_resources()
        {
            for (auto child : children())
            {
                child->init();
            }
        }
        
        void compute_child_views()
        {
            for (auto& child : children())
            {
                child->compute_view_from_data();
            }
        }

        void render_children(Canvas_t *canvas, const Point &offs)
        {
            for (auto& child : children())
            {
                if (child->visible()) 
                {
                    child->render(canvas, offs);
                }
            }
        }

        /** The container_xxxx() methods are intended as "delegate" event handlers, to be 
            called from "real" containers (i.e. descendants of Container<>).            
        */
        void container_mouse_motion(const Point &pos)
        {
            auto hovered = child_at(pos);

            if (hovered != _hovered_child)
            {
                /** TODO: this does not account for situations where the pointer enters a
                widget, then moves on to a zone on a higher Z-level (Z levels not being 
                implemented yet at the time of writing).
                */
                if (_hovered_child) _hovered_child->mouse_exit();
                if (hovered) hovered->mouse_enter();
                _hovered_child = hovered;
            }

            if (_hovered_child)
            {
                _hovered_child->mouse_motion(pos - _hovered_child->position());
            }
        }

        void container_mouse_button(const Point &pos, int button, Key_state state)
        {
            auto child = child_at(pos);

            if (child) child->mouse_button(pos - child->position(), button, state);
        }
        
        void container_mouse_click(const Point &pos, int button, int count)
        {
            auto child = child_at(pos);

            if (child) child->mouse_click(pos - child->position(), button, count);
        }
        
        void container_mouse_wheel(const Point &dist)
        {
            if (_hovered_child) _hovered_child->mouse_wheel(dist);
        }
        
        void container_mouse_exit()
        {
            // We must propagate the exit to any currently hovered child
            if (_hovered_child)
            {
                _hovered_child->mouse_exit();
                _hovered_child = nullptr;
            }
        }
        
        void container_text_input(const char32_t *text, size_t size)
        {
            if (_focused_child)
            {
                _focused_child->text_input(text, size);
            }
        }
        
        bool container_key_down(const Keycode &key)
        {
            if (_focused_child)
            {
                return _focused_child->handle_key_down(key);
            }
            else
                return true;
        }

        std::vector<Widget_t*> _children;
        Widget_t *_hovered_child = nullptr;
        Widget_t *_focused_child = nullptr;
    };

    template <class Config, bool With_layout> class Container;

    // Container_updater aspect

    // IMPORTANT! This is *different* from the "Updater" aspect, which belongs to Widgets!

    template <class Config, bool With_layout, class Parent>
    struct Default_Abstract_container_Container_updater: public Parent 
    {
        using Widget_t = Widget<Config, With_layout>;
        class Container_t: public Container<Config, With_layout> { friend struct Default_Abstract_container_Container_updater; };
        using Root_widget_t = Root_widget<Config, With_layout>;

        auto p() { return static_cast<Container_t*>(static_cast<Container<Config, With_layout>*>(this)); }

        virtual void child_invalidated(Widget_t *) = 0;

        virtual auto container_root_widget() -> Root_widget_t * = 0;
    };

    template <class Config, class Parent> 
    struct Abstract_container__Layouter<Config, true, Parent>: public Parent
    {
        class Container_t: public Container<Config, true> { friend struct Abstract_container__Layouter; };

        auto p() { return static_cast<Container_t*>(static_cast<Container<Config, true>*>(this)); }

        void init_children_layout()
        {
            for (auto child : p()->children())
            {
                child->init_layout();
            }
        }

        void layout_children()
        {
            // TODO: this algorithm, and the whole method, will probably become obsolete as real
            //  layouting gets implemented

            for (auto child : p()->children())
            {
                // EXPERIMENTAL: obtain minimum size and extend accordingly
                auto min_ext = child->get_minimal_size(), cur_ext = child->extents();
                if (child->extents().w == 0 && child->extents().h == 0)
                {
                    child->set_extents({ std::max(min_ext.w, cur_ext.w), std::max(min_ext.h, cur_ext.h) });
                }
                child->layout();
            }
        }
    };

} // ns cppgui
