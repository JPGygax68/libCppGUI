#pragma once

#include "./Widget.hpp"

namespace cppgui {

    template <class Config, bool With_layout>
    struct Textbox_layouter {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            void font_changed() { static_assert(false, "Concept: Textbox_layouter::font_changed(): must never be used"); }
            auto get_minimal_size() -> Extents { static_assert(false, "Concept: Textbox_layouter::get_minimal_size(): must never be used"); return {}; }
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
        using Canvas_t = typename Widget_t::Canvas_t;
        using Font_handle = typename Widget_t::Font_handle;

        // TODO: implement "set" and "change" variants
        void set_font(const Rasterized_font *);
        auto font() const { return _font; }
        void set_text(const std::u32string &);
        void change_text(const std::u32string &);
        auto text() const { return _text; }

        void init() override;

        void mouse_motion(const Position &) override;
        void mouse_button(const Position &, int button, Key_state) override;
        void mouse_click(const Position &, int button, int count) override;
        void text_input(const char32_t *text, size_t count) override;
        void key_down(const Keycode &) override;

        void mouse_enter() override;
        void mouse_exit() override;

        void gained_focus() override;
        void loosing_focus() override;

        void render(Canvas_t *, const Position &pos) override;

    public: // for access by Layouter aspect
        int                     _ascent, _descent; // TODO: support vertical writing
        int                     _mean_char_width;
        Rectangle               _inner_rect;
        Position                _txpos;
        //int                     _txmaxlen;

    protected:
        // Actions
        // TODO: may need to be made public again
        void move_cursor_left    (bool extend_sel);
        void move_cursor_right   (bool extend_sel);
        void move_cursor_to_start(bool extend_sel);
        void move_cursor_to_end  (bool extend_sel);
        void insert_characters(const char32_t *, size_t);
        void delete_before_caret();
        void delete_after_caret();
        void select_all();

        // Internal methods
        // TODO: most or all of this actually belongs to a yet-to-be-created "Textfield" class
        //      that can then 
        void internal_select_all();
        void recalc_selection_strip();
        void collapse_selection_to_caret();
        bool have_selection() const { return _sel_start_char_idx < _sel_end_char_idx; }
        void delete_selected();
        void move_caret_to_pointer_position(const Position &pos);
        auto find_character_at_pointer_position(const Position &pos)->std::pair<size_t, int>;
        void bring_caret_into_view();

        // Styling
        // TODO: make into aspect ?
        auto selected_text_background_color() -> Color;
        auto caret_color() -> Color;

        const Rasterized_font  *_font = nullptr; // TODO: avoid setting default value

        std::u32string          _text;
        Font_handle             _fnthnd;
        unsigned int            _caret_char_idx , _sel_start_char_idx , _sel_end_char_idx ;
        int                     _caret_pixel_pos, _sel_start_pixel_pos, _sel_end_pixel_pos;
        unsigned int            _first_vis_char_idx = 0;
        int                     _scroll_offs = 0;
    };

    // Layouting aspect ---------------------------------------------

    template <class Config>
    struct Textbox_layouter<Config, false> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {};
    };

    template <class Config>
    struct Textbox_layouter<Config, true> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {
        
            class Textbox_t: public Textbox<Config, true> { friend struct Aspect; };

            auto p() { return static_cast<Textbox_t*>(this); }

            void change_font(const Rasterized_font *);

            void compute_label_size();

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;
        };
    };

} // ns cppgui
