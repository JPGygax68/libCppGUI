#pragma once

#include "./Widget.hpp"

namespace cppgui {

    template <class Config, bool With_layout> class Textbox;

    template <class Config, bool With_layout>
    struct Textbox_layouter {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            void font_changed() { static_assert(false, "Concept: Textbox_layouter::font_changed(): must never be used"); }
            auto minimal_size() -> Extents { static_assert(false, "Concept: Textbox_layouter::minimal_size(): must never be used"); return {}; }
            void layout() override { static_assert(false, "Concept: Textbox_layouter::layout(): must never be used"); }
        };
    };

    template <class Config, bool With_layout>
    class Textbox_base: public Widget<Config, With_layout>
    {
    public:
        using Renderer = typename Config::Renderer;
        using Widget_t = typename Widget<Config, With_layout>;
        using Textbox_t = typename Textbox<Config, With_layout>;
        using Font_handle = typename Widget_t::Font_handle;

        void set_font(const Rasterized_font *);
        auto font() const { return _font; }
        void set_text(const std::u32string &);
        auto text() const { return _text; }

        void mouse_click(const Position &, int button, int count) override;

        void gained_focus() override;
        void loosing_focus() override;

        void render(Renderer *, const Position &pos) override;

        // Interface to layouter aspect

        auto& text_position() { return _txpos; }

    protected: // for access by Layouter aspect
        Position                _txpos;

    private:
        const Rasterized_font  *_font = nullptr;

        std::u32string          _text;
        Font_handle             _fnthnd;
    };

    template <class Config, bool With_layout>
    class Textbox : public Textbox_layouter<Config, With_layout>::Aspect< Textbox_base<Config, With_layout> > {};

    // Layouting aspect ---------------------------------------------

    template <class Config>
    struct Textbox_layouter<Config, false> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {};
    };

    template <class Config>
    struct Textbox_layouter<Config, true> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {
        
            using Textbox_t = Textbox_base<Config, true>;

            void font_changed();
            auto minimal_size() -> Extents override;
            void layout() override;

        private:
            int _ascent, _descent; // TODO: support vertical writing
            int _mean_char_width;
        };
    };

} // ns cppgui
