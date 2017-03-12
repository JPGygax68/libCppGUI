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

#include <algorithm>

#include <cppgui/fonts/Rasterized_font.hpp>

#include "./Label.hpp"
#include "./Root_widget.hpp"


namespace cppgui {

    void Label::set_font(const Rasterized_font *font)
    {
        _font.assign(font);
    }

    void Label::set_color(const RGBA &color)
    {
        _color = color;
    }

    void Label::set_text(const std::u32string &text)
    {
        _text = text;
    }

    void Label::init(Canvas *c)
    {
        _font.translate(c);
    }

    void Label::render(Canvas *c, const Point &offs)
    {
        auto p = offs + this->position();

        //c->fill_rect(Rectangle{bounds()} + p, background_color(visual_states()));

        if (!_text.empty())
        {
            c->set_text_color(_color);
            c->render_text(_font.get(), p.x, p.y, _text.data(), _text.size());
        }

        if (this->has_focus())
        {
            auto r = Rectangle{bounds()};
            r.inflate(3, 2); // TODO: make this configurable somewhere
            c->draw_stippled_rectangle_outline(p.x + r.pos.x, p.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    // Layouter aspect ----------------------------------------------

    void Label::init_layout()
    {
        // TODO ?
    }

    auto Label::get_minimal_bounds() -> Bounding_box
    {
        if (!text().empty())
        {
            return Bounding_box{_font.rasterized->compute_text_extents(0, _text.data(), _text.size())};
        }

        return {};
    }

    /* void Label::set_bounds(const Point & p, const Bounding_box & b)
    {
        // Nothing to do for now
    } */

} // ns cppgui