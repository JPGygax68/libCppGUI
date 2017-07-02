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

//#include "./Box_model.hpp"
#include "./Text_widget.hpp"
#include "./Box.hpp"
//#include "./Icon_resources.hpp"


namespace cppgui {

    struct Font_icon_descr;

    class Checkbox: public Text_widget, protected Box<Button_box_styles> // TODO: replace with specific styles
    {
    public:
        using State_change_handler = std::function<void(bool)>;

        Checkbox();

        void set_font(const Rasterized_font *font) { _label_font.assign(font); }
        //auto get_font(Style_element) -> Font_resource & override;
        //void set_tick_glyph(const Rasterized_font *, char32_t codepoint); // TODO: use improved Icon_resources methods
        void set_label(const std::u32string &label) { _label = label; }
        auto label() const { return _label; }
        bool checked() const { return _checked; }
        void set_checked(bool state) { _checked = state; }

        void on_state_change(State_change_handler handler);

        void get_backend_resources(Canvas *) override;

        void render(Canvas *, const Point &offset) override;

        bool mouse_click(const Point &, int button, Count count) override;

    private:

        Font_resource           _label_font;
        Font_resource           _glyph_font;    // TODO: rename to _tick_font ?
        //char32_t                _tick_codepoint;
        std::u32string          _label;
        State_change_handler    _state_change_handler;

        //Point                   _label_pos;
        //Point                   _tick_pos;
        //Rectangle               _box_rect;
        Position                _tick_orig = 0;

        bool                    _checked = false;

        #ifndef CPPGUI_EXCLUDE_LAYOUTING

    public:
        void init_layout() override;
        auto get_minimal_bbox() -> Bbox override;
        void compute_layout(Bbox_cref) override;

        auto tick_padding() const { return 1; }
        auto tick_border() const { return 1; }

        // TODO:
        // void change_font();
        // void change_glyph_font();

    private:
        //void compute_em_bounds();
        //void compute_label_size();
        void get_tick_bounds();

        //Text_bounding_box       _em_bounds;
        //Text_bounding_box       _tick_bounds;
        //Extents                 _tick_extents;
        //Length                  _box_edge;
        //Text_bounding_box       _label_bbox;
        Bbox                    _tick_bbox;

        #endif // CPPGUI_EXCLUDE_LAYOUTING
    };

} // ns cppgui
