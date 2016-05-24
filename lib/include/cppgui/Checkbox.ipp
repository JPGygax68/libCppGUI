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

#include <cassert>
#include <memory>

#include <gpc/fonts/store.hpp>

#include "./Root_widget.hpp"
#include "./Icon_resources.hpp"

#include "./Checkbox.hpp"

namespace cppgui {

    static std::unique_ptr<Rasterized_font> glyph_font;

    template<class Config, bool With_layout>
    void Checkbox<Config, With_layout>::set_tick_glyph(const Rasterized_font *font, const Font_icon_descr &descr)
    {
        _glyph_font.assign( font );
        _tick_descr = descr;
    }

    template<class Config, bool With_layout>
    void Checkbox<Config, With_layout>::on_state_change(State_change_handler handler)
    {
        assert(!_state_change_handler);
        _state_change_handler = handler;
    }

    template<class Config, bool With_layout>
    void Checkbox<Config, With_layout>::init()
    {
        _label_font.translate( root_widget()->canvas() );
        _glyph_font.translate( root_widget()->canvas() );
    }

    template<class Config, bool With_layout>
    void Checkbox<Config, With_layout>::render(Canvas_t *cv, const Point & offs)
    {
        auto pos = offs + position();

        fill(cv, offs, {1, 1, 0.5f, 1});

        cv->render_text(_label_font.get(), pos.x + _label_pos.x, pos.y + _label_pos.y, _label.data(), _label.size());

        fill_rect(cv, _box_rect, pos, paper_color());
        draw_borders(cv, _box_rect, pos, stroke_width(), stroke_color());

        if (_checked)
        {
            cv->render_text(_glyph_font.get(), pos.x + _tick_pos.x, pos.y + _tick_pos.y, &_tick_descr.code_point, 1);
        }
    }

    template<class Config, bool With_layout>
    void Checkbox<Config, With_layout>::mouse_click(const Point &pos, int button, Count count)
    {
        if (_box_rect.contains(pos)) // todo: subtract stroke_width() ?
        {
            _checked = ! _checked;
            if (_state_change_handler) _state_change_handler(_checked);
            invalidate();
        }
        else {
            Widget_t::mouse_click(pos, button, count);
        }
    }

    // Layouter aspect ----------------------------------------------

    template <class Config, class Parent>
    void Checkbox__Layouter<Config, true, Parent>::init_layout()
    {
        compute_em_bounds();
        compute_label_size();
        get_tick_metrics();
    }

    template <class Config, class Parent>
    void Checkbox__Layouter<Config, true, Parent>::compute_em_bounds()
    {
        // TODO: adapt for multi-cultural use

        _em_bounds = p()->font().source()->compute_text_extents(0, U"M", 1);

        //_box_edge = _em_bounds.height() + _padding[3] + _padding[1];
        _box_edge = _em_bounds.height() + 2 * (p()->padding() + p()->stroke_width());
    }

    template <class Config, class Parent>
    void Checkbox__Layouter<Config, true, Parent>::compute_label_size()
    {
        _label_bounds = p()->font().source()->compute_text_extents(0, p()->_label.data(), p()->_label.size());
    }

    template <class Config, class Parent>
    void Checkbox__Layouter<Config, true, Parent>::get_tick_metrics()
    {
        _tick_bounds = p()->_glyph_font.source()->compute_text_extents(0, &p()->_tick_descr.code_point, 1);

        _tick_extents = Extents { _tick_bounds.width(), _tick_bounds.height() };
    }

    template <class Config, class Parent>
    auto Checkbox__Layouter<Config, true, Parent>::get_minimal_size() -> Extents
    {
        // TODO: spacing between label and tick
        return { 
            _label_bounds.width() + /* spacing() + */ _tick_extents.w + 2, 
            std::max(_label_bounds.height(), _em_bounds.height() + 2 * padding() + 2 * p()->stroke_width())
        };
    }

    template <class Config, class Parent>
    void Checkbox__Layouter<Config, true, Parent>::layout()
    {
        // TODO: supporting aligning on a baseline ?

        auto baseline = std::max(_label_bounds.y_max, _em_bounds.y_max + stroke_width() + padding() );
        auto h = static_cast<Length>(baseline + std::max(- _label_bounds.y_min, - _em_bounds.y_min + stroke_width() + padding()));
        baseline += static_cast<Position>((p()->extents().h - h) / 2);

        p()->_label_pos = { 0,  baseline };

        // TODO: non-static glyph adjustment
        auto x = static_cast<Position>(p()->extents().w - _box_edge);
        auto dx = static_cast<Position>((_box_edge - _tick_extents.w) / 2);

        p()->_tick_pos  = { 
            x + dx,
            baseline + p()->_tick_descr.origin_delta.y
        };
        p()->_box_rect = { 
            x, baseline - _em_bounds.y_max - padding() - stroke_width(),
            _box_edge, _box_edge
        };
    }

} // ns cppgui