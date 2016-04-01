#pragma once

#include "./Widget.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    template <class Config, bool With_layout> struct Checkbox_Layouter {
        template <class Parent_aspect> struct Aspect;
    };

    template <class Config, bool With_layout>
    class Checkbox: public Checkbox_Layouter<Config, With_layout>::Aspect< Widget<Config, With_layout> >
    {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Font_handle = typename Widget_t::Font_handle;

        void set_font(const Rasterized_font *font) { _label_font = font; }
        auto font() const { return _label_font; }
        void set_glyph_font(const Rasterized_font *font) { _glyph_font = font; }
        auto glyph_font() { return _glyph_font; }
        void set_label(const std::u32string &label) { _label = label; }
        auto label() const { return _label; }

        void init() override;

        void render(Canvas_t *, const Position &offset) override;

    protected: // for access by Layouter aspect

        const Rasterized_font  *_label_font;
        const Rasterized_font  *_glyph_font;
        std::u32string          _label;
        Position                _label_pos;

    private:
        Font_handle             _fnthnd;
    };

    template <class Config>
    struct Checkbox_Layouter<Config, true> {

        template <class Aspect_parent> struct Aspect: public Aspect_parent
        {
            void init_layout() override;
            auto minimal_size() -> Extents override;
            void layout() override;

        private:
            class Checkbox_t: public Checkbox<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<Checkbox_t*>(this); }

            void compute_sizes();

            Text_bounding_box   _label_bounds;
        };
    };

} // ns cppgui