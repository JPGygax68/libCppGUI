#pragma once

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016, 2017 Hans-Peter Gygax

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

#include <algorithm>
#include "./basic_types.hpp"
#include "./Vertical_scrollbar.hpp"


namespace cppgui {

    Vertical_scrollbar_base::Vertical_scrollbar_base():
        // TODO: obtain from static value or stylesheet:
        _slide_bgcol        ({ 0.7f, 0.7f, 0.7f, 1 }), 
        _thumb_color        ({ 0.8f, 0.8f, 0.8f, 1 }),
        _thumb_hovered_color({ 0.9f, 0.9f, 0.9f, 1 })
    {

        //_up_btn  .enable_border(false);
        //_down_btn.enable_border(false);
        //_up_btn  .set_border({0});
        //_down_btn.set_border({0});

        _up_btn  .set_glyph(cppgui::Icon_resources<24>::up_arrow  ());
        _down_btn.set_glyph(cppgui::Icon_resources<24>::down_arrow());

        _up_btn  .set_focussable(false);
        _down_btn.set_focussable(false);

        _up_btn  .on_pushed([this]() { static_cast<Impl*>(this)->move_by_elements(-1); });
        _down_btn.on_pushed([this]() { static_cast<Impl*>(this)->move_by_elements( 1); });

        add_child(&_up_btn  );
        add_child(&_down_btn);
    }

    void Vertical_scrollbar_base::define_sizes(Length full, Length fraction)
    {
        // assert(full >= fraction);
        if (full < fraction) full = fraction; // TODO: better way ?

        _full_range = full;
        _shown_range = fraction;
    }

    void Vertical_scrollbar_base::init(Canvas *c)
    {
        Container::init(c);

        //_slide_bgcol        .translate(c);
        //_thumb_color        .translate(c);
        //_thumb_hovered_color.translate(c);
    }

    void Vertical_scrollbar_base::compute_view_from_data()
    {
        _thumb_rect.pos = { 2, _track.from };
        // TODO: position from style

        recalc_thumb();
    }

    void Vertical_scrollbar_base::render(Canvas *c, const Point &offset)
    {
        // Background
        // TODO: only draw the part not covered by the buttons ?
        fill(c, offset, _slide_bgcol.get() ); 

        // Thumb
        fill_rect(c, _thumb_rect, offset + position(), _thumb_hovered ? _thumb_hovered_color.get() : _thumb_color.get() );

        // Children: up and down buttons
        Container::render(c, offset);
    }

    // TODO: test!!

    void Vertical_scrollbar_base::change_sizes(Length full, Length shown)
    {
        define_sizes(full, shown);
        recalc_thumb();
        invalidate();
    }

    void Vertical_scrollbar_base::mouse_button(const Point &pos, int button, Key_state state, Count clicks)
    {
        if (button == 1 && state == Key_state::pressed)
        {
            if (pos.y > _track.from && pos.y < _track.to)
            {
                auto y_rel = pos.y - _thumb_rect.pos.y;

                // Above thumb ?
                if (y_rel < 0)
                {
                    static_cast<Impl*>(this)->move_by_page(-1);
                }
                // Below thumb ?
                else if (y_rel > _thumb_rect.ext.bottom())
                {
                    static_cast<Impl*>(this)->move_by_page(1);
                }
                else // On thumb
                {
                    /*_cur_drag_pos = */ _drag_anchor_pos = pos.y;
                    _dragging_thumb = true;
                    root_widget()->capture_mouse(this); // called here because Widget::mouse_button() will not be called
                }

                return; // done with this event
            }
        }
        else if (button == 1 && state == Key_state::released)
        {
            if (_dragging_thumb)
            {
                _dragging_thumb = false;
                // We bypass Container_t::mouse_button() here and call Widget_t::mouse_button() instead,
                // because Container_t does not (at the moment - TODO) pass thru to Widget<>
                Container::mouse_button(pos, button, state, clicks); //root_widget()->release_mouse();
                return;
            }
        }

        Container::mouse_button(pos, button, state, clicks);
    }

    void Vertical_scrollbar_base::mouse_motion(const Point &pos)
    {
        // std::cout << "mouse_motion: " << pos.x << ", " << pos.y << std::endl;

        bool on_thumb = _thumb_rect.contains(pos);
        if (on_thumb != _thumb_hovered)
        {
            _thumb_hovered = on_thumb;
            invalidate();
        }

        if (is_mouse_button_down(1) && _dragging_thumb)
        {
            //std::cerr << "delta = " << (pos.y - _drag_anchor_pos) << std::endl;
            notify_drag_navigation(pos.y - _drag_anchor_pos); // TODO: rename method ?
        }
        else
            Container::mouse_motion(pos);
    }

    void Vertical_scrollbar_base::mouse_wheel(const Vector &delta)
    {
        static_cast<Impl*>(this)->move_by_elements(-delta.y);
    }

    void Vertical_scrollbar_base::mouse_exit()
    {
        if (_thumb_hovered)
        {
            // TODO: reaction of some kind ?
            _thumb_hovered = false;
        }

        Container::mouse_exit();
    }

    auto Vertical_scrollbar_base::current_value_from_thumb_position() -> Fraction<Length>
    {
        if (_track.length() > _thumb_rect.ext.h)
        {
            return {
                (_full_range - _shown_range) * (_thumb_rect.pos.y - _track.from),
                _track.length() - _thumb_rect.ext.h
            };
        }
        else
            return { 0, 1 };
    }

