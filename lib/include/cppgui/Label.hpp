#pragma once

#include <string>

#include "./Widget.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    template <class Config, bool With_layout> struct Label__Layouter {
        CPPGUI_DEFINE_ASPECT(Aspect) {};
    };

    template <class Config, bool With_layout> class Root_widget;

    /** Label, without layouting.
     */
    template <class Config, bool With_layout>
    class Label: public Label__Layouter<Config, With_layout>::Aspect< Widget<Config, With_layout> >
    {
    public:
        using Renderer = typename Config::Renderer;
        using Font_handle = typename Renderer::font_handle;
        using Root_widget_t = Root_widget<Config, With_layout>;
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;

        void set_font(const Rasterized_font *);
        auto font() const { return _font; }
        void set_text(const std::u32string &);
        auto text() const { return _text; }

        void init() override;

        void render(Canvas_t *, const Position &offset) override;

    protected:
        const Rasterized_font  *_font = nullptr;
        std::u32string          _text;
        Position                _text_orig; // origin of first character of label
        Rectangle               _text_rect;

        Font_handle             _fnthnd;
    };

    template <class Config>
    struct Label__Layouter<Config, true> {

        template<class Aspect_parent> struct Aspect: Aspect_parent {

            // Layouter aspect contract

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

            // Own methods

            void set_horizontal_alignment(Alignment align) { _horz_align = align; }
            void set_vertical_alignment(Alignment align) { _vert_align = align; }

        private:
            class Label_t: public Label<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<Label_t*>(this); }

            Alignment               _horz_align = center;
            Alignment               _vert_align = middle;
        };
    };

} // ns cppgui