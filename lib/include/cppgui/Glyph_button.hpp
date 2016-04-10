#pragma once

#include <string>

#include "./Widget.hpp"

namespace cppgui {

    // Forward declaractions

    template <class Config, bool With_layout> struct Glyph_button__Layouter {
        template <class Parent_aspect> struct Aspect;
    };

    // Glyph_button declaration 

    template<class Config, bool With_layout>
    class Glyph_button: public Glyph_button__Layouter<Config, With_layout>::Aspect< Widget<Config, With_layout> > {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Font_handle = typename Widget_t::Font_handle;

        void set_font(const Rasterized_font *font) { _label_fnt = font; }

        void set_label(const std::u32string &label) { _label = label; }

        void set_glyph(const Icon_glyph &);

        void enable_border(bool enabled) { _border_enabled = enabled; }

        void init() override;

        void render(Canvas_t *, const Position &offset) override;

    protected:
        const Rasterized_font          *_label_fnt = nullptr;
        std::u32string                  _label;
        const Rasterized_font          *_glyph_fnt;
        char32_t                        _glyph_cp;
        bool                            _border_enabled = true;

        Position                        _label_pos;
        Position                        _glyph_pos;

    private:
        Font_handle                     _glyph_font_hnd;
        Font_handle                     _label_font_hnd;
    };

    // Layouter aspect

    template <class Config>
    struct Glyph_button__Layouter<Config, true> {

        template <class Aspect_parent> struct Aspect: public Aspect_parent
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
