#pragma once

#include "./Widget.hpp"
#include "./Box.hpp"

namespace cppgui {

    // Forward declarations 

    template <class Config, bool With_layout, class Parent>
    struct Textbox__Layouter: public Parent
    {
        void font_changed() { static_assert(false, "Concept: Textbox__Layouter::font_changed(): must never be used"); }
        auto get_minimal_size() -> Extents { static_assert(false, "Concept: Textbox__Layouter::get_minimal_size(): must never be used"); return {}; }
        void layout() override { static_assert(false, "Concept: Textbox__Layouter::layout(): must never be used"); }
    };

    // Main class 

    // TODO: do not stretch vertically to fill all available space, instead display a strip with border and padding to fit the font size

    template <class Config, bool With_layout>
    class Textbox: 
        public Textbox__Layouter<Config, With_layout, 
            Bordered_box<Config, With_layout, 
                Widget<Config, With_layout> > >
    {
    public:
        using Renderer      = typename Config::Renderer;
        using Keycode       = typename Config::Keyboard::Keycode;
        using Widget_t      = typename Widget<Config, With_layout>;
        using Textbox_t     = typename Textbox<Config, With_layout>;
        using Canvas_t      = typename Widget_t::Canvas_t;
        using Font_resource = typename Widget_t::Font_resource;

        // TODO: implement "set" and "change" variants
        void set_font(const Rasterized_font *);
        auto font() const { return _font; }
        void set_text(const std::u32string &);
        void set_text(const std::string &);
        void change_text(const std::u32string &);
        void change_text(const std::string &);
        auto text() const { return _text; }

        void init() override;
        void compute_view_from_data() override;

        void mouse_motion(const Point &) override;
        void mouse_button(const Point &, int button, Key_state) override;
        void mouse_click(const Point &, int button, int count) override;
        void text_input(const char32_t *text, size_t count) override;

        void mouse_enter() override;
        void mouse_exit() override;

        void gained_focus() override;
        void loosing_focus() override;

        void render(Canvas_t *, const Point &pos) override;

        bool handle_key_down(const Keycode &) override;

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
        void move_caret_to_pointer_position(const Point &pos);
        auto find_character_at_pointer_position(const Point &pos) -> std::pair<size_t, int>;
        void bring_caret_into_view();

        // Styling
        // TODO: make into aspect ?
        auto selected_text_background_color() -> Color;
        auto caret_color() -> Color;

        Font_resource           _font;

        int                     _ascent, _descent; // TODO: support vertical writing
        int                     _mean_char_width;
        Rectangle               _inner_rect;
        Point                   _txpos;
        //int                     _txmaxlen;

        std::u32string          _text;
        unsigned int            _caret_char_idx , _sel_start_char_idx , _sel_end_char_idx ;
        int                     _caret_pixel_pos, _sel_start_pixel_pos, _sel_end_pixel_pos;
        unsigned int            _first_vis_char_idx = 0;
        int                     _scroll_offs = 0;
    };

    // Layouting aspect ---------------------------------------------

    template <class Config, class Parent>
    struct Textbox__Layouter<Config, false, Parent>: public Parent
    {
    };

    template <class Config, class Parent>
    struct Textbox__Layouter<Config, true, Parent>: 
        public Box__Layouter<Config, true, Parent>
    {
        class Textbox_t: public Textbox<Config, true> { friend struct Textbox__Layouter; };

        auto p() { return static_cast<Textbox_t*>(static_cast<Textbox<Config, true>*>(this)); }

        Textbox__Layouter();

        void change_font(const Rasterized_font *);

        void compute_text_extents();

        void init_layout() override;
        auto get_minimal_size() -> Extents override;
        void layout() override;

        // "Stylesheet"
        static constexpr auto default_padding() -> Padding { return { 3, 3, 3, 3 }; }
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_TEXTBOX(Config, With_layout) \
    template cppgui::Textbox<Config, With_layout>; \
    template cppgui::Textbox__Layouter<Config, With_layout, cppgui::Textbox<Config, With_layout>>;