    void Vertical_scrollbar_base::update_thumb_position(Length value)
    {
        assert(_full_range >= _shown_range);

        if (_full_range > _shown_range)
        {
            value = clamp(value, 0, _full_range - _shown_range);

            auto new_y = _track.from + (value * (_track.length() - _thumb_rect.ext.h) / (_full_range - _shown_range));

            if (_dragging_thumb)
            {
                _drag_anchor_pos += new_y - _thumb_rect.pos.y;
            }

            _thumb_rect.pos.y = new_y;
        }

        invalidate();
    }

    /*
    template<class Impl, class Config, bool With_layout, Box_model_definition BMDef>
    void Vertical_scrollbar_base<Impl, Config, With_layout, BMDef>::move_thumb_to(Position new_pos)
    {
        new_pos = std::max(new_pos, _track.from);
        new_pos = std::min(new_pos, _track.to - _thumb_rect.ext.h);

        _thumb_rect.pos = {2, new_pos};
        
        //notify_position_change();
        invalidate();
    }
    */

    void Vertical_scrollbar_base::recalc_thumb()
    {
        _thumb_rect.ext.h = std::max(
            _full_range == 0 ? 0 : _track.length() * _shown_range / _full_range, 
            _thumb_rect.ext.w / 2
        );

        clip_thumb_pos();
    }

    void Vertical_scrollbar_base::clip_thumb_pos()
    {
        if (_thumb_rect.bottom() > _track.to)
        {
            _thumb_rect.pos.y = _track.to - static_cast<Position>(_thumb_rect.ext.h);
        }
        else if (_thumb_rect.pos.y < _track.from)
        {
            _thumb_rect.pos.y = _track.from;
        }
    }

    void Vertical_scrollbar_base::notify_drag_navigation(Position_delta delta)
    {
        if (delta != 0)
        {
            //std::cerr << "delta = " << delta << std::endl;
            //static_cast<Impl*>(this)->move_by_fraction(_drag_start_pos, { delta, static_cast<int>(_sliding_range.l - _thumb_rect.ext.h) });
            static_cast<Impl*>(this)->move_by_fraction({ delta, _track.length() - _thumb_rect.ext.h });
        }
    }

    // Layouter aspect ----------------------------------------------

    auto Vertical_scrollbar_base::get_minimal_bounds() -> Bounding_box
    {
        // Width: based on up/down buttons, height: 3 times button height

        auto btn_minsz = _up_btn.get_minimal_bounds();

        return { btn_minsz.w, 3 * btn_minsz.h };
    }

    void Vertical_scrollbar_base::set_bounds(const Point &p, const Bounding_box &b)
    {
        auto ext = extents();

        auto minsz_up_btn   = _up_btn  .get_minimal_size();
        auto minsz_down_btn = _down_btn.get_minimal_size();

        _up_btn  .set_rectangle_nw({0, 0           }, { ext.w, minsz_up_btn  .h} );
        _down_btn.set_rectangle_sw({0, ext.bottom()}, { ext.w, minsz_down_btn.h} );

        _track.define( _up_btn.rectangle().bottom() + 2, _down_btn.rectangle().top() - 2 );

        _thumb_rect.pos.x = 2; // TODO: obtain margin from style
        _thumb_rect.ext.w = extents().w - 4; // obtain margin from style
                                                       //_thumb_rect.ext.h = 20; // TODO: REMOVE, temporary

        _up_btn  .layout();
        _down_btn.layout();
    }

    // Customizable specialization ==================================

    void Custom_vertical_scrollbar::on_navigation(Navigation_handler handler)
    {
        _nav_handler = handler;
    }

    void Custom_vertical_scrollbar::move_by_page(int delta)
    {
        // Note: the nav handler must update the position in response

        if (_nav_handler)
        {
            _nav_handler(Navigation_unit::page, /* current_position(), */ { delta, 1 });
            //invalidate();
        }
    }

    void Custom_vertical_scrollbar::move_by_elements(int delta)
    {
        if (_nav_handler)
        {
            _nav_handler(Navigation_unit::element, /* current_position(), */ { delta, 1 });
            //invalidate();
        }
    }

    void Custom_vertical_scrollbar::move_by_fraction(/* Position initial_pos, */ const Fraction<int> &delta)
    {
        if (_nav_handler)
        {
            _nav_handler(Navigation_unit::fraction, /* initial_pos, */ delta);
            //invalidate();
        }
    }

    // Standalone specialization ====================================

    void Vertical_scrollbar::on_position_change(Position_change_handler handler)
    {
        _on_pos_chng = handler;
    }

    void Vertical_scrollbar::move_by_page(int pages)
    {
        auto delta = shown_range() * pages;
        //auto new_pos = current_thumb_position() + delta;
        update_thumb_position( current_value_from_thumb_position() + delta );
        notify_position_change();
    }

    void Vertical_scrollbar::move_by_elements(int elements)
    {
        update_thumb_position(current_value_from_thumb_position() + elements * 10); // TODO: make step configurable
        notify_position_change();
        invalidate();
    }

    void Vertical_scrollbar::move_by_fraction(/* Position initial_pos, */ const Fraction<int>& frac)
    {
        auto delta = full_range() * frac.num / frac.den;
        if (delta == 0) delta = frac.num * frac.den < 0 ? -1 : 1;
        auto thumb_pos = current_value_from_thumb_position();
        auto new_pos = (thumb_pos.num + delta * thumb_pos.den) / thumb_pos.den;
        update_thumb_position( new_pos );
        notify_position_change();
    }

    void cppgui::Vertical_scrollbar::notify_position_change()
    {
        if (_on_pos_chng) _on_pos_chng(current_value_from_thumb_position());
    }

} // ns cppgui