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

#include <fonts/store.hpp>
#include <Icon_resources.hpp>
#include <layouting.hpp>
#include <Glyph_button.hpp>


namespace cppgui {

    // Main class implementation ------------------------------------

    void Glyph_button::on_pushed(Pushed_handler handler)
    {
        _on_pushed = handler;
    }

    void Glyph_button::set_glyph(const Glyph_specifier &g)
    {
        _glyph_font.assign(g.font);
        _glyph_cp  = g.code_point;
    }

    void Glyph_button::init(Canvas *c)
    {
        if (_label_font.rasterized) _label_font.translate(c);
        if (_glyph_cp) _glyph_font.translate(c);
    }

    void Glyph_button::render(Canvas *c, const Point & offset)
    {
        auto p = offset + this->position();

        draw_background_and_border(c, p, bounds(), visual_states());

        // Label
        if (!_label.empty())
        {
            c->render_text(_label_font.get(), p.x, p.y, _label.data(), _label.size());
        }

        // Glyph
        if (_glyph_cp)
        {
            c->render_text(_glyph_font.get(), p.x + _glyph_orig, p.y, &_glyph_cp, 1);
        }

        if (this->has_focus())
        {
            auto r = Rectangle{_label_bbox};
            r.inflate(2, 2); // TODO: make this configurable somewhere
            c->draw_stippled_rectangle_outline(p.x + r.pos.x, p.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    void Glyph_button::mouse_click(const Point & /*point*/, int button, Count /*count*/)
    {
        if (_on_pushed && button == 1) _on_pushed();
    }

    // Layouter aspect ----------------------------------------------

 #ifndef CPPGUI_EXCLUDE_LAYOUTING

    void Glyph_button::init_layout()
    {
        compute_bounds();
    }

    auto Glyph_button::get_minimal_bounds() -> Bounding_box
    {
        Inline_layouter l; // TODO: make a member ?

        if (_label_font.rasterized) l.set_default_spacing(_label_font.rasterized);

        l.append_text(_label_font.rasterized, _label.data(), _label.size());
        l.append_space(); // necessary to make room for stippled "focus" rectangle
        l.append_glyph(_glyph_font.rasterized, _glyph_cp);

        return adjust_box_bounds(l.minimal_bounds());
    }

    void Glyph_button::set_bounds(const Point &p, const Bounding_box &b)
    {
        Widget::set_bounds(p, b);

        auto inner_bbox = adjust_box_bounds(b, -1);

        // Label position is defined as 0, 0

        // Glyph origin
        _glyph_orig = inner_bbox.x_max - _glyph_bbox.x_max; // TODO: better way ? x_max contains anti-aliasing pixels
    }

    void Glyph_button::compute_bounds()
    {
        _label_bbox = _label_font.rasterized->compute_text_extents(0, _label.data(), _label.size());
        _glyph_bbox = _glyph_font.rasterized->compute_text_extents(0, &_glyph_cp, 1);
    }
    
    #endif // CPPGUI_EXCLUDE_LAYOUTING

} // ns cppgui
