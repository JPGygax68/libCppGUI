#pragma once

#include <string>

#include "./Widget.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    /** Label, without layouting.
     */
    template <class Config, bool WithLayout>
    class Label_basic: public Widget<Config, WithLayout> {
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
    class Label_full: public Label_basic<Config, true>, public Widget_layouter {
    public:
        auto minimal_size() -> Extents override;
        void layout() override;
    };

    template <class Config, bool WithLayout = true>
    class Label: public std::conditional<WithLayout, Label_full<Config>, Label_basic<Config, false>>::type {};

} // ns cppgui