#pragma once

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016 Hans-Peter Gygax

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "./Widget.hpp"
#include "./Box.hpp"


namespace cppgui {

    // Main class 

    // TODO: do not stretch vertically to fill all available space, instead display a strip with border and padding to fit the font size

    class Textbox: public Widget, public Box<Textfield_box_styles>
    {
    public:

        using Done_handler  = std::function<void(const std::u32string&)>;

        static constexpr auto default_padding(int /*dir*/) { return 2; }

        Textbox();

        // TODO: on_blur() ? on_text_changed() ?
        void on_done(Done_handler);

        // TODO: implement "set" and "change" variants
        void set_font(const Rasterized_font *);
        auto font() const { return _font; }
        void set_size(Count chars);
        auto size() const { return _size; }
        void set_text(const std::u32string &);
        void set_text(const std::string &);
        void change_text(const std::u32string &);
        void change_text(const std::string &);
        auto text() const { return _text; }

        void init(Canvas *) override;
        void compute_view_from_data() override;

        bool mouse_motion(const Point &) override;
        bool mouse_button(const Point &, int button, Key_state, Count clicks) override;
        bool mouse_click(const Point &, int button, Count count) override;
        bool key_down(const Keycode &) override;
        bool text_input(const char32_t *text, size_t count) override;

        void mouse_enter() override;
        void mouse_exit() override;

        void gained_focus() override;
        void loosing_focus() override;

        void render(Canvas *, const Point &pos) override;

    protected:
        using Super = Widget;

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
        void done();

        // Internal methods
        void internal_select_all();
        void recalc_selection_strip();
        void collapse_selection_to_caret();
        bool have_selection() const { return _sel_start_char_idx < _sel_end_char_idx; }
        void delete_selected();
        void move_caret_to_pointer_position(const Point &pos);
        auto find_character_at_pointer_position(const Point &pos) -> std::pair<size_t, int>;
        void bring_caret_into_view();
        void notify_done();

        // Styling
        // TODO: make into aspect ?
        auto selected_text_background_color() -> RGBA;
        auto caret_color() -> RGBA;

        Done_handler            _on_done;

        int                     _size;
        Font_resource           _font;

        Bbox                    _inner_bbox;
        int                     _ascent, _descent; // TODO: support vertical writing
        int                     _mean_char_width;
        //Point                   _txpos;
        //int                     _txmaxlen;

        std::u32string          _text;
        unsigned int            _caret_char_idx , _sel_start_char_idx , _sel_end_char_idx ;
        int                     _caret_pixel_pos, _sel_start_pixel_pos, _sel_end_pixel_pos;
        unsigned int            _first_vis_char_idx = 0;
        int                     _scroll_offs = 0;

        #ifndef CPPGUI_EXCLUDE_LAYOUTING

    public:
        void change_font(const Rasterized_font *);

        void compute_text_extents();

        void init_layout() override;
        auto get_minimal_bbox() -> Bbox override;
        void compute_layout(Bbox_cref) override;

        // "Stylesheet"
        //static constexpr auto default_padding() -> Padding { return { 3, 3, 3, 3 }; }

        #endif // CPPGUI_EXCLUDE_LAYOUTING
    };

} // ns cppgui
