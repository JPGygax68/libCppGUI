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

#include <functional>

#include "./Box.hpp"
#include "./Widget.hpp"
#include "./Icon_resources.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    template <class Config, bool With_layout, class Parent> struct Checkbox__Layouter;

    struct Font_icon_descr;

    template <class Config, bool With_layout>
    class Checkbox: 
        public Checkbox__Layouter<Config, With_layout, 
            Widget<Config, With_layout> >
    {
    public:
        using Widget_t      = Widget<Config, With_layout>;
        using Canvas_t      = typename Widget_t::Canvas_t;
        //using Font_handle = typename Widget_t::Font_handle;
        using Font_resource = typename Widget_t::Font_resource;

        using State_change_handler = std::function<void(bool)>;

        void set_font(const Rasterized_font *font) { _label_font.assign(font); }
        auto font() const { return _label_font; }
        void set_tick_glyph(const Rasterized_font *font, const Font_icon_descr &descr)
        {
            _glyph_font.assign( font );
            _tick_descr = descr;
            // TODO: use improved Icon_resources methods
        }
        void set_label(const std::u32string &label) { _label = label; }
        auto label() const { return _label; }
        bool checked() const { return _checked; }
        void set_checked(bool state) { _checked = state; }

        void on_state_change(State_change_handler handler)
        {
            assert(!_state_change_handler);
            _state_change_handler = handler;
        }

        void init() override
        {
            Widget_t::init();

            _label_font.translate( root_widget()->canvas() );
            _glyph_font.translate( root_widget()->canvas() );
        }

        void render(Canvas_t *canvas, const Point &offset) override
        {
            auto pos = offset + position();

            fill(canvas, offset, {1, 1, 0.5f, 1});

            canvas->render_text(_label_font.get(), pos.x + _label_pos.x, pos.y + _label_pos.y, _label.data(), _label.size());

            fill_rect(canvas, _box_rect, pos, paper_color());
            draw_borders(canvas, _box_rect, pos, stroke_width(), stroke_color());

            if (_checked)
            {
                canvas->render_text(_glyph_font.get(), pos.x + _tick_pos.x, pos.y + _tick_pos.y, &_tick_descr.code_point, 1);
            }
        }

        void mouse_click(const Point &pos, int button, int count) override
        {
            if (_box_rect.contains(pos)) // todo: subtract stroke_width() ?
            {
                _checked = ! _checked;
                if (_state_change_handler) _state_change_handler(_checked);
                invalidate();
            }
            else {
                Widget_t::mouse_click(pos, button, count);
            }
        }

    protected: // for access by Layouter aspect
        
        Font_resource           _label_font;
        Font_resource           _glyph_font;    // TODO: rename to _tick_font ?
        Font_icon_descr         _tick_descr;
        std::u32string          _label;
        State_change_handler    _state_change_handler;

        Point                   _label_pos;
        Point                   _tick_pos;
        Rectangle               _box_rect;

    private:
        bool                    _checked;
    };

    template <class Config, class Parent>
    struct Checkbox__Layouter<Config, true, Parent>: public Parent 
    {
        void init_layout() override
        {
            compute_em_bounds();
            compute_label_size();
            get_tick_metrics();
        }
        auto get_minimal_size() -> Extents override
        {
            // TODO: spacing between label and tick
            return { 
                _label_bounds.width() + /* spacing() + */ _tick_extents.w + 2, 
                std::max(_label_bounds.height(), _em_bounds.height() + 2 * padding() + 2 * p()->stroke_width())
            };
        }
        void layout() override
        {
            // TODO: supporting aligning on a baseline ?

            auto baseline = std::max(_label_bounds.y_max, _em_bounds.y_max + stroke_width() + padding() );
            auto h = static_cast<Length>(baseline + std::max(- _label_bounds.y_min, - _em_bounds.y_min + stroke_width() + padding()));
            baseline += static_cast<Position>((p()->extents().h - h) / 2);

            p()->_label_pos = { 0,  baseline };

            // TODO: non-static glyph adjustment
            auto x = static_cast<Position>(p()->extents().w - _box_edge);
            auto dx = static_cast<Position>((_box_edge - _tick_extents.w) / 2);

            p()->_tick_pos  = { 
                x + dx,
                baseline + p()->_tick_descr.origin_delta.y
            };
            p()->_box_rect = { 
                x, baseline - _em_bounds.y_max - padding() - stroke_width(),
                _box_edge, _box_edge
            };
        }

        auto padding() { return 2; }

        // TODO:
        // void change_font();
        // void change_glyph_font();

    private:
        class Checkbox_t: public Checkbox<Config, true> { friend struct Checkbox__Layouter; };
        auto p() { return static_cast<Checkbox_t*>(static_cast<Checkbox<Config, true>*>(this)); }
            
        void compute_em_bounds()
        {
            // TODO: adapt for multi-cultural use

            _em_bounds = p()->font().source()->compute_text_extents(0, U"M", 1);

            //_box_edge = _em_bounds.height() + _padding[3] + _padding[1];
            _box_edge = _em_bounds.height() + 2 * (p()->padding() + p()->stroke_width());
        }
        void compute_label_size()
        {
            _label_bounds = p()->font().source()->compute_text_extents(0, p()->_label.data(), p()->_label.size());
        }
        void get_tick_metrics()
        {
            _tick_bounds = p()->_glyph_font.source()->compute_text_extents(0, &p()->_tick_descr.code_point, 1);

            _tick_extents = Extents { _tick_bounds.width(), _tick_bounds.height() };
        }

        Text_bounding_box       _em_bounds;
        Text_bounding_box       _tick_bounds;
        Extents                 _tick_extents;
        Length                  _box_edge;
        Text_bounding_box       _label_bounds;
    };

} // ns cppgui
