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

        void render(Canvas_t *, const Point &offset) override;

        void change_text(const std::u32string &);

    protected:
        const Rasterized_font  *_font = nullptr;
        std::u32string          _text;
        Point                _text_origin; // origin of first character of label
        Rectangle               _text_rect;

        Font_handle             _fnthnd;
    };

    class Single_element_layout;

    template <class Config>
    struct Label__Layouter<Config, true> {

        template<class Aspect_parent> struct Aspect: Aspect_parent {

            // Layouter aspect contract

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

            // Own methods

            void set_minor_alignment(Alignment align) { _minor_alignment = align; }
            void set_major_alignment(Alignment align) { _major_alignment = align; }
            // TODO: "change" versions of the above that update layout

        private:
            class Label_t: public Label<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<Label_t*>(this); }

            Alignment               _minor_alignment = Alignment::cultural_minor_middle;
            Alignment               _major_alignment = Alignment::cultural_major_middle;
            Single_element_layout   _layout;
        };
    };

} // ns cppgui