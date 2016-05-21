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

#include <string>

#include "./Widget.hpp"
#include "./Box.hpp"

namespace cppgui {

    // Forward declaractions

    struct Icon_glyph;

    template <class Config, bool With_layout, class Parent> struct Glyph_button__Layouter;

    // Glyph_button declaration 

    template<class Config, bool With_layout>
    class Glyph_button: 
        public Glyph_button__Layouter<Config, With_layout,
            Bordered_box<Config, With_layout, 
                Widget<Config, With_layout> > >
    {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Font_handle = typename Widget_t::Font_handle;
        using Pushed_handler = typename Widget_t::Pushed_handler;
        using Font_resource = typename Widget_t::Font_resource;

        void on_pushed(Pushed_handler handler)
        {
            _on_pushed = handler;
        }

        void set_font(const Rasterized_font *font) { _label_font = font; }

        void set_label(const std::u32string &label) { _label = label; }

        void set_glyph(const Icon_glyph &icgl)
        {
            _glyph_font.assign( gpc::fonts::get(icgl.data_store) );
            _glyph_cp  = icgl.code_point;
        }

        //void enable_border(bool enabled) { _border_enabled = enabled; }

        void init() override
        {
            Widget_t::init();

            if (_label_font.source()) _label_font.translate( root_widget()->canvas() );

            if (_glyph_cp)
            {
                _glyph_font.translate( root_widget()->canvas() );
            }
        }

        void render(Canvas_t *canvas, const Point &offset) override
        {
            // Background
            fill(canvas, offset, rgba_to_native( button_face_color() ));

            // Border
            /*
            if (_border_enabled)
            {
            auto border_ntvclr = rgba_to_native( button_border_color() );
            draw_borders(canvas, rectangle(), offset, button_border_width(), border_ntvclr, border_ntvclr, border_ntvclr, border_ntvclr);
            }
            */
            draw_border(canvas, offset);

            auto pos = offset + position();

            // Label
            if (!_label.empty())
            {
                canvas->render_text(_label_font.get(), pos.x + _label_pos.x, pos.y + _label_pos.y, _label.data(), _label.size());
            }

            // Glyph
            if (_glyph_cp)
            {
                canvas->render_text(_glyph_font.get(), pos.x + _glyph_pos.x, pos.y + _glyph_pos.y, &_glyph_cp, 1);
            }

            if (has_focus())
            {
                // TODO: draw the rectangle along the border instead of around the label ?
                auto &r = _focus_rect; // _label_rect.grow({3, 3});
                canvas->draw_stippled_rectangle_outline(pos.x + r.pos.x, pos.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
            }
        }

        void mouse_button(const Point &point, int button, Key_state state) override
        {
            if (button == 1 && state == Key_state::pressed)
            {
                if (_on_pushed) _on_pushed();
            }

            Widget_t::mouse_button(point, button, state);
        }

    protected:
        //Click_handler                   _on_click;
        Pushed_handler                  _on_pushed;
        Font_resource                   _label_font;
        std::u32string                  _label;
        Font_resource                   _glyph_font;
        char32_t                        _glyph_cp;
        //bool                            _border_enabled = true;

        Point                           _label_pos;
        //Rectangle                       _label_rect;
        Rectangle                       _focus_rect;
        Point                           _glyph_pos;
    };

    // Layouter aspect

    template <class Config, class Parent>
    struct Glyph_button__Layouter<Config, true, Parent>: 
        public Box__Layouter<Config, true, Parent>
    {
        Glyph_button__Layouter() { _padding = this->button_padding(); }

        void init_layout() override
        {
            compute_sizes();
        }
        auto get_minimal_size() -> Extents override
        {
            return { 
                _padding[3] + _label_bounds.width () + _spacing + _glyph_min_edge + _padding[1], 
                _padding[0] + std::max(_label_bounds.height(), _glyph_min_edge)   + _padding[2]
            };
        }
        void layout() override
        {
            auto ext = p()->extents();

            if (!p()->_label.empty())
            {
                p()->_label_pos = { 
                    (Position) _padding[3], 
                    (Position) ((ext.h - _label_bounds.height()) / 2) + _label_bounds.y_max 
                };

                /* p()->_label_rect = {
                p()->_label_pos.x, p()->_label_pos.y - _label_bounds.y_max,
                _label_bounds.width(), _label_bounds.height()
                }; */

                p()->_glyph_pos = {
                    (Position) (ext.w - _padding[1] - _glyph_min_edge),
                    (Position) ((ext.h - _glyph_min_edge) / 2) + _glyph_bounds.y_max
                };

            }
            else {
                p()->_glyph_pos = {
                    (Position) ((ext.w - _glyph_bounds.width ()) / 2), // - _glyph_bounds.x_min,
                    (Position) ((ext.h - _glyph_bounds.height()) / 2) + _glyph_bounds.y_max
                };
            }

            // Focus rectangle
            // TODO: stylesheet control
            p()->_focus_rect = add_padding( Rectangle{ p()->extents() } );
        }

        // TODO:
        // void change_glyph();

    private:
        class Glyph_button_t: public Glyph_button<Config, true> { friend struct Glyph_button__Layouter; };
        auto p() { return static_cast<Glyph_button_t*>(static_cast<Glyph_button<Config, true>*>(this)); }

        void compute_sizes()
        {
            if (p()->_glyph_cp)
            {
                _glyph_bounds = p()->_glyph_font.source()->lookup_glyph(0, p()->_glyph_cp)->cbox.bounds;
                _glyph_min_edge = std::max(_glyph_bounds.width(), _glyph_bounds.height());
            }
            else {
                _glyph_bounds = {};
                _glyph_min_edge = 0;
            }

            if (!p()->_label.empty())
            {
                _label_bounds = p()->_label_font.source()->compute_text_extents(0, p()->_label.data(), p()->_label.size());
                _spacing = _glyph_min_edge > 0 ? 4 : 0; // TODO: replace with value based on EM, or stylesheet setting
            }
            else {
                _label_bounds = {};
                _spacing = 0;
            }
        }

        Text_bounding_box           _glyph_bounds;
        Text_bounding_box           _label_bounds;
        Length                      _glyph_min_edge;
        Length                      _spacing; // spacing between label and glyph
    };


} // ns cppui

#define CPPGUI_INSTANTIATE_GLYPH_BUTTON(Config, With_layout) \
    template cppgui::Glyph_button<Config, With_layout>;
