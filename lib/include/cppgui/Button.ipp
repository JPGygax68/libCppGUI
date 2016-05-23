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

#include "./layouting.hpp"
#include "./unicode.hpp"

#include "./Button.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    Button<Config, With_layout>::Button()
    {
        // set_border( default_border() ); // TODO: make this stylable
    }

    template<class Config, bool With_layout>
    void Button<Config, With_layout>::on_pushed(Pushed_handler handler)
    {
        _on_pushed = handler;
    }

    template<class Config, bool With_layout>
    inline void cppgui::Button<Config, With_layout>::set_font(const Rasterized_font *font)
    {
        _font.assign(font);
    }

    template<class Config, bool With_layout>
    void Button<Config, With_layout>::set_label(const std::u32string &label)
    {
        _label = label;
    }

    template<class Config, bool With_layout>
    void Button<Config, With_layout>::set_label(const std::string &label)
    {
        set_label( utf8_to_utf32(label) );
    }

    template<class Config, bool With_layout>
    void Button<Config, With_layout>::init()
    {
        _font.translate( root_widget()->canvas() );
    }

    template<class Config, bool With_layout>
    void Button<Config, With_layout>::render(Canvas_t *cnv, const Point &offs)
    {
        fill(cnv, offs, rgba_to_native( button_face_color() ));

        draw_border(cnv, offs);

        auto pos = offs + position();

        cnv->render_text(_font.get(), pos.x + _label_origin.x, pos.y + _label_origin.y, _label.data(), _label.size());

        if (has_focus())
        {
            // TODO: draw the rectangle along the border instead of around the label ?
            auto r = _label_rect + Extents{ 3, 3 };
            cnv->draw_stippled_rectangle_outline(pos.x + r.pos.x, pos.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    template<class Config, bool With_layout>
    void Button<Config, With_layout>::mouse_click(const Point &pos, int button, int count)
    {
        if (button == 1 && count == 1)
        {
            if (_on_pushed) _on_pushed();
        }
        else 
        {
            Parent_t::mouse_click(pos, button, count);
        }

        Widget_t::mouse_click(pos, button, count); // will take focus
    }

    // Layouter -----------------------------------------------------

    template<class Config, class Parent>
    void Button__Layouter<Config, true, Parent>::init_layout()
    {
        // TODO: implement configurable alignment ?
        _layout.set_major_alignment(Alignment::cultural_major_middle);
        _layout.set_minor_alignment(Alignment::cultural_minor_middle);
        _layout.set_text_element(p()->font(), p()->_label.data(), p()->_label.size(), & p()->_label_origin, & p()->_label_rect);
    }

    template<class Config, class Parent>
    auto Button__Layouter<Config, true, Parent>::get_minimal_size() -> Extents
    {
        return add_padding( _layout.compute_minimal_size() );
    }

    template<class Config, class Parent>
    void Button__Layouter<Config, true, Parent>::layout()
    {
        _layout.compute_layout( p()->extents() );
    }

    template<class Config, class Parent>
    inline void Button__Layouter<Config, true, Parent>::font_changed()
    {
        compute_bounding_box();
        layout();
        this->invalidate();
    }

    template<class Config, class Parent>
    inline void Button__Layouter<Config, true, Parent>::text_changed()
    {
        compute_bounding_box();
        layout();
        this->invalidate();
    }

    template<class Config, class Parent>
    void Button__Layouter<Config, true, Parent>::compute_bounding_box()
    {
        _bbox = p()->_font.source()->compute_text_extents(0, p()->_label.data(), p()->_label.size());
    }

} // ns cppgui