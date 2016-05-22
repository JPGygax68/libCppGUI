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

#include "./Scrollbox.hpp"

namespace cppgui {

    // Listbox_base ======================================================

    // Forward declarations

    template<class Config>
    struct _listbox {
    
        template<bool With_layout, class Pane, class Parent> struct Layouter;

        /** Customizable base class
         */
        template<bool With_layout, class Pane>
        class Base: public Layouter<With_layout, Pane, Scrollbox<Config, With_layout, Pane> >
        {
        public:
            using Widget_t = Widget<Config, With_layout>;
            using Container_t = Container<Config, With_layout>;
            //using Canvas_t = typename Canvas<typename Config::Renderer>;
            using Pane_t = Pane;
            using Layoutable_widget_t = Widget<Config, true>; // THIS IS SPECIAL - only layoutable widgets can be added at runtime

            Base()
            {
                _content_pane.set_layout_type(Layout_type::stack); // TODO: will need adapting

                set_content_pane(&_content_pane);
            }

            // TODO: a variant that does not require the item to be layoutable
            void add_item(Layoutable_widget_t *item)
            {
                _content_pane.add_child( item );
            }

            auto selected_item() -> Index
            {
                return _content_pane.child_index( focused_child() );
            }
        
            auto item_index(Widget_t *child) -> Index
            {
                return _content_pane.child_index( child );
            }

            void ensure_item_in_view(int item_index)
            {
                auto item = _content_pane.children()[item_index];

                auto y1_item = item->rectangle().top(), y2_item = item->rectangle().bottom();

                // Item not already fully visible ?
                if (!(y1_item >= 0 && y2_item <= _content_rect.ext.bottom()))
                {
                    Position_delta dy = 0;
                    Index first = 0;

                    for (auto i = 0U; i < _content_pane.children().size(); i++)
                    {
                        if (y2_item + dy <= _content_rect.ext.bottom()) break;

                        dy = - _content_pane.children()[++first]->position().y;
                    }

                    _content_pane._first_visible_item = first;
                    _content_pane.position().y = _content_rect.pos.y + dy;
                    update_scrollbar_position();
                }
            }

            void update_scrollbar_position()
            {
                vertical_scrollbar().update_position( - (_content_pane.position().y - content_rectangle().pos.y) );
            }

        protected:
            using Vertical_scrollbar_t = Custom_vertical_scrollbar<Config, With_layout>;

            Pane            _content_pane;
        };

        // Layouter aspect

        // TODO: is it really needed, or is the Scrollbox layouter sufficient ?

        template<class Pane, class Parent>
        struct Layouter<true, Pane, Parent>: public Parent 
        {
            using Scrollbox_t = Scrollbox<Config, true, Pane>;

            void layout() override
            {
                p()->_content_pane.set_extents( p()->_content_pane.get_minimal_size() );

                Scrollbox_t::layout();
            }

            auto get_preferred_size() -> Extents override
            {
                // TODO: borders, padding, separation

                auto pane_size  = p()->_content_pane.get_preferred_size();
                auto vsbar_size = p()->vertical_scrollbar().get_preferred_size();

                auto w = pane_size.w + vsbar_size.w;
                auto h = std::max( pane_size.h, vsbar_size.h );

                return { w + 2 * p()->_border.width, h + 2 * p()->_border.width };
            }

        protected:
            struct Listbox_t: public Base<true, Pane> { friend struct Layouter; };
            auto p() { return static_cast<Listbox_t*>(static_cast<Base<true, Pane>*>(this)); }
        };

        // Pane -----------------------------------------------------

        template <class Pane, bool With_layout, class Parent> struct Pane__Layouter;

        template<class Class, bool With_layout>
        class Pane_base: public Pane__Layouter<Class, With_layout, Scrollable_pane<Config, With_layout> >
        {
        public:
            using Widget_t = Widget<Config, With_layout>;
            using Canvas_t = typename Widget_t::Canvas_t;
            using Listbox_base_t = Base<With_layout, Pane_base<Class, With_layout>>;
            using Scrollable_pane_t = Scrollable_pane<Config, With_layout>;
            using Parent_t = Scrollable_pane_t;
            using Parent_class = Scrollable_pane<Config, With_layout>;

            void set_separator(const Separator &sep)
            {
                _separator = { sep.width, Canvas_t::rgba_to_native(sep.color) };
            }

            void init() override
            {
                Parent_class::init();

                // Future...
            }

            void compute_view_from_data() override
            {
                Parent_class::compute_view_from_data();

                compute_visible_item_range();
            }

