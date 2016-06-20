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

#include <gpc/fonts/rasterized_font.hpp>

#include "./Label.hpp"

#include "./Widget.ipp"
#include "./Box.ipp"

namespace cppgui {

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::set_font(const Rasterized_font *font)
    {
        _font.assign(font);
    }

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::set_text(const std::u32string &text)
    {
        _text = text;
    }

    template<class Config, bool With_layout>
    void Label<Config, With_layout>::init()
    {
        _font.translate( root_widget()->canvas() );
    }

    template<class Config, bool WithLayout>
    inline void Label<Config, WithLayout>::render(Canvas_t *cnv, const Point &offs)
    {
        fill(cnv, offs, rgba_to_native(background_color())); 

        auto pos = offs + position();
        cnv->render_text(_font.get(), pos.x + _text_origin.x, pos.y + _text_origin.y, _text.data(), _text.size());

        if (has_focus())
        {
            // TODO: make this into a method of Box<>
            auto r = _text_rect + Extents{ 3, 2 };
            cnv->draw_stippled_rectangle_outline(pos.x + r.pos.x, pos.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    // Layouter aspect ----------------------------------------------

    template<class Config, class Parent>
    Label__Layouter<Config, true, Parent>::Label__Layouter()
    {
        // set_padding( default_padding() );
    }

    template<class Config, class Parent>
    void Label__Layouter<Config, true, Parent>::init_layout()
    {
        assert(!p()->text().empty());

        this->_bounding_box = p()->font()->compute_text_extents(0, p()->text().data(), p()->text().size() );
    }

    template<class Config, class Parent>
    auto Label__Layouter<Config, true, Parent>::get_minimal_size() -> Extents
    {
        assert(!p()->text().empty());

        return p()->add_boxing({ _bounding_box.width(), _bounding_box.height() });
    }

    template<class Config, class Parent>
    void Label__Layouter<Config, true, Parent>::layout()
    {
        // _layout.compute_layout( p()->extents(), Padding{} ); // TODO _padding);

        p()->_text_origin = this->position_text_element(this->_bounding_box, this->_minor_alignment, this->_major_alignment);

        p()->_text_rect = {
            p()->_text_origin.x + this->_bounding_box.x_min, p()->_text_origin.y - this->_bounding_box.y_max,
            this->_bounding_box.width(), this->_bounding_box.height()
        };
    }

} // ns cppgui