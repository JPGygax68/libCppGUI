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
    class Textbox: public Textbox_layouter<Config, With_layout>::Aspect< Widget<Config, With_layout> >
    {
    public:
        using Renderer = typename Config::Renderer;
        using Keycode  = typename Config::Keyboard::Keycode;
        using Widget_t = typename Widget<Config, With_layout>;
        using Textbox_t = typename Textbox<Config, With_layout>;
        using Font_handle = typename Widget_t::Font_handle;

        void set_font(const Rasterized_font *);
        auto font() const { return _font; }
        void set_text(const std::u32string &);
        auto text() const { return _text; }

        void mouse_click(const Position &, int button, int count) override;
        void text_input(const char32_t *text, size_t count) override;
        void key_down(const Keycode &) override;

        void mouse_enter() override;
        void mouse_exit() override;

        void gained_focus() override;
        void loosing_focus() override;

        void render(Renderer *, const Position &pos) override;

    public: // for access by Layouter aspect
        int                     _ascent, _descent; // TODO: support vertical writing
        int                     _mean_char_width;
        Position                _txpos;

    private:
        void font_changed();

        // Actions
        // TODO: may need to be made public again
        void move_cursor_left();
        void move_cursor_right();
        void insert_characters(const char32_t *, size_t);
        void delete_before_caret();
        void delete_after_caret();
        void move_caret_to_pointer_position(const Position &pos);

        const Rasterized_font  *_font = nullptr; // TODO: avoid setting default value

        std::u32string          _text;
        Font_handle             _fnthnd;
        unsigned int            _caret_pos = 0; // TODO: avoid setting default value
        int                     _caret_offs = 0; // TODO: extend to selection; avoid default value
    };

    // Layouting aspect ---------------------------------------------

    template <class Config>
    struct Textbox_layouter<Config, false> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {};
    };

    template <class Config>
    struct Textbox_layouter<Config, true> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {
        
            using Textbox_t = Textbox<Config, true>;

            auto p() { return static_cast<Textbox_t*>(this); }

            auto minimal_size() -> Extents override;
            void layout() override;
        };
    };

} // ns cppgui
