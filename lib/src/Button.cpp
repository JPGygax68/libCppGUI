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

#include "unicode.hpp"
#include "Button.hpp"


namespace cppgui {

    void Button::on_pushed(Pushed_handler handler)
    {
        _on_pushed = handler;
    }

    void Button::set_font(const Rasterized_font *font)
    {
        _font.assign(font);
    }

    void Button::set_label(const std::u32string &label)
    {
        _label = label;
    }

    void Button::set_label(const std::string &label)
    {
        set_label( utf8_to_utf32(label) );
    }

    void Button::init(Canvas *c)
    {
        _font.translate(c);
    }

    void Button::render(Canvas *c, const Point &offs)
    {
        auto p = offs + position();

        draw_background_and_border(c, p, bounds(), visual_states());

        c->render_text(_font.get(), p.x /*+ _x_offs*/, p.y, _label.data(), _label.size());

        if (this->has_focus())
        {
            auto r = Rectangle{bounds()};
            r.inflate(3, 3); // TODO: make this configurable somewhere
            c->draw_stippled_rectangle_outline(p.x + r.pos.x, p.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    void Button::mouse_click(const Point &p, int button, Count count)
    {
        Widget::mouse_click(p, button, count); // will take focus

        if (button == 1 && count == 1)
        {
            if (_on_pushed) _on_pushed();
        }
    }

    #ifndef CPPGUI_EXCLUDE_LAYOUTING

    void Button::compute_label_bounds()
    {
        _label_bbox = _font.rasterized->compute_text_extents(0, _label.data(), _label.size());
    }

    void Button::init_layout()
    {
        compute_label_bounds();
    }

    auto Button::get_minimal_bounds() -> Bounding_box
    {
        return adjust_box_bounds(_label_bbox);
    }

    void Button::set_bounds(const Point &p, const Bounding_box &b)
    {        
        Widget::set_bounds(p, b);

        //auto inner_bounds = adjust_box_bounds(b, -1);

        //_x_offs = align_cultural_minor(_label_bbox, inner_bounds, _minor_align);
    }

    #ifdef NOT_DEFINED
    void Button::set_alignment(Alignment align)
    {
        _minor_align = align;
    }
    #endif

    void Button::font_changed()
    {
        compute_label_bounds();
        set_bounds(position(), bounds());
        invalidate();
    }

    void Button::text_changed()
    {
        compute_label_bounds();
        set_bounds(position(), bounds());
        invalidate();
    }

    #endif // CPPGUI_EXCLUDE_LAYOUTING

} // ns cppgui