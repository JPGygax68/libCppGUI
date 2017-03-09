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

#include "./Widget.hpp"
#include "./Box.hpp"


namespace cppgui {

    /** TODO: how to support changing label (and later on, icon) at runtime without mixing
            in the layouting aspect ?
     */
    class Button: public Widget, public Box<Button_box_styles>
    {
    public:

        void on_pushed(Pushed_handler);

        void set_font(const Rasterized_font *);
        auto font() const { return _font.rasterized; }
        void set_label(const std::u32string &);
        void set_label(const std::string &);
        auto label() const { return _label; }

        void init(Canvas *) override;

        void render(Canvas *, const Point &) override;

        // TODO: visual feedback on mouse down / up

    protected:

        void mouse_click(const Point &pos, int button, Count count) override;

        Pushed_handler          _on_pushed;
        Font_resource           _font;
        std::u32string          _label;
        Bounding_box            _label_bbox;

        #ifndef CPPGUI_EXCLUDE_LAYOUTING

    public:
        void init_layout() override;
        auto get_minimal_bounds() -> Bounding_box override;
        void set_bounds(const Point &, const Bounding_box &) override;

        // Extra capabilities coming with layouting
        // TODO
        // void change_font(const Rasterized_font *);
        // void change_label(const std::u32string &);
        //void set_alignment(Alignment);

        // Extra properties
        //auto minimal_padding() -> int; // THIS *INCLUDES* THE BORDER WIDTH (unlike the CSS box model!)

        // Interface with main class (Button)

        void font_changed();
        void text_changed();

    protected:
        void compute_label_bounds();

    private:
        //Alignment               _minor_align = cultural_minor_middle;
        //Alignment               _major_align = cultural_major_middle;
        //Text_bounding_box       _label_bbox;
        
        #endif // CPPGUI_EXCLUDE_LAYOUTING

    };

} // ns cppgui
