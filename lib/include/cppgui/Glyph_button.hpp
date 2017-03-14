#pragma once

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

#include "./Boxed.hpp"
#include "./Widget.hpp"


namespace cppgui {

    // Forward declaractions

    struct Glyph_specifier;

    // Glyph_button declaration 

    class Glyph_button_base: public Widget
    {
    public:

        //static constexpr auto default_padding(int /*dir*/) { return 2; }

        void on_pushed(Pushed_handler);

        void set_font(const Rasterized_font *font) { _label_font.assign(font); }

        void set_label(const std::u32string &label) { _label = label; }

        void set_glyph(const Glyph_specifier &);

        //void enable_border(bool enabled) { _border_enabled = enabled; }

        void init(Canvas *) override;

        void render(Canvas *, const Point &offset) override;

        bool mouse_click(const Point &, int button, Count clicks) override;

    protected:
        //Click_handler                   _on_click;
        Pushed_handler                  _on_pushed;
        Font_resource                   _label_font;
        std::u32string                  _label;
        Font_resource                   _glyph_font;
        char32_t                        _glyph_cp = 0;
        Bounding_box                    _label_bbox = {};
        Position_delta                  _glyph_orig = 0;
        //bool                            _border_enabled = true;

        //Point                           _label_pos;
        //Rectangle                       _label_rect;
        //Rectangle                       _focus_rect;
        //Point                           _glyph_pos;

        #ifndef CPPGUI_EXCLUDE_LAYOUTING

    public:
        void init_layout() override;
        auto get_minimal_bounds() -> Bounding_box override;
        void compute_layout(const Bounding_box &) override;

    private:
        void compute_bounds();

        //Text_bounding_box           _glyph_bounds;
        //Text_bounding_box           _label_bbox;
        //Length                      _glyph_min_edge;
        //Length                      _spacing; // spacing between label and glyph
        Bounding_box                    _glyph_bbox = {};

        #endif // CPPGUI_EXCLUDE_LAYOUTING
    };


    // "Boxed versions

    using Glyph_button = Boxed<Glyph_button_base, Button_box_styles>;

    using Borderless_glyph_button = Boxed<Glyph_button_base, Borderless_button_box_styles>;


} // ns cppui
