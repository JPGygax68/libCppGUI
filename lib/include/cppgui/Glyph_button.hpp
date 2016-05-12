#pragma once

#include <string>

#include "./Widget.hpp"
#include "./Box.hpp"

namespace cppgui {

    // Forward declaractions

    struct Icon_glyph;

    template <class Config, bool With_layout> struct Glyph_button__Layouter {
        template <class Parent_aspect> struct Aspect: Aspect_parent {};
    };

    // Glyph_button declaration 

    template<class Config, bool With_layout>
    class Glyph_button: 
        public Glyph_button__Layouter<Config, With_layout>::template Aspect< 
            Bordered_box<Config, With_layout>::template Aspect< 
                Widget<Config, With_layout> > >
    {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Font_handle = typename Widget_t::Font_handle;
        using Push_handler = typename Widget_t::Push_handler;
        using Font_resource = typename Widget_t::Font_resource;

        void on_push(Push_handler);

        void set_font(const Rasterized_font *font) { _label_font = font; }

        void set_label(const std::u32string &label) { _label = label; }

        void set_glyph(const Icon_glyph &);

        //void enable_border(bool enabled) { _border_enabled = enabled; }

        void init() override;

        void render(Canvas_t *, const Point &offset) override;

        void mouse_button(const Point &, int button, Key_state) override;

    protected:
        //Click_handler                   _on_click;
        Push_handler                    _on_push;
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

    template <class Config>
    struct Glyph_button__Layouter<Config, true> {

        template <class Aspect_parent>
        struct Aspect: public Box__Layouter<Config, true>::template Aspect< Aspect_parent >
        {
            Aspect() { _padding = this->button_padding(); }

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

            // TODO:
            // void change_glyph();

        private:
            class Glyph_button_t: public Glyph_button<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<Glyph_button_t*>(this); }

            void compute_sizes();

            Text_bounding_box           _glyph_bounds;
            Text_bounding_box           _label_bounds;
            Length                      _glyph_min_edge;
            Length                      _spacing; // spacing between label and glyph
        };
    };


} // ns cppui

#define CPPGUI_INSTANTIATE_GLYPH_BUTTON(Config, With_layout) \
    template cppgui::Bordered_box          <Config, With_layout>; \
    template cppgui::Glyph_button          <Config, With_layout>; \
    template cppgui::Glyph_button__Layouter<Config, With_layout>;
