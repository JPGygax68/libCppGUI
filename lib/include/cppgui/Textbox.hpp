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

        // TODO: implement "set" and "change" variants
        void set_font(const Rasterized_font *);
        auto font() const { return _font; }
        void set_text(const std::u32string &);
        auto text() const { return _text; }

        void init() override;

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
        void move_cursor_left (bool extend_sel);
        void move_cursor_right(bool extend_sel);
        void insert_characters(const char32_t *, size_t);
        void delete_before_caret();
        void delete_after_caret();
        void move_caret_to_pointer_position(const Position &pos);

        // Internal methods
        void internal_select_all();
        void recalc_selection_strip();
        void collapse_selection_to_caret();

        const Rasterized_font  *_font = nullptr; // TODO: avoid setting default value

        std::u32string          _text;
        Font_handle             _fnthnd;
        unsigned int            _caret_pos , _sel_start_pos , _sel_end_pos ;
        int                     _caret_offs, _sel_start_offs, _sel_end_offs;
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
