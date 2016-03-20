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

    template <class Config>
    struct Label_layouter<Config, true> {

        CPPGUI_DEFINE_ASPECT(Aspect)
        {
            auto minimal_size()->Extents override;
            void layout() override;
        };
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

        void set_font(const Rasterized_font *); //, Root_widget_t *, Renderer *);
        auto font() const { return _font; }
        void set_text(const std::u32string &);
        auto text() const { return _text; }

        //void update_render_resources(Renderer *) override;
        void render(Renderer *, const Position &) override;

        // Interface to layouter aspect

        auto& text_position() { return _txpos; }

    protected:
        const gpc::fonts::rasterized_font  *_font = nullptr;
        std::u32string                      _text;
        Position                            _txpos;
        Font_handle                         _fnthnd;
    };

} // ns cppgui