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
#include "./Box.hpp"
#include "./Container_base.hpp"

namespace cppgui {

    // Container base class: descended from Widget

    enum class Layout_type { none, 
        header_content, 
        content_footer, 
        head_content,           // horizontal: fixed-width head, followed by stretching content TODO: better name! 
        content_tail,           // opposite of head_content
        stack,
        end
    };

    // Forward-declarations

    template <class Config, bool With_layout, class Parent> struct Container__Layouter;

    // Main class

    /** This generic container class exists primarily to combine the functionalities of 
        Abstract_container with those of Widget. At the present (2016-05-03), it also offers 
        a choice of layouting algorithms, but that is bad design and will be removed.
     */
    template <class Config, bool With_layout>
    class Container: 
        public Container__Layouter<Config, With_layout,
            Container_base<Config, With_layout> >
    {
    public:

        void compute_view_from_data() override
        {
            compute_child_views();

            Widget_t::compute_view_from_data();
        }

    protected:
        //auto paper_margin() -> unsigned int { return 2; } // TODO: remove (or move to Stack<>)
    };

    // Layouter aspect

    template <class Config, class Parent>
    struct Container__Layouter<Config, true, Parent>: 
        public Box__Layouter<Config, true, Parent >
    {
        using Widget_t = typename Widget<Config, true>;
        class Container_t: public Container<Config, true> { friend struct Container__Layouter; };

        // Layout contract

        void init_layout() override
        {
            for (auto child: p()->children())
            {
                child->init_layout();
            }
        }
        auto get_minimal_size() -> Extents override
        {
            // TODO: use polymorphic delegate class 

            Extents result;

            if (_layout_type == Layout_type::none)
            {            
            }
            else if (_layout_type == Layout_type::header_content || _layout_type == Layout_type::content_footer)
            {
                for(auto child: p()->children())
                {
                    auto min_sz  = child->get_minimal_size();

                    // Accumulate minimal, preferred height
                    result.h  += min_sz.h;

                    // Use greatest minimal width
                    if (min_sz .w > result.w) result.w = min_sz .w;
                }
            }
            else if (_layout_type == Layout_type::content_tail)
            {
                result.w = 0, result.h = 0;

                for(auto child: p()->children())
                {
                    auto min_sz = child->get_minimal_size();

                    // Accumulate minimal width
                    result.w += min_sz.w;

                    // Use greatest minimal height
                    if (min_sz.h > result.h) result.h = min_sz.h;
                }

                result.w += (p()->children().size() - 1) * _spacing;
            }
            else if (_layout_type == Layout_type::stack)
            {
                result.w = 0, result.h = 0;

                for(auto child: p()->children())
                {
                    auto min_sz = child->get_minimal_size();

                    // Use greatest minimal width
                    if (min_sz.w > result.w) result.w = min_sz.w;

                    // Add heights
                    result.h += min_sz.h;
                }

                result.h += (p()->children().size() - 1) * _spacing;
            }
            else {
                assert(false); 
            }

            // Add borders
            result.h += _padding[0] + _padding[2];
            result.w += _padding[3] + _padding[1];

            return result;
        }
        auto get_preferred_size() -> Extents override
        {
            // TODO: use polymorphic delegate class 

            Extents result;

            if (_layout_type == Layout_type::none)
            {
            }
            else if (_layout_type == Layout_type::header_content || _layout_type == Layout_type::content_footer)
            {
                for(auto child: p()->children())
                {
                    auto pref_sz = child->get_preferred_size(); 

                    // Accumulate minimal, preferred height
                    result.h += pref_sz.h;

                    // Use greatest minimal width
                    if (pref_sz.w > result.w) result.w = pref_sz.w;
                }
            }
            else if (_layout_type == Layout_type::content_tail)
            {
                result.w = 0, result.h = 0;

                for(auto child: p()->children())
                {
                    auto min_sz = child->get_preferred_size();

                    // Accumulate minimal width
                    result.w += min_sz.w;

                    // Use greatest preferred height
                    if (min_sz.h > result.h) result.h = min_sz.h;
                }

                //result.h += _padding[0] + _padding[2];
                result.w += (p()->children().size() - 1) * _spacing; // + _padding[3] + _padding[1];
            }
            else if (_layout_type == Layout_type::stack)
            {
                result.w = 0, result.h = 0;

                for(auto child: p()->children())
                {
                    auto min_sz = child->get_preferred_size();

                    // Use greatest preferred width
                    if (min_sz.w > result.w) result.w = min_sz.w;

                    // Add heights
                    result.h += min_sz.h;
                }

                result.h += (p()->children().size() - 1) * _spacing;

            }
            else {
                assert(false); 
            }

            // Add borders
            result.h += _padding[0] + _padding[2];
            result.w += _padding[3] + _padding[1];

            return result;
        }
        void layout() override
        {
            //p()->layout_children();

            // TODO: use polymorphic delegate class 

            if (_layout_type == Layout_type::none) 
            {
                for (auto child: p()->children()) child->layout();
            }
            else if (_layout_type == Layout_type::header_content)
            {
                auto ext = p()->extents();

                Widget_t *header  = p()->children()[0];
                Widget_t *content = p()->children()[1];

                Length h_rem = ext.h - _padding[0] - _padding[2];
                Length h;
                Position y = _padding[0];

                h = header->get_minimal_size().h;

                header->set_position({ (Position) _padding[3], y });
                header->set_extents ({ ext.w - _padding[3] - _padding[2], h });
                y += (Position) h;
                h_rem -= h;

                content->set_position({ (Position) _padding[3], y });
                content->set_extents ({ ext.w - _padding[3] - _padding[1], h_rem });

                for (auto child : p()->children()) child->layout();
            }
            else if (_layout_type == Layout_type::content_footer)
            {
                auto ext = p()->extents();

                Widget_t *content = p()->children()[0];
                Widget_t *footer  = p()->children()[1];

                Length h_rem = ext.h - _padding[0] - _padding[2]; // "remaining" height
                Length h;
                Position y = (Position) ext.h - _padding[2];

                h = footer->get_minimal_size().h;

                y -= static_cast<Position>(h);
                footer->set_position({ (Position) _padding[3], y });
                footer->set_extents ({ ext.w - _padding[3] - _padding[2], h });
                h_rem -= h;

                content->set_position({ (Position) _padding[3], (Position) _padding[0] });
                content->set_extents ({ ext.w - _padding[3] - _padding[2], h_rem });

                for (auto child : p()->children()) child->layout();
            }
            else if (_layout_type == Layout_type::content_tail)
            {
                auto ext = p()->extents();

                Widget_t *content = p()->children()[0];
                Widget_t *tail    = p()->children()[1];

                Length w_rem = ext.w - _padding[1] - _padding[3]; // "remaining" width
                Length w;
                Position x = (Position) ext.w - _padding[1];

                w = tail->get_minimal_size().w;

                x -= (Position) w;
                tail->set_position({ x, (Position) _padding[0] });
                tail->set_extents ({ w, ext.h - _padding[0] - _padding[2] });
                w_rem -= w;

                w_rem -= _spacing;

                content->set_position({ (Position) _padding[3], (Position) _padding[0] });
                content->set_extents ({ w_rem, ext.h - _padding[0] - _padding[2] });

                for (auto child : p()->children()) child->layout();
            }
            else if (_layout_type == Layout_type::stack)
            {
                auto ext = p()->extents();

                Position y = 0; //_padding[0];
                Position x = 0; //_padding[3];

                for (auto child: p()->children())
                {
                    child->set_position({ x, y });
                    child->set_extents({ ext.w, child->get_minimal_size().h });

                    child->layout();

                    y += child->extents().h + _spacing;
                }
            }
            else {
                assert(false);
            }
        }

        // Specific interface

        void set_layout_type(Layout_type type ) { _layout_type = type; }

        void insert_child(Widget_t *) // TODO: find a better name OR support insertion index
        {
            p()->add_child(child);
            layout();
        }
        void drop_child(Widget_t *)
        {
            p()->remove_child(child);
            layout();
        }

        void set_spacing(Length spacing) { _spacing = spacing; }

    private:
        auto p() { return static_cast<Container_t*>(static_cast<Container<Config, true>*>(this)); }

        Layout_type     _layout_type = Layout_type::none;
        Length          _spacing = 0;
    };

    // Nil implementation of the Layouter aspect

    template <class Config, class Parent>
    struct Container__Layouter<Config, false, Parent> 
    {
        // void recalc_sizes() {}
    };
        
} // ns cppgui

#define CPPGUI_INSTANTIATE_CONTAINER(Config, With_layout) \
    template cppgui::Container<Config, With_layout>;
