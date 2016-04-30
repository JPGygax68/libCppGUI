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
        auto item = _content_pane.children()[item_index];

        auto y1 = _content_pane.position().y + item->position().y; // TODO: subtract border/padding (use "inner rect" ?)
        auto y2 = y1 + static_cast<Position_delta>(item->extents().h);
        Position_delta delta_y = 0;

        if (y1 < 0)
        {
            delta_y = - y1;
        }
        else if (y2 > extents().bottom_edge()) // TODO: use "inner rect"
        {
            delta_y = - (y2 - static_cast<Position_delta>(extents().h));
        }

        if (delta_y != 0)
        {
            // TODO: this following two should be bundled and abstracted as a shift operation
            _content_pane.set_position({ _content_pane.position().x, _content_pane.position().y + delta_y });
            invalidate();

            //scrollbar().change_position()
        }

        //item->take_focus();
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    void Listbox__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        p()->_content_pane.set_extents( p()->_content_pane.get_minimal_size() );

        Scrollbox_t::layout();
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
        if (children().empty()) return;

        if (unit == Navigation_unit::element)
        {
            assert(delta.den == 1);

            if (delta.num < 0)
            {
                // TODO: emit sound if already on first item (or none in the list)

                if (_first_visible_item_index > 0)
                {
                    _first_visible_item_index --;
                    
                    shift_vertically( static_cast<Position_delta>(children()[_first_visible_item_index]->extents().h) );

                    // Update the index of the last fully visible item
                    auto i = _last_visible_item_index;
                    for (; i >= _first_visible_item_index; i --)
                    {
                        if (child_fully_before_bottom(children()[i])) break;
                    }

                    _last_visible_item_index = i;
                }
            }
            else if (delta.num > 0)
            {
                // TODO: emit sound if already on last item (or none in the list)

                if (!child_fully_before_bottom(last_child()))
                {
                    shift_vertically(- static_cast<Position_delta>(children()[_first_visible_item_index]->extents().h) );

                    _first_visible_item_index ++;

                    // Update the index of the last visible item
                    auto i = _last_visible_item_index;
                    for ( ; i < children().size() - 1; i ++)
                    {
                        if (!child_fully_before_bottom(children()[i])) break;
                    }

                    _last_visible_item_index = i;

                    // TODO: special case of arriving at bottom ?
                }
            }

            //listbox()->bring_item_into_view(_selected_item_index);
        }
        else {
            assert(false); // TODO
        }
    }

    template<class Config, bool With_layout>
    void List_pane<Config, With_layout>::compute_visible_item_range()
    {
        unsigned int first, last;

        for (first = 0; first < children().size(); first ++)
        {
            if (position().y + children()[first]->position().y >= 0) break;
        }

        for (last = first; last < children().size(); last ++)
        {
            if (position().y + children()[last]->rectangle().bottom() > listbox()->content_rectangle().bottom()) break;
        }

        _first_visible_item_index = first;
        _last_visible_item_index = last;
    }

    template<class Config, bool With_layout>
    bool List_pane<Config, With_layout>::child_fully_before_bottom(Widget_t * child)
    {
        return position().y + child->rectangle().bottom() <= listbox()->extents().bottom_edge();
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
