#pragma once

#include <string>

#include "./Widget.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    template <class Config>
    class Label_min: public Widget<Config> {
    public:
        void set_font(const gpc::fonts::rasterized_font *);
        void set_text(const std::u32string &);

        void update_render_resources(Renderer *) override;
        void render(Renderer *, const Position &) override;

    protected:
        const gpc::fonts::rasterized_font  *_font;
        std::u32string                      _text;
        Position                            _txpos;
        Font_handle                         _fnthnd;
    };

    template <class Config>
    class Label_full: public Label_min<Config>, public Widget_layouter {
    public:
        auto minimal_size() -> Extents override;
        void layout() override;
    };

    template <class Config, bool WithLayout = true>
    class Label: public std::conditional<WithLayout, Label_full<Config>, Label_min<Config>>::type {};

} // ns cppgui