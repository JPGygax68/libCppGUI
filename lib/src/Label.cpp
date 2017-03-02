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

    void Label::set_text(const std::u32string &text)
    {
        _text = text;
    }

    void Label::init()
    {
        _font.translate( root_widget()->canvas() );
    }

    void Label::render(Canvas *cnv, const Point &offs)
    {
        fill(cnv, offs, this->rgba_to_native(background_color())); 

        auto pos = offs + this->position();

        if (!_text.empty())
        {
            cnv->render_text(_font.get(), pos.x, pos.y, _text.data(), _text.size());
        }

        if (this->has_focus())
        {
            auto r = Rectangle{bounds()};
            r.inflate(3, 2); // TODO: make this configurable somewhere
            cnv->draw_stippled_rectangle_outline(pos.x + r.pos.x, pos.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
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
            return Bounding_box{_font.source()->compute_text_extents(0, _text.data(), _text.size())};
        }

        return {};
    }

    void Label::layout()
    {
        // Nothing to do for now
    }

} // ns cppgui