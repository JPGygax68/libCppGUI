#pragma once

#include "./Widget.hpp"

namespace cppgui {

    template <class Config, bool With_layout>
    struct Textbox_layouter {

        CPPGUI_DEFINE_ASPECT(Aspect) {
            void font_changed() { static_assert(false, "Concept: Textbox_layouter::font_changed(): must never be used"); }
            auto minimal_size() -> Extents { static_assert(false, "Concept: Textbox_layouter::minimal_size(): must never be used"); return {}; }
            void layout() override { static_assert(false, "Concept: Textbox_layouter::layout(): must never be used"); }
        };
    };

    template <class Config, bool With_layout>
    class Textbox: public Textbox_layouter<Config, With_layout>::Aspect< Widget<Config, With_layout> >
    {
    public:
        using Renderer = typename Config::Renderer;

        void set_font(const Rasterized_font *);
        auto font() const { return _font; }
        void set_text(const std::u32string &);
        auto text() const { return _text; }

        void render(Renderer *, const Position &pos) override;

        // Interface to layouter aspect

        auto& text_position() { return _txpos; }

    private:
        Rasterized_font    *_font = nullptr;
    };

    // Layouting aspect ---------------------------------------------

    // Without layouting, the aspect "specializes" to nil

    template <class Config>
    struct Textbox_layouter<Config, false> : public Nil_aspect_wrapper {};

    // Real layouter

    template <class Config>
    struct Textbox_layouter<Config, true> {

        CPPGUI_DEFINE_ASPECT(Aspect)
        {
            void font_changed();
            auto minimal_size() -> Extents override;
            void layout() override;
        };
    };

} // ns cppgui