            void render(Canvas_t *canvas, const Point &offset) override 
            { 
                auto pos = offset + position();

                Length w = extents().w;
                Position y = 0;

                for (auto i = 0U; ; ) // i < children().size(); i ++)
                {
                    auto child = children()[i];

                    auto h = child->extents().h + _item_padding[0] + _item_padding[2];

                    fill_rect(canvas, {{0, y}, {w, h}}, pos, Canvas_t::rgba_to_native(static_cast<Class*>(this)->item_background_color(i)) );

                    child->render(canvas, pos);

                    if (++i >= children().size()) break; // we're done here

                    y += (Position_delta) h;

                    fill_rect(canvas, {{0, y}, {w, _separator.width}}, pos, _separator.color);

                    y += _separator.width;
                }
            }

            // Interface with Scrollbox container

            void scroll(Navigation_unit unit, Fraction<int> delta)
            {
                if (!children().empty())
                {
                    if (unit == Navigation_unit::element)
                    {
                        assert(delta.den == 1);
                        scroll_by_items(delta.num);
                    }
                    else if (unit == Navigation_unit::fraction)
                    {
                        // We can only scroll if the pane is higher than the listbox's content rectangle
                        if (extents().h > listbox()->content_rectangle().ext.h)
                        {
                            int dist = (int) hidden_items() * delta.num / delta.den;
                            if (dist != 0)
                            {
                                scroll_by_items(dist);
                            }
                        }
                    }
                    else if (unit == Navigation_unit::page)
                    {
                        assert(delta.den == 1);
                        scroll_by_pages(delta.num);
                    }
                    else {
                        assert(false); // TODO
                    }
                }
            }

        protected:
            friend class Listbox_t;
            using Mapped_separator_t = Mapped_separator<typename Config::Renderer::native_color>;

            auto listbox() { return static_cast<Listbox_base_t*>(container()); }

            static auto item_background_color(std::size_t) { return element_background_color(); }

            void compute_visible_item_range()
            {
                _first_visible_item = scan_children_forward(0, [this](auto child) { return child_fully_after_top(child); });
                _last_visible_item  = scan_children_forward(_first_visible_item, [this](auto child) { return !child_fully_before_bottom(child); }) - 1;
            }

            bool child_fully_after_top    (Widget_t *child, Position_delta offset = 0)
            {
                return position().y + offset + child->position().y >= listbox()->content_rectangle().pos.y;
            }
            
            bool child_fully_before_bottom(Widget_t *child, Position_delta offset = 0)
            {
                return position().y + offset + child->rectangle().bottom() <= listbox()->content_rectangle().bottom();
            }
            
            auto first_visible_child()
            { 
                return children()[_first_visible_item]; 
            }

            auto last_visible_child ()
            { 
                return children()[_last_visible_item]; 
            }

            void scroll_down(Count items = 1)
            {
                Length dy = 0;

                while (items-- > 0 && !child_fully_before_bottom(last_child(), - (Position_delta) dy))
                {
                    dy += first_visible_child()->extents().h + _item_padding[0] + _item_padding[2] + _separator.width;

                    _first_visible_item ++;
                }

                // TODO: special case of arriving at bottom (close gap) ?

                shift_up( dy );

                _last_visible_item = scan_children_forward(_last_visible_item, [=, this](auto child) { return !child_fully_before_bottom(child); }) - 1;
                assert(_last_visible_item >= 0);

                //std::cout << "scroll_down() -> _first_visible_item = " << _first_visible_item << ", _last_visible_item = " << _last_visible_item << std::endl;
            }

            void scroll_up  (Count items = 1)
            {
                Length dy = 0;

                while (items -- > 0 && _first_visible_item > 0)
                {
                    dy += children()[_first_visible_item - 1]->extents().h + _item_padding[0] + _item_padding[2] + _separator.width;

                    _first_visible_item --;
                }

                shift_down( dy );

                _last_visible_item = scan_children_backward(_last_visible_item, [=, this](auto child) { return child_fully_before_bottom(child); });
                assert(_last_visible_item >= 0);

                //std::cout << "scroll_down() -> _first_visible_item = " << _first_visible_item << ", _last_visible_item = " << _last_visible_item << std::endl;
            }
            
            void scroll_by_items(int delta)
            {
                if (delta < 0)
                {
                    scroll_up  ( - delta );
                }
                else if (delta > 0)
                {
                    scroll_down( delta );
                }
                else
                    assert(false);

                listbox()->update_scrollbar_position();
            }

            void scroll_by_pages(int delta)
            {
                int items = (int) children().size() * delta * (int) listbox()->content_rectangle().ext.h / (int) extents().h;

                scroll_by_items( items );
            }

