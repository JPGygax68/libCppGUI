#pragma once

#include <iostream> // TODO: REMOVE!

#include "./layouting.hpp"

#include "./Widget.hpp"

namespace cppgui {

    template <class Config, bool With_layout>
    struct Button_layouter {
        template <class Aspect_parent> struct Aspect;
    };

    /** TODO: how to support changing label (and later on, icon) at runtime without mixing
            in the layouting aspect ?
     */
    template <class Config, bool With_layout>
    class Button: public Button_layouter<Config, With_layout>::template Aspect< Widget<Config, With_layout> >
    {
    public:
        using Renderer = typename Config::Renderer;
        using Font_handle = typename Renderer::font_handle;
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;

        void set_font(const Rasterized_font *);
        auto font() const { return _font; }
        void set_label(const std::u32string &);
        auto label() const { return _text; }

        void init() override;

        //void update_render_resources(Renderer *) override;
        void render(Canvas_t *, const Position &) override;

        // TODO: visual feedback on mouse down / up

        // TODO: TEMPORARY, REMOVE!!
        //void mouse_enter() override { std::cout << "Button::mouse_enter() " << std::endl; Widget_t::mouse_enter(); }
        //void mouse_exit () override { std::cout << "Button::mouse_exit() "  << std::endl; Widget_t::mouse_exit (); }

    protected:
        // Styling
        // TODO: make into aspect ?
        auto face_color() -> Color;
        auto border_color() -> Color;
        auto border_width() -> int;

        const Rasterized_font  *_font = nullptr;
        std::u32string          _label;
        Position                _label_origin;

        Font_handle             _fnthnd;
    };

    // Layouter aspect

    template <class Config>
    struct Button_layouter<Config, true> {
        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            // Layouter contract

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

            // Extra capabilities coming with layouting
            // TODO
            // void change_font(const Rasterized_font *);
            // void change_label(const std::u32string &);

            // Extra properties
            auto minimal_padding() -> int; // THIS *INCLUDES* THE BORDER WIDTH (unlike the CSS box model!)

            // Interface with main class (Button)

            void font_changed();
            void text_changed();

        protected:
            class Button_t: public Button<Config, true> { friend struct Aspect; };

            auto p() { return static_cast<Button_t*>(this); }
            void compute_bounding_box();

            Text_bounding_box   _bbox;
        };
    };

    // Nil implementation (must short out interface with main class)

    template <class Config>
    struct Button_layouter<Config, false> {
        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            void layout() {} // called from init
            void font_changed() {}
            void text_changed() {}
        };
    };

} // ns cppgui