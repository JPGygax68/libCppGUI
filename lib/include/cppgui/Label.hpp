#pragma once

#include <string>

#include "./Widget.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    template <class Config, bool With_layout> struct Label_layouter {
        CPPGUI_DEFINE_ASPECT(Aspect) {};
    };

    template <class Config, bool With_layout> class Root_widget;

    /** Label, without layouting.
     */
    template <class Config, bool With_layout>
    class Label: public Label_layouter<Config, With_layout>::Aspect< Widget<Config, With_layout> >
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
        void set_background_color(const Color &); // TODO: move to Widget<> ?

        void init() override;

        void render(Canvas_t *, const Position &offset) override;

    protected:
        const Rasterized_font  *_font = nullptr;
        Color                   _bkgnd_clr = {0, 0, 0, 0};
        std::u32string          _text;
        Position                _txpos;
        Font_handle             _fnthnd;
    };

    template <class Config>
    struct Label_layouter<Config, true> {

        CPPGUI_DEFINE_ASPECT(Aspect)
        {
            void init_layout() override;
            auto minimal_size() -> Extents override;
            void layout() override;

            class Label_t: public Label<Config, true> { friend struct Aspect; };

        private:
            auto p() { return static_cast<Label_t*>(this); }
        };
    };

} // ns cppgui