            auto visible_items() const { return _last_visible_item - _first_visible_item + 1;  }
            
            auto hidden_items() { return visible_items() < (int) children().size() ? (int) (children().size()) - visible_items() : 0; }

            Mapped_separator_t  _separator {};

            Padding             _item_padding {};

            Index               _first_visible_item, _last_visible_item;
        };

        // Layouter aspect

        template<class Pane, class Parent>
        struct Pane__Layouter<Pane, true, Parent>: public Parent
        {
            void set_item_padding(Length w)
            {
                p()->_item_padding = { w, w, w, w };
            }

            void set_item_padding(const Padding &padding)
            {
                p()->_item_padding = padding;
            }

            auto get_minimal_size() -> Extents override
            {
                Length h_tot = 0;
                Width w_min = 0;

                for (auto child: p()->children())
                {
                    auto minsz = child->get_minimal_size();
                    if (minsz.w > w_min) w_min = minsz.w;
                    h_tot += minsz.h + p()->_item_padding[0] + p()->_item_padding[2] + p()->_separator.width;
                }

                h_tot -= p()->_separator.width;

                return { w_min + p()->_item_padding[3] + p()->_item_padding[1], h_tot };
            }

            void compute_and_set_extents(const Extents &container_extents)
            {
                p()->set_extents({ container_extents.w, p()->extents().h });
            }

            void layout() override
            {
                auto w = extents().w - p()->_item_padding[3] - p()->_item_padding[1];

                Position x = (Position) p()->_item_padding[3], y = 0;

                for (auto child: p()->children())
                {
                    y += (Position_delta) p()->_item_padding[0];

                    auto minsz = child->get_minimal_size();

                    child->set_position({ x, y       });
                    child->set_extents ({ w, minsz.h });
                    child->layout();

                    y += (Position_delta) minsz.h;
                    y += (Position_delta) p()->_item_padding[2];
                    y += (Position_delta) p()->_separator.width;
                }
            }

        protected:
            struct Pane_t: public Pane { friend struct Pane__Layouter; };
            auto p() { return static_cast<Pane_t*>(this); }
        };

        // "Vanilla" pane

        template<bool With_layout>
        class Vanilla_pane: public Pane_base<Vanilla_pane<With_layout>, With_layout>
        {
            friend class Base<With_layout, Vanilla_pane>;
        };

        // Pane with selectable items

        template<bool With_layout>
        class Selectable_items_pane: public Pane_base<Selectable_items_pane<With_layout>, With_layout>
        {
        public:
            using Widget_t = Widget<Config, With_layout>;
            using Canvas_t = typename Widget_t::Canvas_t;

            void mouse_button(const Point &pos, int button, Key_state state) override
            {
                if (state == Key_state::pressed)
                {
                    for (auto i = 0U; i < children().size(); i ++)
                    {
                        auto child = children()[i];
                        auto rect = child->rectangle();
                        rect.grow( _item_padding );

                        if (rect.contains(pos))
                        {
                            toggle_item(i);
                        }
                    }
                }

                Pane_base::mouse_button(pos, button, state);
            }

            void toggle_item(std::size_t index)
            {
                if (index >= _selected.size())
                {
                    _selected.resize(index + 1);
                    _selected[index] = true;
                }
                else {
                    _selected[index] = ! _selected[index];
                }

                invalidate(); // TODO: optimize
            }

            void select_range(std::size_t first, std::size_t last, bool add = false);

        protected:
            friend class Base<With_layout, Selectable_items_pane<With_layout>>;
            friend class Pane_base<Selectable_items_pane<With_layout>, With_layout>;

            auto item_background_color(std::size_t index)
            {
                if (index < _selected.size() && _selected[index])
                    return element_background_color_selected();
                else
                    return element_background_color();
            }

            std::vector<bool> _selected;
        };

    }; // pseudo-ns _listbox


    // Configurable Listbox class template --------------------------

    /** Specializable class template */
    
    template<class Config, bool With_layout, bool Items_selectable = false> class Listbox;

    /* Specialization: non-selectable items */

    template<class Config, bool With_layout>
    class Listbox<Config, With_layout, false>: public _listbox<Config>::Base<With_layout, 
        typename _listbox<Config>::Vanilla_pane<With_layout> > {};

    /* Specialization: with selectable items */

    template<class Config, bool With_layout>
    class Listbox<Config, With_layout, true>: 
        public _listbox<Config>::Base<With_layout, typename _listbox<Config>::Selectable_items_pane<With_layout> > {};

} // ns cppgui
