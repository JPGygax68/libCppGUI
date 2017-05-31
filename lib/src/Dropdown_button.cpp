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

#include "unicode.hpp"
#include "Dropdown_button.hpp"


namespace cppgui {

    void Dropdown_button_base::on_pushed(Pushed_handler handler)
    {
        _on_pushed = handler;
    }

    void Dropdown_button_base::set_font(const Rasterized_font *font)
    {
        _font.assign(font);
    }

    void Dropdown_button_base::set_label(const std::u32string &label)
    {
        _label = label;
    }

    void Dropdown_button_base::set_label(const std::string &label)
    {
        set_label( utf8_to_utf32(label) );
    }

    void Dropdown_button_base::init(Canvas *c)
    {
        _font.translate(c);
    }

    void Dropdown_button_base::render(Canvas *c, const Point &offs)
    {
        auto p = offs + origin();

        //draw_background_and_border(c, p, bounds(), visual_states());

        c->set_text_color(label_color(visual_states()));
        c->render_text(_font.get(), p.x /*+ _x_offs*/, p.y, _label.data(), _label.size());

        if (this->has_focus())
        {
            auto r = Rectangle{_label_bbox};
            r.inflate(2, 2); // TODO: make this configurable somewhere
            c->draw_stippled_rectangle_outline(p.x + r.pos.x, p.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    bool Dropdown_button_base::mouse_click(const Point &p, int button, Count count)
    {
        Widget::mouse_click(p, button, count); // will take focus

        if (button == 1 && count == 1)
        {
            return do_pushed();
        }

        return false;
    }

    bool Dropdown_button_base::key_down(const Keycode &k)
    {
        if (is_return(k))
        {
            return do_pushed();
        }

        return Super::key_down(k);
    }

    bool Dropdown_button_base::do_pushed()
    {
        if (_on_pushed) return _on_pushed();

        return false;
    }

#ifndef CPPGUI_EXCLUDE_LAYOUTING

    void Dropdown_button_base::compute_label_bounds()
    {
        _label_bbox = _font.rasterized->compute_text_extents(0, _label.data(), _label.size());
    }

    void Dropdown_button_base::init_layout()
    {
        compute_label_bounds();
    }

    auto Dropdown_button_base::get_minimal_bounds() -> Bbox
    {
        //return adjust_box_bounds(_label_bbox);
        return _label_bbox;
    }

    void Dropdown_button_base::compute_layout(Bbox_cref b)
    {        
        Widget::compute_layout(b);

        //auto inner_bounds = adjust_box_bounds(b, -1);

        //_x_offs = align_cultural_minor(_label_bbox, inner_bounds, _minor_align);
    }

    #ifdef NOT_DEFINED
    void Dropdown_button_base::set_alignment(Alignment align)
    {
        _minor_align = align;
    }
    #endif

    void Dropdown_button_base::font_changed()
    {
        compute_label_bounds();
        compute_layout(bbox());
        invalidate();
    }

    void Dropdown_button_base::text_changed()
    {
        compute_label_bounds();
        compute_layout(bbox());
        invalidate();
    }

    #endif // CPPGUI_EXCLUDE_LAYOUTING

} // ns cppgui
