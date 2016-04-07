#pragma once

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

        void set_glyph(const Icon_glyph &);

        void enable_border(bool enabled) { _border_enabled = enabled; }

        void init() override;

        void render(Canvas_t *, const Position &offset) override;


    protected:
        const Rasterized_font          *_glyph_fnt;
        char32_t                        _glyph_cp;
        Position                        _glyph_pos;
        bool                            _border_enabled = true;

    private:
        Font_handle                     _font_hnd;
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
            Length                      _min_edge;
        };
    };


} // ns cppui
