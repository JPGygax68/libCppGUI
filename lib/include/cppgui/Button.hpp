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

#include "Text_widget.hpp"
#include "Boxed.hpp"
#include "UI_builder.hpp"


namespace cppgui {

    /** TODO: how to support changing label (and later on, icon) at runtime without mixing
            in the layouting aspect ?
     */
    class Button_base: public Text_widget
    {
    public:

        Button_base() = default;

        explicit Button_base(const std::u32string &l, Alignment a = center)
        {
            set_label(l);
            set_alignment(a);
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
        Pbbox                   _label_pbbox;

    #ifndef CPPGUI_EXCLUDE_LAYOUTING

    public:
        void init_layout() override;
        auto get_minimal_bbox() -> Bbox override;
        void compute_layout(Bbox_cref) override;

        // Extra capabilities coming with layouting
        // TODO
        // void change_font(const Rasterized_font *);
        // void change_label(const std::u32string &);
        void set_alignment(Alignment);

        // Extra properties
        //auto minimal_padding() -> int; // THIS *INCLUDES* THE BORDER WIDTH (unlike the CSS box model!)

        // Interface with main class (Button)

        void font_changed();
        void text_changed();

    protected:
        
        void compute_label_bounds();

        bool do_pushed();

    private:

        Alignment               _alignment = center;
        //Alignment               _major_align = cultural_major_middle;
        
    #endif // CPPGUI_EXCLUDE_LAYOUTING
    };


    using Button = Boxed<Button_base, Button_box_styles>;


#ifndef CPPGUI_EXCLUDE_UI_BUILDER

#ifdef REMOVED_CODE

    /* This UI builder was working, but using the only method it brings to the table requires
     * using begin<>() / end(), which makes this very verbose.
     * Alignment will now be made into an optional constructor parameter.
     */
    template<class ParentBuilderT>
    class UI_builder<Button, ParentBuilderT>: public UI_builder_base2<Button, ParentBuilderT>
    {
    public:

        UI_builder(Button& lb, Widget_bag &wb, ParentBuilderT *pb): 
            UI_builder_base2{lb, wb, pb} 
        {}

        auto& set_alignment(Alignment a)
        {
            this->widget().set_alignment(a);
            return *this;
        }
    };

#endif // REMOVED_CODE

#endif // !CPPGUI_EXCLUDE_UI_BUILDER

} // ns cppgui
