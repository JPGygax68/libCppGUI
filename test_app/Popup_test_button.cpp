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

#include <cppgui/unicode.hpp>
#include <cppgui/Root_widget.hpp>
#include <cppgui/Internal_popup.hpp>
#include "./Popup_test_button.hpp"


namespace cppgui {

    class Dummy_popup_zone: public Internal_popup
    {
    public:
        using Internal_popup::Internal_popup;

        void render(Canvas *, const Point &offset) override;
    };


    void Popup_test_button_base::set_font(const Rasterized_font *font)
    {
        _font.assign(font);
    }

    void Popup_test_button_base::set_label(const std::u32string &label)
    {
        _label = label;
    }

    void Popup_test_button_base::set_label(const std::string &label)
    {
        set_label( utf8_to_utf32(label) );
    }

    void Popup_test_button_base::init(Canvas *c)
    {
        _font.translate(c);
    }

    void Popup_test_button_base::render(Canvas *c, const Point &offs)
    {
        auto p = offs + origin();

        // draw_background_and_border(c, p, bounds(), visual_states());

        c->set_text_color(label_color(visual_states()));
        c->render_text(_font.get(), p.x /*+ _x_offs*/, p.y, _label.data(), _label.size());

        if (this->has_focus())
        {
            auto r = Rectangle{_label_bbox};
            r.inflate(2, 2); // TODO: make this configurable somewhere
            c->draw_stippled_rectangle_outline(p.x + r.pos.x, p.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    bool Popup_test_button_base::mouse_click(const Point &p, int button, Count count)
    {
        Widget::mouse_click(p, button, count); // will take focus

        if (button == 1 && count == 1)
        {
            toggle_popup();
            return true;
        }

        return false;
    }

    bool Popup_test_button_base::key_down(const Keycode &k)
    {
        if (is_return(k))
        {
            toggle_popup();
            return true;
        }

        return Super::key_down(k);
    }

    void Popup_test_button_base::toggle_popup()
    {
        if (!_popup_zone)
        {
            _popup_zone.reset( new Dummy_popup_zone(this) );
        }
        else
        {
            _popup_zone.reset();
        }
    }

#ifndef CPPGUI_EXCLUDE_LAYOUTING

    void Popup_test_button_base::compute_label_bounds()
    {
        _label_bbox = _font.rasterized->compute_text_extents(0, _label.data(), _label.size());
    }

    void Popup_test_button_base::init_layout()
    {
        compute_label_bounds();
    }

    auto Popup_test_button_base::get_minimal_bbox() -> Bbox
    {
        //return adjust_box_bounds(_label_bbox);
        return _label_bbox;
    }

    void Popup_test_button_base::compute_layout(Bbox_cref b)
    {        
        Widget::compute_layout(b);

        //auto inner_bounds = adjust_box_bounds(b, -1);

        //_x_offs = align_cultural_minor(_label_bbox, inner_bounds, _minor_align);
    }

#ifdef NOT_DEFINED
    void Popup_test_button_base::set_alignment(Alignment align)
    {
        _minor_align = align;
    }
#endif

    void Popup_test_button_base::font_changed()
    {
        compute_label_bounds();
        compute_layout(bbox());
        invalidate();
    }

    void Popup_test_button_base::text_changed()
    {
        compute_label_bounds();
        compute_layout(bbox());
        invalidate();
    }

#endif // CPPGUI_EXCLUDE_LAYOUTING


    void Dummy_popup_zone::render(Canvas *c, const Point & offset)
    {
        c->fill_rect(rectangle(), {1, 1, 0.5f});
    }

} // ns cppgui
