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
            cnv->render_text(_font.get(), pos.x + _text_origin.x, pos.y + _text_origin.y, _text.data(), _text.size());
        }

        if (this->has_focus())
        {
            // TODO: make this into a method of Box<>
            auto r = _text_rect + Extents{ 3, 2 };
            cnv->draw_stippled_rectangle_outline(pos.x + r.pos.x, pos.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    // Layouter aspect ----------------------------------------------

    void Label::init_layout()
    {
        //assert(!p()->text().empty());

        if (!text().empty())
        {
            _bounding_box = font()->compute_text_extents(0, text().data(), text().size() );
        }
        else {
            _bounding_box = {};
        }
    }

    auto Label::get_minimal_size() -> Extents
    {
        //assert(!p()->text().empty());

        if (!text().empty())
        {
            return add_boxing({ _bounding_box.width(), _bounding_box.height() });
        }

        return {};
    }

    void Label::layout()
    {
        // _layout.compute_layout( p()->extents(), Padding{} ); // TODO _padding);

        _text_origin = position_text_element(_bounding_box, _minor_alignment, _major_alignment);

        _text_rect = {
            _text_origin.x + _bounding_box.x_min, _text_origin.y - _bounding_box.y_max,
            _bounding_box.width(), _bounding_box.height()
        };
    }

} // ns cppgui