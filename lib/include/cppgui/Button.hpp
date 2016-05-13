#pragma once

#include <iostream> // TODO: REMOVE!

//#include "./layouting.hpp"

#include "./Widget.hpp"
#include "./Box.hpp"

namespace cppgui {

    template <class Config, bool With_layout>
    struct Button__Layouter {
        template <class Aspect_parent> struct Aspect;
    };

    /** TODO: how to support changing label (and later on, icon) at runtime without mixing
            in the layouting aspect ?
     */
    template <class Config, bool With_layout>
    class Button: 
        public Button__Layouter<Config, With_layout>::template Aspect< 
            Bordered_box<Config, With_layout>::template Aspect< Widget<Config, With_layout> > >
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

        void mouse_click(const Point &pos, int button, int count) override;

        Pushed_handler          _on_pushed;
        Font_resource           _font;
        std::u32string          _label;
        Point                   _label_origin;
        Rectangle               _label_rect;
    };

    // Layouter aspect

    class Single_element_layout;

    template <class Config>
    struct Button__Layouter<Config, true> {
        template <class Aspect_parent> struct Aspect: Box__Layouter<Config, true>::template Aspect< Aspect_parent >  
        {
            Aspect() { _padding = this->button_padding(); }

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
            class Button_t: public Button<Config, true> { friend struct Aspect; };

            auto p() { return static_cast<Button_t*>(this); }
            void compute_bounding_box();

            Text_bounding_box       _bbox;
            Single_element_layout   _layout;
        };
    };

    // Nil implementation (must short out interface with main class)

    template <class Config>
    struct Button__Layouter<Config, false> {
        template <class Aspect_parent> struct Aspect: public Aspect_parent  {
            void layout() {} // called from init
            void font_changed() {}
            void text_changed() {}
        };
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_BUTTON(Config, With_layout) \
    template cppgui::Bordered_box    <Config, With_layout>; \
    template cppgui::Button          <Config, With_layout>; \
    template cppgui::Button__Layouter<Config, With_layout>;
