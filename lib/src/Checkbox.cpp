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
#include "Parent_map.hpp"
#include <cppgui/Container.hpp>
#include <cppgui/Checkbox.hpp>


namespace cppgui {

    //static Parent_map::Registrant<Checkbox, Widget> parent_map_registrant;
    REGISTER_PARENTHOOD(Checkbox, Widget)

    // static std::unique_ptr<Rasterized_font> glyph_font;

    static const char32_t tick_cp = 0x25; //0x2714; // E14C; // 0xE876;

    Checkbox::Checkbox(const std::u32string &label, State_change_handler state_change_handler)
    {
        _glyph_font.assign(&baked_fonts::modernpics_18_font());
        _label = label;
        _state_change_handler = state_change_handler;
    }

    void Checkbox::on_state_change(State_change_handler handler)
    {
        assert(!_state_change_handler);
        _state_change_handler = handler;
    }

    void Checkbox::get_backend_resources(Canvas *c)
    {
        // TODO: define a standardized query on the Resource contract to determine if assigned ?
        if (_label_font.rasterized) _label_font.translate(c);
        _glyph_font.translate(c);
    }

    void Checkbox::render(Canvas *c, const Point & offs)
    {
        auto p = offs + origin();

        // Label
        if (!_label.empty())
        {
            c->render_text(font_handle(), p.x, p.y, _label.data(), _label.size());
        }

        // Box border and background
        auto r = Rectangle{_tick_bbox.expand(tick_padding() + tick_border())} + Point{_tick_orig, 0};
        c->fill_rect(r + p, {0, 0, 0, 1}); // TODO: make configurable
        r.inflate(-tick_border(), -tick_border());
        c->fill_rect(r + p, {1, 1, 1, 1}); // TODO: make configurable

        if (has_focus() && !_label.empty())
        {
            auto r = Rectangle{bbox()};
            r.inflate(3, 2); // TODO: make this configurable somewhere
            c->draw_stippled_rectangle_outline(p.x + r.pos.x, p.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
        // TODO: else ...

        // Tick
        if (_checked)
        {
            c->render_text(_glyph_font.get(), p.x + _tick_orig, p.y, &tick_cp, 1);
        }

        // TODO: stippled rectangle when focused ?
    }

    bool Checkbox::mouse_click(const Point &p, int button, Count count)
    {
        auto r = Rectangle{_tick_bbox} + Point{_tick_orig, 0};

        if (r.contains(p)) // todo: subtract stroke_width() ?
        {
            _checked = ! _checked;
            if (_state_change_handler) _state_change_handler(_checked);
            invalidate();

            return true;
        }

        return Widget::mouse_click(p, button, count);
    }

    #ifndef CPPGUI_EXCLUDE_LAYOUTING

    void Checkbox::init_layout()
    {
        //compute_em_bounds();
        //compute_label_size();
        get_tick_bounds();
    }

    auto Checkbox::get_minimal_bbox() -> Bbox
    {
        Inline_layouter l; // TODO: make a member ?

        l.set_default_spacing(font());

        l.append_text(font(), _label.data(), _label.size());
        l.append_bounding_box(_tick_bbox.expand(tick_padding() + tick_border()), tick_border() > 0);

        return l.minimal_bounds();
    }

    void Checkbox::compute_layout(Bbox_cref b)
    {
        Widget::compute_layout(b);

        // Label position is defined as 0, 0

        // Tick box origin is origin of tick glyph
        _tick_orig = b.x_max - tick_border() - tick_padding() - _tick_bbox.x_max;
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