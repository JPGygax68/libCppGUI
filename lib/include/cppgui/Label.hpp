#pragma once

#include <string>

#include "./Widget.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    template <class Config>
    struct Label_layouter {
        
        CPPGUI_DEFINE_ASPECT(Aspect)
        {
            auto minimal_size() -> Extents override;
            void layout() override;
        };
    };

    /** Label, without layouting.
     */
    template <class Config, bool With_layout>
    class Label: 
        public select_aspect<With_layout, Label_layouter<Config>::Aspect, Nil_layouter>::template aspect<Widget<Config, With_layout>>
    {
    public:
        using Renderer = typename Config::Renderer;
        using Font_handle = typename Renderer::font_handle;

        void set_font(const gpc::fonts::rasterized_font *);
        auto font() const { return _font; }
        void set_text(const std::u32string &);
        auto text() const { return _text; }

        void update_render_resources(Renderer *) override;
        void render(Renderer *, const Position &) override;

        // Interface to layouter aspect

        auto& text_position() { return _txpos; }

    protected:
        const gpc::fonts::rasterized_font  *_font;
        std::u32string                      _text;
        Position                            _txpos;
        Font_handle                         _fnthnd;
    };

} // ns cppgui