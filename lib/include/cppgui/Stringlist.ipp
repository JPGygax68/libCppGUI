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

#include "./unicode.hpp"

#include "./Stringlist.hpp"

namespace cppgui {

    template<class Config>
    template<class Class, bool With_layout>
    _stringlist<Config>::Base<Class, With_layout>::Base()
    {
        add_child(&_vert_sbar);

        _vert_sbar.on_navigation([ this ](Navigation_unit unit, const Fraction<int> &delta) {

            switch (unit)
            {
            case Navigation_unit::element:
                assert(delta.den == 1);
                move_by_elements(delta.num);
                break;
            case Navigation_unit::page:
                assert(delta.den == 1);
                move_by_pages(delta.num);
                break;
            case Navigation_unit::fraction:
                move_by_fraction(delta);
                break;
            case full_range: 
                // TODO
                break;
            default: 
                // TODO
                break;
            }

            //static_cast<Pane*>(_content)->scroll(unit, delta);
        });
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::on_item_selected(Item_selected_handler handler)
    {
        assert(!_on_item_selected);
        _on_item_selected = handler;
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::on_item_activated(Item_activated_handler handler)
    {
        assert(!_on_item_activated);
        _on_item_activated = handler;
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::set_font(const Rasterized_font * font)
    {
        _font.assign( font );
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::init()
    {
        _font.translate( this->root_widget()->canvas() );

        Container_base_t::init();
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::compute_view_from_data()
    {
        compute_content_rectangle();

        _vert_sbar.define_values(_items.size(), fully_visible_item_count());

        Container_base_t::compute_view_from_data();
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::add_item(const std::string & text)
    {
        add_item( utf8_to_utf32(text) );
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::add_item(const std::u32string & text)
    {
        _items.push_back( text );
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::render(Canvas_t *canvas, const Point & offset)
    {
        Point pos = offset + this->position();

        // fill(canvas, offset, Canvas_t::rgba_to_native({1, 1, 1, 1}) );

        draw_border(canvas, offset);

        // Render the separator between the scrollbar and the content rectangle
        canvas->fill_rect(pos.x + _vert_sep_pos, pos.y, _sbar_separator.width, this->extents().h,
            Canvas_t::rgba_to_native(_sbar_separator.color));
        //draw_vert_separator(canvas, offset, _sbar_separator, _vert_sep_pos);

        // Render the scrollbar (delegate to the container)
        Container_base_t::render(canvas, offset);

        // Render the items
        canvas->push_clipping_rect( _content_rect + pos );

        // Item rectangle
        Rectangle r_item { _content_rect };
        r_item.ext.h = item_height();

        // Separator rectangle
        Rectangle r_sep { r_item };
        r_sep.pos.y += r_item.ext.h;
        r_sep.ext.h = _item_separator.width;

        // Draw all items
        for (auto i = _first_vis_item; ; ++ i)
        {
            // Done ?
            if (i >= static_cast<Index>(_items.size()))
            {
                if (r_item.pos.y < _content_rect.bottom())
                {
                    auto r_bgnd = r_item;
                    r_bgnd.set_bottom( _content_rect.bottom() );
                    fill_rect(canvas, r_bgnd, pos, Canvas_t::rgba_to_native(this->widget_background_color()));
                }
                break;
            }

            // Draw item background
            Native_color bgclr;
            if      (i == _selected_item) bgclr = Canvas_t::rgba_to_native(this->selected_item_background_color());
            else if (i == _hovered_item ) bgclr = Canvas_t::rgba_to_native(this->hovered_item_background_color ());
            else                          bgclr = Canvas_t::rgba_to_native(this->item_background_color         ());
            fill_rect(canvas, r_item, pos, bgclr);

            // Render item text
            Point p_text = r_item.pos + Extents{ _item_padding[3], _item_padding[0] + _ascent };
            Width w_text = r_item.width() - _item_padding[3] - _item_padding[1];
            canvas->render_text( _font.get(), pos.x + p_text.x, pos.y + p_text.y, _items[i].data(), _items[i].size(), w_text);

            r_item.pos.y += static_cast<Position_delta>(r_item.ext.h);
            if (r_item.pos.y >= _content_rect.bottom()) break;

            // Draw item separator
            r_sep.pos.y = r_item.pos.y;
            fill_rect(canvas, r_sep, pos, Canvas_t::rgba_to_native(_item_separator.color) );

            r_item.pos.y += _item_separator.width;
            if (r_item.pos.y >= _content_rect.bottom()) break;
        }

        // Done rendering items
        canvas->pop_clipping_rect();
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::mouse_motion(const Point& pos)
    {
        auto item = item_at_pos(pos);
        if (item != _hovered_item)
        {
            _hovered_item = item;
            this->invalidate();
        }

        Parent_t::mouse_motion(pos);
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::mouse_button(const Point &pos, int button, Key_state state, Count clicks)
    {
        if (button == 1 && state == Key_state::pressed && clicks == 1)
        {
            auto index = item_at_pos(pos);
            if (index != _selected_item)
            {
                if (index >= 0)
                {
                    // Is item partially hidden ?
                    // TODO: with pixel-wise scrolling, we would need a generalized bring_item_into_view() method
                    if (index >= _first_vis_item + static_cast<Index>(fully_visible_item_count()))
                    {
                        // TODO: optimizable scrolling
                        _first_vis_item ++;
                    }
                }
                select_item(index);
                this->invalidate();
            }
        }
        else if (button == 1 && state == Key_state::released && clicks == 2)
        {
            auto index = item_at_pos(pos);
            if (index >= 0) raise_item_activated(index, _items[index]);
        }

        Parent_t::mouse_button(pos, button, state, clicks);
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::mouse_wheel(const Vector& delta)
    {
        _vert_sbar.mouse_wheel(delta);
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::mouse_exit()
    {
        _hovered_item = -1;

        Parent_t::mouse_exit();
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::key_down(const Keycode& key)
    {
        if      (Keyboard::is_down     (key)) select_next           ();
        else if (Keyboard::is_up       (key)) select_previous       ();
        else if (Keyboard::is_page_down(key)) page_down             ();
        else if (Keyboard::is_page_up  (key)) page_up               ();
        else if (Keyboard::is_return   (key)) activate_selected_item();
        else
            Parent_t::key_down(key);
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::compute_content_rectangle()
    {
        Length sb_width = _vert_sbar.extents().w + _sbar_separator.width;

        _content_rect = Rectangle{ this->extents() } - 
            Margins{ this->get_border_width(0), this->get_border_width(1) + sb_width, 
                this->get_border_width(2), this->get_border_width(3) };
    }

    template<class Config>
    template<class Class, bool With_layout>
    auto _stringlist<Config>::Base<Class, With_layout>::item_height() const
    {
        return static_cast<Length>(_item_padding[0] + _ascent - _descent + _item_padding[2]);
    }

    template <class Config>
    template <class Class, bool With_layout>
    inline auto _stringlist<Config>::Base<Class, With_layout>::fully_visible_item_count() const -> Count
    {
        auto h_rect = _content_rect.height(), h_item = item_height() + _item_separator.width;
        auto n = h_rect / h_item, r = h_rect % h_item;
        if (r <= _item_separator.width) n += 1;
        return n;
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::move_by_elements(int delta)
    {
        if (delta > 0)
        {
            Index cap = std::max(0, static_cast<Index>(_items.size()) - static_cast<Index>(fully_visible_item_count()));
            auto first = std::min(cap, _first_vis_item + delta);
            if (first != _first_vis_item)
            {
                // TODO: define and use scroll() function
                _first_vis_item = first;
                _hovered_item = -1; // TODO: update using current mouse position ?
                update_scrollbar_position();
                this->invalidate();
            }
        }
        else if (delta < 0)
        {
            if (_first_vis_item > 0)
            {
                --_first_vis_item;
                _hovered_item = -1; // TODO: update using current mouse position ?
                update_scrollbar_position();
                this->invalidate();
            }
        }
        else 
            assert(false);
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::move_by_pages(int delta)
    {
        if (delta > 0)
        {
            Index cap = _items.size() - fully_visible_item_count();
            auto first = std::min(cap, _first_vis_item + static_cast<Index>(fully_visible_item_count()) * delta);
            if (first != _first_vis_item)
            {
                // TODO: define and use scroll() function
                _first_vis_item = first;
                _hovered_item = -1; // TODO: update using current mouse position ?
                update_scrollbar_position();
                this->invalidate();
            }
        }
        else if (delta < 0)
        {
            if (_first_vis_item > 0)
            {
                Index first = std::max(0, _first_vis_item + delta * static_cast<Index>(fully_visible_item_count()));
                if (first != _first_vis_item)
                {
                    _first_vis_item = first;
                    _hovered_item = -1; // TODO: update using current mouse position ?
                    update_scrollbar_position();
                    this->invalidate();
                }
            }
        }
        else 
            assert(false);
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::move_by_fraction(const Fraction<int>& delta)
    {
        if (delta.den == 0) return;

        std::cout << delta.num << "/" << delta.den << std::endl;

        if (delta.num > 0)
        {
            Index cap = _items.size() - fully_visible_item_count();
            Index range = _items.size() - fully_visible_item_count();
            auto first = std::min(cap, _first_vis_item + delta.num * range / delta.den);
            if (first != _first_vis_item)
            {
                // TODO: define and use scroll() function
                _first_vis_item = first;
                _hovered_item = -1; // TODO: update using current mouse position ?
                update_scrollbar_position();
                this->invalidate();
            }
        }
        else if (delta.num < 0)
        {
            if (_first_vis_item > 0)
            {
                Index range = _items.size() - fully_visible_item_count();
                Index first = std::max(0, _first_vis_item + delta.num * range / delta.den);
                if (first != _first_vis_item)
                {
                    // TODO: define and use scroll() function
                    _first_vis_item = first;
                    _hovered_item = -1; // TODO: update using current mouse position ?
                    update_scrollbar_position();
                    this->invalidate();
                }
            }
        }
        else 
            assert(false);
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::update_scrollbar_position()
    {
        _vert_sbar.update_position(_first_vis_item);
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::select_next()
    {
        if (_selected_item < static_cast<Index>(_items.size() -1))
        {
            select_item( _selected_item + 1 );
            if (_selected_item >= static_cast<Index>(fully_visible_item_count()) + _first_vis_item) 
            {
                // TODO: optimizable scrolling
                _first_vis_item ++;
                _hovered_item = -1; // TODO: update using current mouse position ?
                update_scrollbar_position();
            }
            this->invalidate();
        }
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::select_previous()
    {
        if (_selected_item > 0)
        {
            select_item( _selected_item - 1 );
            if (_selected_item < _first_vis_item)
            {
                // TODO: optimizable scrolling
                _first_vis_item --;
                _hovered_item = -1; // TODO: update using current mouse position ?
                update_scrollbar_position();
            }
            this->invalidate();
        }
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::page_down()
    {
        Index item = _selected_item < 0 ? fully_visible_item_count() - 1 : _selected_item + fully_visible_item_count() - 1;
        if (item >= static_cast<Index>(_items.size())) item = static_cast<Index>(_items.size() -1);
        if (item > _selected_item)
        {
            if (item >= _first_vis_item + static_cast<Index>(fully_visible_item_count()))
            {
                Index first = item - (fully_visible_item_count() - 1);
                // TODO: optimizable scrolling
                _first_vis_item = first;
                _hovered_item = -1; // TODO: update using current mouse position ?
                update_scrollbar_position();
            }
            select_item( item );
            this->invalidate();
        }
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::page_up()
    {
        if (_selected_item >= 0)
        {
            Index item = _selected_item - (fully_visible_item_count() - 1);
            if (item < 0) item = 0;
            if (item != _selected_item)
            {
                if (item < _first_vis_item)
                {
                    // TODO: optimizable scrolling
                    _first_vis_item = item;
                    _hovered_item = -1; // TODO: update using current mouse position ?
                    update_scrollbar_position();
                }
                select_item( item );
                this->invalidate();
            }
        }
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::activate_selected_item()
    {
        if (_selected_item >= 0)
        {
            raise_item_activated( _selected_item, _items[_selected_item]);
        }
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::raise_item_activated(Index index, const std::u32string& item)
    {
        if (_on_item_activated) _on_item_activated(index, item);
    }

    template <class Config>
    template <class Class, bool With_layout>
    auto _stringlist<Config>::Base<Class, With_layout>::item_at_pos(const Point& pos) -> Index
    {
        if (!_content_rect.contains(pos)) return -1;

        // Check position
        auto res = std::div(pos.y - _content_rect.top(), (item_height() + _item_separator.width));

        // On separator ?
        if (res.rem >= static_cast<decltype(res.rem)>(item_height())) return -1;

        // Compute index
        auto index = _first_vis_item + res.quot;

        // Past last item ?
        if (index >= static_cast<int>(_items.size())) return -1;
    
        return index;
    }

    template <class Config>
    template <class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::select_item(Index index)
    {
        _selected_item = index;
        if (_on_item_selected && index >= 0) _on_item_selected(index, _items[index]);
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Class, class Parent>
    void _stringlist<Config>::Layouter<Class, true, Parent>::init_layout()
    {
        compute_text_extents();

        Parent::init_layout();
    }

    template<class Config>
    template<class Class, class Parent>
    auto _stringlist<Config>::Layouter<Class, true, Parent>::get_minimal_size() -> Extents
    {
        return { 100, 100 }; // TODO
    }

    template<class Config>
    template<class Class, class Parent>
    void _stringlist<Config>::Layouter<Class, true, Parent>::layout()
    {
        // Preparations
        auto exts = p()->extents();
        auto vertsb_minsz = p()->_vert_sbar.get_minimal_size();

        // Vertical scrollbar
        p()->_vert_sep_pos = exts.w - p()->get_border_width(1) - vertsb_minsz.w - p()->_sbar_separator.width;
        p()->_vert_sbar.set_position({ 
            exts.right() - (vertsb_minsz.w + p()->get_border_width(1)), 
            p()->get_border_width(0)
        });
        p()->_vert_sbar.set_extents ({ vertsb_minsz.w, exts.h - p()->get_border_width(0) - p()->get_border_width(2) });
        p()->_vert_sbar.layout();

        //Parent::layout();
    }

    template<class Config>
    template<class Class, class Parent>
    void _stringlist<Config>::Layouter<Class, true, Parent>::compute_text_extents()
    {
        if (p()->_font.source())
        {
            // TODO: support other cultures
            auto bbox = p()->_font.source()->compute_text_extents(0, U"My", 2);
            p()->_ascent  = bbox.y_max;
            p()->_descent = bbox.y_min;
            //p()->_mean_char_width = (bbox.width() + 1) / 2;
        }
    }

} // ns cppgui