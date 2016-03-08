#pragma once

#include <string>

#include "./Widget.hpp"

namespace gpc { namespace fonts {

    struct rasterized_font;
} }

namespace cppgui {

    template <class Renderer>
    class Label_min: public Widget<Renderer> {
    public:
        void set_font(const gpc::fonts::rasterized_font *);
        void set_text(const std::u32string &);

        void update_resources(Renderer *) override;
        void render(Renderer *, const Position &) override;

    protected:
        const gpc::fonts::rasterized_font  *_font;
        std::u32string                      _text;
        Position                            _txpos;
        Font_handle                         _fnthnd;
    };

    template <class Renderer>
    class Label_full: public Label_min<Renderer>, public Widget_layouter {
    public:
        auto minimal_size() -> Extents override;
        void layout() override;
    };

    template <class Renderer, bool WithLayout = true>
    class Label: public std::conditional<WithLayout, Label_full<Renderer>, Label_min<Renderer>>::type {};

} // ns cppgui