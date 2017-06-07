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

#include "./Text_widget.hpp"
#include "./Boxed.hpp"


namespace cppgui {

    /** TODO: how to support changing label (and later on, icon) at runtime without mixing
            in the layouting aspect ?
     */
    class Button_base: public Text_widget
    {
    public:

        Button_base() = default;

        explicit Button_base(const std::u32string &label, const Rasterized_font *font)
        {
            set_label(label);
            set_font(font);
        }

        void on_pushed(Pushed_handler);

        void set_label(const std::u32string &);
        void set_label(const std::string &);
        auto label() const { return _label; }

        void render(Canvas *, const Point &) override;

        bool mouse_click(const Point &pos, int button, Count count) override;
        bool key_down(const Keycode &) override;

        // TODO: visual feedback on mouse down / up

    protected:

        virtual auto label_color(Widget_states) -> RGBA const { return {0, 0, 0, 1}; }

        Pushed_handler          _on_pushed;
        RGBA                    _color = {0, 0, 0, 1};
        std::u32string          _label;
        Bbox                    _label_bbox;

        #ifndef CPPGUI_EXCLUDE_LAYOUTING

    public:
        void init_layout() override;
        auto get_minimal_bbox() -> Bbox override;
        void compute_layout(Bbox_cref) override;

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

        bool do_pushed();

    private:
        using Super = Widget;

        //Alignment               _minor_align = cultural_minor_middle;
        //Alignment               _major_align = cultural_major_middle;
        //Text_bounding_box       _label_bbox;
        
        #endif // CPPGUI_EXCLUDE_LAYOUTING
    };


    using Button = Boxed<Button_base, Button_box_styles>;

} // ns cppgui
