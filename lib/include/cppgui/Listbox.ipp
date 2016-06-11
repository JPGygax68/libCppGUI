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

#include "./Listbox.hpp"
#include "Listbox.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    Listbox<Config, With_layout>::Listbox()
    {
        //_content_pane.set_layout_type(Layout_type::stack); // TODO: will need adapting
        _content_pane.set_layout_manager<typename layouting<Config>::Stack>();

        set_content_pane(&_content_pane);
    }

    template<class Config, bool With_layout>
    void Listbox<Config, With_layout>::add_item(Layoutable_widget_t *item)
    {
        _content_pane.add_child(item);
    }

    template<class Config, bool With_layout>
    auto Listbox<Config, With_layout>::selected_item() -> Index
    {
        return _content_pane.child_index( focused_child() );
    }

    template<class Config, bool With_layout>
    auto Listbox<Config, With_layout>::item_index(Widget_t *child) -> Index
    {
        return _content_pane.child_index( child );
    }

    /** TODO: there wil eventually have to be two different variants of this, one for view computation
            time, which calculates a position but does not invalidate, and one that will scroll() 
            (with the invalidation that this usually means, depending on the Updater aspect).
     */
    template<class Config, bool With_layout>
    void Listbox<Config, With_layout>::ensure_item_in_view(int item_index)
    {
        // TODO: make sure this method can only be called in "view loading" state

        #ifndef NOT_DEFINED

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

        #else

        /** TODO: this implementation is *wrong* for the Listbox (though it might be ok for Scrollbox).
        The Listbox implementation needs to move forward item by item.
        */

        auto item = _content_pane.children()[item_index];

        auto y1 = _content_pane.position().y + item->position().y; // TODO: subtract border/padding (use "inner rect" ?)
        auto y2 = y1 + static_cast<Position_delta>(item->extents().h);
        Position_delta delta_y = 0;

        if (y1 < 0)
        {
            // TODO: replace with shift_down()
            delta_y = - y1;
        }
        else if (y2 > extents().bottom_edge()) // TODO: use "inner rect"
        {
            // TODO: replace with shift_up()
            delta_y = -(y2 - (Position_delta) (extents().h));
        }

        // TODO: remove as soon as shift_up() & shift_down() are being used
        if (delta_y != 0)
        {
            // TODO: these following two lines should be bundled and abstracted as a shift operation
            _content_pane.set_position({ _content_pane.position().x, _content_pane.position().y + delta_y });
            invalidate();

            update_scrollbar_position();
        }

        //item->take_focus();

        #endif
    }

    template<class Config, bool With_layout>
    void Listbox<Config, With_layout>::update_scrollbar_position()
        // Bring the position of the scrollbar in sync with the current vertical offset of the content pane
    {
        vertical_scrollbar().update_position( - (_content_pane.position().y - content_rectangle().pos.y) );
    }

    // Layouter aspect ----------------------------------------------

    template<class Config, class Parent>
    void Listbox__Layouter<Config, true, Parent>::layout()
    {
        p()->_content_pane.set_extents( p()->_content_pane.get_minimal_size() );

        Scrollbox_t::layout();
    }

    /*
    template<class Config, class Parent>
    auto Listbox__Layouter<Config, true, Parent>::get_preferred_size() -> Extents
    {
        // TODO: borders, padding, separation

        auto pane_size  = p()->_content_pane.get_preferred_size();
        auto vsbar_size = p()->vertical_scrollbar().get_preferred_size();

        auto w = pane_size.w + vsbar_size.w;
        auto h = std::max( pane_size.h, vsbar_size.h );

        return { w + 2 * p()->_border.width, h + 2 * p()->_border.width };
    }
    */

    // List_pane_base ====================================================

    template<class Config, bool With_layout>
    void List_pane_base<Config, With_layout>::set_separator(const Separator &sep)
    {
        _separator = { sep.width, Canvas_t::rgba_to_native(sep.color) };
    }

    template<class Config, bool With_layout>
    void List_pane_base<Config, With_layout>::init()
    {
        Parent_class::init();

        // Future...
    }

    template<class Config, bool With_layout>
    void List_pane_base<Config, With_layout>::compute_view_from_data()
    {
        Parent_class::compute_view_from_data();

        compute_visible_item_range();
    }

    template<class Config, bool With_layout>
    void List_pane_base<Config, With_layout>::render(Canvas_t *canvas, const Point & offset)
    {
        auto pos = offset + position();

        Length w = this->extents().w;
        Position y = 0;

        for (auto i = 0U; ; ) // i < children().size(); i ++)
        {
            auto child = children()[i];

            auto h = child->extents().h + _vert_item_padding;

            fill_rect(canvas, {{0, y}, {w, h}}, pos, Canvas_t::rgba_to_native(element_background_color()));

            child->render(canvas, pos);

            if (++i >= children().size()) break; // we're done here

            y += (Position_delta) h;

            fill_rect(canvas, {{0, y}, {w, _separator.width}}, pos, _separator.color);

            y += _separator.width;
        }
    }

    template<class Config, bool With_layout>
    void List_pane_base<Config, With_layout>::scroll(Navigation_unit unit, Fraction<int> delta)
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

    template<class Config, bool With_layout>
    void List_pane_base<Config, With_layout>::compute_visible_item_range()
    {
        _first_visible_item = scan_children_forward(0, [this](auto child) { return child_fully_after_top(child); });
        _last_visible_item  = scan_children_forward(_first_visible_item, [this](auto child) { return !child_fully_before_bottom(child); }) - 1;
    }

    template<class Config, bool With_layout>
    bool List_pane_base<Config, With_layout>::child_fully_after_top(Widget_t * child, Position_delta offset)
    {
        return position().y + offset + child->position().y >= listbox()->content_rectangle().pos.y;
    }

    template<class Config, bool With_layout>
    bool List_pane_base<Config, With_layout>::child_fully_before_bottom(Widget_t * child, Position_delta offset)
    {
        return position().y + offset + child->rectangle().bottom() <= listbox()->content_rectangle().bottom();
    }

    template<class Config, bool With_layout>
    void List_pane_base<Config, With_layout>::scroll_by_items(int delta)
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

    // TODO: return a boolean to indicate whether scrolling was possible or not ?

    template<class Config, bool With_layout>
    void List_pane_base<Config, With_layout>::scroll_up(Count items)
        // Note: scrolling UP means shifting the pane DOWN!
    {
        Length dy = 0;

        while (items -- > 0 && _first_visible_item > 0)
        {
            dy += children()[_first_visible_item - 1]->extents().h + _vert_item_padding + _separator.width;

            _first_visible_item --;
        }

        shift_down( dy );

        _last_visible_item = scan_children_backward(_last_visible_item, [=, this](auto child) { return child_fully_before_bottom(child); });
        assert(_last_visible_item >= 0);

        //std::cout << "scroll_down() -> _first_visible_item = " << _first_visible_item << ", _last_visible_item = " << _last_visible_item << std::endl;
    }

    template<class Config, bool With_layout>
    void List_pane_base<Config, With_layout>::scroll_by_pages(int delta)
    {
        int items = (int) children().size() * delta * (int) listbox()->content_rectangle().ext.h / (int) extents().h;

        scroll_by_items( items );

        // listbox()->update_scrollbar_position(); // TODO: call this from within scroll_page_up() / scroll_page_down() ?
    }

    // TODO: return a boolean to indicate whether scrolling was possible or not ?

    template<class Config, bool With_layout>
    void List_pane_base<Config, With_layout>::scroll_down(Count items)
        // Note: scrolling DOWN means shifting the pane UP!
    {
        Length dy = 0;

        while (items-- > 0 && !child_fully_before_bottom(last_child(), - (Position_delta) dy))
        {
            dy += first_visible_child()->extents().h + _vert_item_padding + _separator.width;

            _first_visible_item ++;
        }

        // TODO: special case of arriving at bottom (close gap) ?

        shift_up( dy );

        _last_visible_item = scan_children_forward(_last_visible_item, [=, this](auto child) { return !child_fully_before_bottom(child); }) - 1;
        assert(_last_visible_item >= 0);

        //std::cout << "scroll_down() -> _first_visible_item = " << _first_visible_item << ", _last_visible_item = " << _last_visible_item << std::endl;
    }

    // Layouter aspect ----------------------------------------------

    template<class Config, class Parent>
    void List_pane__Layouter<Config, true, Parent>::set_item_padding(const Extents &padding)
    {
        _item_padding = padding;
    }

    template<class Config, class Parent>
    auto List_pane__Layouter<Config, true, Parent>::get_minimal_size() -> Extents
    {
        Length h_tot = 0;
        Width w_min = 0;

        for (auto child: p()->children())
        {
            auto minsz = child->get_minimal_size();
            if (minsz.w > w_min) w_min = minsz.w;
            h_tot += minsz.h + 2 * _item_padding.h + p()->_separator.width;
        }

        h_tot -= p()->_separator.width;

        return { w_min + 2 * _item_padding.w, h_tot };
    }

    template<class Config, class Parent>
    void List_pane__Layouter<Config, true, Parent>::compute_and_set_extents(const Extents &container_extents)
    {
        p()->set_extents({ container_extents.w, p()->extents().h });
    }

    template<class Config, class Parent>
    void List_pane__Layouter<Config, true, Parent>::layout()
    {
        auto w = extents().w - 2 * _item_padding.w;

        Position x = (Position) _item_padding.w, y = 0;

        for (auto child: p()->children())
        {
            y += (Position_delta) _item_padding.h;

            auto minsz = child->get_minimal_size();

            child->set_position({ x, y       });
            child->set_extents ({ w, minsz.h });
            child->layout();

            y += (Position_delta) minsz.h;
            y += (Position_delta) _item_padding.h;
            y += (Position_delta) p()->_separator.width;
        }

        p()->_vert_item_padding = 2 * _item_padding.h;
    }

} // ns cppgui
