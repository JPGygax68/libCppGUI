#include "./Listbox.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    Listbox<Config, With_layout>::Listbox()
    {
        _content_pane.set_layout_type(Layout_type::stack); // TODO: will need adapting

        set_content_pane(&_content_pane);
    }

    template<class Config, bool With_layout>
    void Listbox<Config, With_layout>::add_item(Layoutable_widget_t *item)
    {
        _content_pane.add_child(item);
    }

    template<class Config, bool With_layout>
    void Listbox<Config, With_layout>::init()
    {
        Scrollbox_t::init();
    }

    template<class Config, bool With_layout>
    void Listbox<Config, With_layout>::bring_item_into_view(int item_index)
    {
        // TODO: this will need adapting to the new content_rectangle() (not just extents anymore)

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

            //scrollbar().change_position()
        }

        //item->take_focus();
    }

    template<class Config, bool With_layout>
    void Listbox<Config, With_layout>::update_scrollbar_position()
        // Bring the position of the scrollbar in sync with the current vertical offset of the content pane
    {
        vertical_scrollbar().change_position( - (_content_pane.position().y - content_rectangle().pos.y) );
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    void Listbox__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        p()->_content_pane.set_extents( p()->_content_pane.get_minimal_size() );

        Scrollbox_t::layout();
    }

    template<class Config>
    template<class Aspect_parent>
    auto Listbox__Layouter<Config, true>::Aspect<Aspect_parent>::get_preferred_size() -> Extents
    {
        // TODO: borders, padding, separation

        auto pane_size  = p()->_content_pane.get_preferred_size();
        auto vsbar_size = p()->vertical_scrollbar().get_preferred_size();

        auto w = pane_size.w + vsbar_size.w;
        auto h = std::max( pane_size.h, vsbar_size.h );

        return { w + 2 * p()->_border.width, h + 2 * p()->_border.width };
    }

    // List_pane ====================================================

    template<class Config, bool With_layout>
    void List_pane<Config, With_layout>::init()
    {
        Parent_class::init();

        compute_visible_item_range();
    }

    template<class Config, bool With_layout>
    void List_pane<Config, With_layout>::scroll(Navigation_unit unit, Position initial_pos, Fraction<int> delta)
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
                    // Compute item index from initial_pos (which is in pixels), then add fraction
                    // TODO: it may be more precise to compute the movement first and translate that to items afterwards
                    auto initial_item = children().size() * (initial_pos + first_visible_child()->extents().h / 2) / extents().h;
                    int new_pos = initial_item + (int) hidden_items() * delta.num / delta.den;
                    if (new_pos != _first_visible_item)
                    {
                        scroll_by_items(new_pos - _first_visible_item);
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
    void List_pane<Config, With_layout>::compute_visible_item_range()
    {
        _first_visible_item = scan_children_forward(0, [this](auto child) { return child_fully_after_top(child); });
        _last_visible_item  = scan_children_forward(_first_visible_item, [this](auto child) { return !child_fully_before_bottom(child); }) - 1;
    }

    template<class Config, bool With_layout>
    bool List_pane<Config, With_layout>::child_fully_after_top(Widget_t * child, Position_delta offset)
    {
        return position().y + offset + child->position().y >= listbox()->content_rectangle().pos.y;
    }

    template<class Config, bool With_layout>
    bool List_pane<Config, With_layout>::child_fully_before_bottom(Widget_t * child, Position_delta offset)
    {
        return position().y + offset + child->rectangle().bottom() <= listbox()->content_rectangle().bottom();
    }

    template<class Config, bool With_layout>
    void List_pane<Config, With_layout>::scroll_by_items(int delta)
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
    void List_pane<Config, With_layout>::scroll_up(Count items)
        // Note: scrolling UP means shifting the pane DOWN!
    {
        Length dy = 0;

        while (items -- > 0 && _first_visible_item > 0)
        {
            dy += children()[_first_visible_item - 1]->extents().h;

            _first_visible_item --;
        }

        shift_down( dy );

        _last_visible_item = scan_children_backward(_last_visible_item, [=, this](auto child) { return child_fully_before_bottom(child); });
        assert(_last_visible_item >= 0);

        //std::cout << "scroll_down() -> _first_visible_item = " << _first_visible_item << ", _last_visible_item = " << _last_visible_item << std::endl;
    }

    template<class Config, bool With_layout>
    void List_pane<Config, With_layout>::scroll_by_pages(int delta)
    {
        int items = (int) children().size() * delta * (int) listbox()->content_rectangle().ext.h / (int) extents().h;

        scroll_by_items( items );

        listbox()->update_scrollbar_position(); // TODO: call this from within scroll_page_up() / scroll_page_down() ?
    }

    // TODO: return a boolean to indicate whether scrolling was possible or not ?

    template<class Config, bool With_layout>
    void List_pane<Config, With_layout>::scroll_down(Count items)
        // Note: scrolling DOWN means shifting the pane UP!
    {
        Length dy = 0;

        while (items-- > 0 && !child_fully_before_bottom(last_child(), - (Position_delta) dy))
        {
            dy += first_visible_child()->extents().h;

            _first_visible_item ++;
        }

        // TODO: special case of arriving at bottom (close gap) ?

        shift_up( dy );

        _last_visible_item = scan_children_forward(_last_visible_item, [=, this](auto child) { return !child_fully_before_bottom(child); }) - 1;
        assert(_last_visible_item >= 0);

        //std::cout << "scroll_down() -> _first_visible_item = " << _first_visible_item << ", _last_visible_item = " << _last_visible_item << std::endl;
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    auto List_pane__Layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        Position y = 0;
        Width w_min = 0;

        for (auto child: p()->children())
        {
            auto minsz = child->get_minimal_size();
            if (minsz.w > w_min) w_min = minsz.w;
            y += static_cast<Position_delta>(minsz.h);
        }

        return { w_min, static_cast<Length>(y) };
    }

    template<class Config>
    template<class Aspect_parent>
    void List_pane__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto w = extents().w;

        Position y = 0;

        for (auto child: p()->children())
        {
            auto minsz = child->get_minimal_size();

            child->set_position({ 0, y       });
            child->set_extents ({ w, minsz.h });
            child->layout();

            y += static_cast<Position_delta>(minsz.h);
        }
    }

} // ns cppgui
