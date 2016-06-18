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
#include "./Box_model.hpp"
#include "./Box.hpp"

namespace cppgui {

    template<class Class, class Config, bool With_layout, class Parent> struct Button__Layouter;

    // Internal stuff

    template<class Config>
    struct _button
    {
        template<class Parent>
        using Default_box_model = Fixed_border_and_padding_box_model<1, 3, Parent>; // TODO: take DPI into account
    };

    #ifdef NOT_DEFINED

    #define CPPGUI_INSTANTIATE_BUTTON(Config, With_layout, ...) \
        template cppgui::Button<Config, With_layout, __VA_ARGS__>; \
        template cppgui::Button__Layouter<Config, With_layout, __VA_ARGS__, \
            cppgui::Box<Config, With_layout, \
                __VA_ARGS__< cppgui::Widget<Config, With_layout> > > >;
    
    #endif

    /** TODO: how to support changing label (and later on, icon) at runtime without mixing
            in the layouting aspect ?
     */
    template <class Config, bool With_layout, template<class> class BoxModel = _button<Config>::Default_box_model>
    class Button: public 
        Button__Layouter<Button<Config, With_layout, BoxModel>, Config, With_layout,
            Box<Config, With_layout, 
                BoxModel< Widget<Config, With_layout> > > >
    {
    public:
        using Renderer = typename Config::Renderer;
        using Font_handle = typename Renderer::font_handle;
        using Widget_t = Widget<Config, With_layout>;
        using Parent_t = Widget_t;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Font_resource = typename Widget_t::Font_resource;
        using Pushed_handler = typename Widget_t::Pushed_handler;

        Button();

        void on_pushed(Pushed_handler);

        void set_font(const Rasterized_font *);
        auto font() const { return _font.source(); }
        void set_label(const std::u32string &);
        void set_label(const std::string &);
        auto label() const { return _label; }

        void init() override;

        void render(Canvas_t *, const Point &) override;

        // TODO: visual feedback on mouse down / up

    protected:

        void mouse_click(const Point &pos, int button, Count count) override;

        Pushed_handler          _on_pushed;
        Font_resource           _font;
        std::u32string          _label;
        Point                   _label_origin;
        Rectangle               _label_rect;
    };

    // Layouter aspect ----------------------------------------------

    // Dummy implementation (no layouting)

    template <class Class, class Config, class Parent>
    struct Button__Layouter<Class, Config, false, Parent>: public Parent
    {
        void layout() {} // called from init
        void font_changed() {}
        void text_changed() {}
    };

    // Real implementation

    template <class Class, class Config, class Parent>
    struct Button__Layouter<Class, Config, true, Parent>: public Parent
    {
        // Button__Layouter() { _padding = this->button_padding(); }

        // Layouter contract

        void init_layout() override;
        auto get_minimal_size() -> Extents override;
        void layout() override;

        // Extra capabilities coming with layouting
        // TODO
        // void change_font(const Rasterized_font *);
        // void change_label(const std::u32string &);

        // Extra properties
        //auto minimal_padding() -> int; // THIS *INCLUDES* THE BORDER WIDTH (unlike the CSS box model!)

        // Interface with main class (Button)

        void font_changed();
        void text_changed();

    protected:
        class Button_t: public Class { friend struct Button__Layouter; };

        auto p() { return static_cast<Button_t*>(this); }
        void compute_bounding_box();

        Alignment               _minor_align = Alignment::cultural_minor_middle;
        Alignment               _major_align = Alignment::cultural_major_middle;
        Text_bounding_box       _bounding_box;
    };

} // ns cppgui
