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
//#include "./Box_model.hpp"

namespace cppgui {

    // Forward declaractions

    struct Icon_glyph;

    template<class Config, bool With_layout> 
    struct Glyph_button__Layouter
    {
        template<class Class, class Parent>
        struct Aspect: Parent {};
    };

    // Internal stuff

    template<class Config>
    struct _glyph_button
    {
        //template<class Parent>
        //using Default_box_model = Fixed_border_and_padding_box_model<1, 3, Parent>; // TODO: take DPI into account
    };

    // Glyph_button declaration 

    template<class Config, bool With_layout, Box_model_definition BMDef>
    class Glyph_button: public 
        Glyph_button__Layouter<Config, With_layout>::template Aspect< Glyph_button<Config, With_layout, BMDef>, 
        Box_model<Config, With_layout, BMDef>::template Aspect< Glyph_button<Config, With_layout, BMDef>,
        Widget<Config, With_layout> > >
    {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Font_handle = typename Widget_t::Font_handle;
        using Pushed_handler = typename Widget_t::Pushed_handler;
        using Font_resource = typename Widget_t::Font_resource;

        static constexpr auto default_padding(int /*dir*/) { return 2; }

        void on_pushed(Pushed_handler);

        void set_font(const Rasterized_font *font) { _label_font = font; }

        void set_label(const std::u32string &label) { _label = label; }

        void set_glyph(const Icon_glyph &);

        //void enable_border(bool enabled) { _border_enabled = enabled; }

        void init() override;

        void render(Canvas_t *, const Point &offset) override;

        void mouse_click(const Point &, int button, Count clicks) override;

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

    template<class Config>
    struct Glyph_button__Layouter<Config, true>
    {
        template<class Class, class Parent>
        struct Aspect: Parent
        {
            // Glyph_button__Layouter() { _padding = this->button_padding(); }

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

            // TODO:
            // void change_glyph();

        private:
            struct Main: public Class { friend struct Glyph_button__Layouter; };
            auto p() { return static_cast<Main*>(this); }

            void compute_sizes();

            Text_bounding_box           _glyph_bounds;
            Text_bounding_box           _label_bounds;
            Length                      _glyph_min_edge;
            Length                      _spacing; // spacing between label and glyph
        };
    };

} // ns cppui
