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

#include <cassert>
//#include "./Icon_resources.hpp"
#include "layouting.hpp"
#include <cppgui/baked_fonts.hpp>
#include "./Checkbox.hpp"


namespace cppgui {

    // static std::unique_ptr<Rasterized_font> glyph_font;

    static const char32_t tick_cp = 0x25; //0x2714; // E14C; // 0xE876;

    #ifdef NOT_DEFINED

    void Checkbox::set_tick_glyph(const Rasterized_font *font, char32_t codepoint)
    {
        _glyph_font.assign( font );
        _tick_codepoint = codepoint;
    }

    #endif

    Checkbox::Checkbox()
    {
        _glyph_font.assign(&baked_fonts::default_symbol_font());
    }

    void Checkbox::on_state_change(State_change_handler handler)
    {
        assert(!_state_change_handler);
        _state_change_handler = handler;
    }

    void Checkbox::init(Canvas *c)
    {
        _label_font.translate(c);
        _glyph_font.translate(c);
    }

    void Checkbox::render(Canvas *c, const Point & offs)
    {
        auto p = offs + position();

        // Label
        c->render_text(_label_font.get(), p.x, p.y, _label.data(), _label.size());

        // Box border and background
        auto r = Rectangle{_tick_bbox.expand(tick_padding() + tick_border())} + Point{_tick_orig, 0};
        c->fill_rect(r + p, {0, 0, 0, 1}); // TODO: make configurable
        r.inflate(-tick_border(), -tick_border());
        c->fill_rect(r + p, {1, 1, 1, 1}); // TODO: make configurable

        // Tick
        if (_checked)
        {
            c->render_text(_glyph_font.get(), p.x + _tick_orig, p.y, &tick_cp, 1);
        }

        // TODO: stippled rectangle when focused ?
    }

    void Checkbox::mouse_click(const Point &p, int button, Count count)
    {
        auto r = Rectangle{_tick_bbox} + Point{_tick_orig, 0};

        if (r.contains(p)) // todo: subtract stroke_width() ?
        {
            _checked = ! _checked;
            if (_state_change_handler) _state_change_handler(_checked);
            invalidate();
        }
        else {
            Widget::mouse_click(p, button, count);
        }
    }

    #ifndef CPPGUI_EXCLUDE_LAYOUTING

    void Checkbox::init_layout()
    {
        //compute_em_bounds();
        //compute_label_size();
        get_tick_bounds();
    }

    auto Checkbox::get_minimal_bounds() -> Bounding_box
    {
        Inline_layouter l; // TODO: make a member ?

        l.set_default_spacing(_label_font.rasterized);

        l.append_text(_label_font.rasterized, _label.data(), _label.size());
        l.append_bounding_box(_tick_bbox.expand(tick_padding() + tick_border()), tick_border() > 0);

        return l.minimal_bounds();

        #ifdef NOT_DEFINED
        // TODO: spacing between label and tick
        return { 
            _label_bbox.width() + /* spacing() + */ _tick_extents.w + 2, 
            std::max(_label_bbox.height(), _em_bounds.height() + 2 * padding() + 2 * p()->stroke_width())
        };
        #endif
    }

    void Checkbox::compute_layout(const Bounding_box &b)
    {
        Widget::compute_layout(b);

        // Label position is defined as 0, 0

        // Tick box origin is origin of tick glyph
        _tick_orig = b.x_max - tick_border() - tick_padding() - _tick_bbox.x_max;

        #ifdef NOT_DEFINED

        auto baseline = std::max(_label_bbox.y_max, _em_bounds.y_max + this->stroke_width() + padding() );
        auto h = static_cast<Length>(baseline + std::max(- _label_bbox.y_min, - _em_bounds.y_min + this->stroke_width() + padding()));
        baseline += static_cast<Position>((extents().h - h) / 2);

        _label_pos = { 0,  baseline };

        // TODO: non-static glyph adjustment
        auto x = static_cast<Position>(extents().w - _box_edge);
        auto dx = static_cast<Position>((_box_edge - _tick_extents.w) / 2);

        _tick_pos  = { 
            x + dx,
            baseline + _tick_descr.origin_delta.y
        };
        _box_rect = { 
            x, baseline - _em_bounds.y_max - padding() - this->stroke_width(),
            _box_edge, _box_edge
        };

        #endif
    }

    #ifdef NOT_DEFINED

    void Checkbox::compute_em_bounds()
    {
        // TODO: adapt for multi-cultural use

        _em_bounds = font().rasterized->compute_text_extents(0, U"M", 1);

        //_box_edge = _em_bounds.height() + _padding[3] + _padding[1];
        _box_edge = _em_bounds.height() + 2 * (padding() + stroke_width());
    }

    void Checkbox::compute_label_size()
    {
        _label_bbox = font().rasterized->compute_text_extents(0, _label.data(), _label.size());
    }

    #endif // NOT_DEFINED

    void Checkbox::get_tick_bounds()
    {
        _tick_bbox = _glyph_font.rasterized->compute_text_extents(0, &tick_cp, 1); // TODO: use lookup_glyph() instead
        // _tick_bbox = _glyph_font.rasterized->lookup_glyph(0, 0xE876)->cbox.bounds;
    }

    #endif // CPPGUI_EXCLUDE_LAYOUTING

} // ns cppgui