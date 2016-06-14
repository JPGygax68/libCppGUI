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

    template<class Config, bool With_layout, template<class> class BoxModel>
    Button<Config, With_layout, BoxModel>::Button()
    {
        // set_border( default_border() ); // TODO: make this stylable
    }

    template<class Config, bool With_layout, template<class> class BoxModel>
    void Button<Config, With_layout, BoxModel>::on_pushed(Pushed_handler handler)
    {
        _on_pushed = handler;
    }

    template<class Config, bool With_layout, template<class> class BoxModel>
    void cppgui::Button<Config, With_layout, BoxModel>::set_font(const Rasterized_font *font)
    {
        _font.assign(font);
    }

    template<class Config, bool With_layout, template<class> class BoxModel>
    void Button<Config, With_layout, BoxModel>::set_label(const std::u32string &label)
    {
        _label = label;
    }

    template<class Config, bool With_layout, template<class> class BoxModel>
    void Button<Config, With_layout, BoxModel>::set_label(const std::string &label)
    {
        set_label( utf8_to_utf32(label) );
    }

    template<class Config, bool With_layout, template<class> class BoxModel>
    void Button<Config, With_layout, BoxModel>::init()
    {
        _font.translate( this->root_widget()->canvas() );
    }

    template<class Config, bool With_layout, template<class> class BoxModel>
    void Button<Config, With_layout, BoxModel>::render(Canvas_t *canvas, const Point &offs)
    {
        fill(canvas, offs, Canvas_t::rgba_to_native( this->button_face_color() ));

        auto pos = offs + this->position();

        draw_border(canvas, pos);

        canvas->render_text(_font.get(), pos.x + _label_origin.x, pos.y + _label_origin.y, _label.data(), _label.size());

        if (this->has_focus())
        {
            // TODO: draw the rectangle along the border instead of around the label ?
            auto r = _label_rect + Extents{ 3, 3 };
            canvas->draw_stippled_rectangle_outline(pos.x + r.pos.x, pos.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    template<class Config, bool With_layout, template<class> class BoxModel>
    void Button<Config, With_layout, BoxModel>::mouse_click(const Point &pos, int button, Count count)
    {
        Widget_t::mouse_click(pos, button, count); // will take focus

        if (button == 1 && count == 1)
        {
            if (_on_pushed) _on_pushed();
        }
        else 
        {
            Parent_t::mouse_click(pos, button, count);
        }
    }

    // Layouter -----------------------------------------------------

    template<class Config, template<class> class BoxModel, class Parent>
    void Button__Layouter<Config, true, BoxModel, Parent>::init_layout()
    {
        // TODO: implement configurable alignment ?
        _layout.set_major_alignment(Alignment::cultural_major_middle);
        _layout.set_minor_alignment(Alignment::cultural_minor_middle);
        _layout.set_text_element(p()->font(), p()->_label.data(), p()->_label.size(), & p()->_label_origin, & p()->_label_rect);
    }

    template<class Config, template<class> class BoxModel, class Parent>
    auto Button__Layouter<Config, true, BoxModel, Parent>::get_minimal_size() -> Extents
    {
        auto bbox = p()->font()->compute_text_extents(0, p()->_label.data(), p()->_label.size() );

        return p()->add_boxing({ bbox.width(), bbox.height() });
    }

    template<class Config, template<class> class BoxModel, class Parent>
    void Button__Layouter<Config, true, BoxModel, Parent>::layout()
    {
        _layout.compute_layout( p()->extents() );
    }

    template<class Config, template<class> class BoxModel, class Parent>
    void Button__Layouter<Config, true, BoxModel, Parent>::font_changed()
    {
        compute_bounding_box();
        layout();
        this->invalidate();
    }

    template<class Config, template<class> class BoxModel, class Parent>
    void Button__Layouter<Config, true, BoxModel, Parent>::text_changed()
    {
        compute_bounding_box();
        layout();
        this->invalidate();
    }

    template<class Config, template<class> class BoxModel, class Parent>
    void Button__Layouter<Config, true, BoxModel, Parent>::compute_bounding_box()
    {
        _bbox = p()->_font.source()->compute_text_extents(0, p()->_label.data(), p()->_label.size());
    }

} // ns cppgui