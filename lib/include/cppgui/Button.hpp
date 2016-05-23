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

#include <iostream> // TODO: REMOVE!

#include "./Widget.hpp"
#include "./Box.hpp"

namespace cppgui {

    template <class Config, bool With_layout, class Parent> struct Button__Layouter;

    /** TODO: how to support changing label (and later on, icon) at runtime without mixing
            in the layouting aspect ?
     */
    template <class Config, bool With_layout>
    class Button: 
        public Button__Layouter<Config, With_layout,
            Bordered_box<Config, With_layout, Widget<Config, With_layout> > >
    {
    public:
        using Renderer = typename Config::Renderer;
        using Font_handle = typename Renderer::font_handle;
        using Widget_t = Widget<Config, With_layout>;
        using Parent_t = Widget_t;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Font_resource = typename Widget_t::Font_resource;
        using Pushed_handler = typename Widget_t::Pushed_handler;

        Button()
        {
            // set_border( default_border() ); // TODO: make this stylable
        }

        void on_pushed(Pushed_handler handler)
        {
            _on_pushed = handler;
        }

        void set_font(const Rasterized_font *font)
        {
            _font.assign(font);
        }
        auto font() const { return _font.source(); }
        void set_label(const std::u32string &label)
        {
            _label = label;
        }
        void set_label(const std::string &label)
        {
            set_label( utf8_to_utf32(label) );
        }
        auto label() const { return _label; }

        void init() override
        {
            Widget_t::init();

            _font.translate( root_widget()->canvas() );
        }

        void render(Canvas_t *canvas, const Point &offs) override
        {
            fill(canvas, offs, rgba_to_native( button_face_color() ));

            draw_border(canvas, offs);

            auto pos = offs + position();

            canvas->render_text(_font.get(), pos.x + _label_origin.x, pos.y + _label_origin.y, _label.data(), _label.size());

            if (has_focus())
            {
                // TODO: draw the rectangle along the border instead of around the label ?
                auto r = _label_rect + Extents{3, 3};
                canvas->draw_stippled_rectangle_outline(pos.x + r.pos.x, pos.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
            }
        }

        // TODO: visual feedback on mouse down / up

    protected:

        void mouse_click(const Point &pos, int button, int count) override
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

        Pushed_handler          _on_pushed;
        Font_resource           _font;
        std::u32string          _label;
        Point                   _label_origin;
        Rectangle               _label_rect;
    };

    // Layouter aspect

    class Single_element_layout;

    template <class Config, class Parent>
    struct Button__Layouter<Config, true, Parent>: public Box__Layouter<Config, true, Parent>  
    {
        Button__Layouter() { _padding = this->button_padding(); }

        // Layouter contract

        void init_layout() override
        {
            // TODO: implement configurable alignment ?
            _layout.set_major_alignment(Alignment::cultural_major_middle);
            _layout.set_minor_alignment(Alignment::cultural_minor_middle);
            _layout.set_text_element(p()->font(), p()->_label.data(), p()->_label.size(), & p()->_label_origin, & p()->_label_rect);
        }
        auto get_minimal_size() -> Extents override
        {
            return add_padding( _layout.compute_minimal_size() );
        }
        void layout() override
        {
            _layout.compute_layout( p()->extents() );
        }

        // Extra capabilities coming with layouting
        // TODO
        // void change_font(const Rasterized_font *);
        // void change_label(const std::u32string &);

        // Extra properties
        //auto minimal_padding() -> int; // THIS *INCLUDES* THE BORDER WIDTH (unlike the CSS box model!)

        // Interface with main class (Button)

        void font_changed()
        {
            compute_bounding_box();
            layout();
            this->invalidate();
        }
        void text_changed()
        {
            compute_bounding_box();
            layout();
            this->invalidate();
        }

    protected:
        class Button_t: public Button<Config, true> { friend struct Button__Layouter; };

        auto p() { return static_cast<Button_t*>(static_cast<Button<Config, true>*>(this)); }
        void compute_bounding_box()
        {
            _bbox = p()->_font.source()->compute_text_extents(0, p()->_label.data(), p()->_label.size());
        }

        Text_bounding_box       _bbox;
        Single_element_layout   _layout;
    };

    // Nil implementation (must short out interface with main class)

    template <class Config, class Parent>
    struct Button__Layouter<Config, false, Parent>: public Parent 
    {
        void layout() {} // called from init
        void font_changed() {}
        void text_changed() {}
    };

} // ns cppgui
