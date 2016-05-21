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

#include "./unicode.hpp"
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
        void set_font(const Rasterized_font *font)
        {
            /* TODO: move this change_font() in layouter aspect
            if (font != _label_font)
            {
            _label_font = font;
            font_changed();
            }
            */
            _font.assign(font);
        }
        auto font() const { return _font; }
        void set_text(const std::u32string &text)
        {
            _text = text;
        }
        void set_text(const std::string &text)
        {
            _text = utf8_to_utf32(text);
        }
        void change_text(const std::u32string &text)
        {
            set_text(text);

            _first_vis_char_idx = 0;
            _scroll_offs = 0;

            if (font().source()) // TODO: is there a way to avoid this check ? (i.e. use different set_text() before font is set ?)
            {
                internal_select_all();
                _caret_char_idx = _sel_start_char_idx;
                _caret_pixel_pos = _sel_start_pixel_pos;
                bring_caret_into_view();
                invalidate();
            }
        }
        void change_text(const std::string &text)
        {
            change_text( utf8_to_utf32(text) );
        }
        auto text() const { return _text; }

        void init() override
        {
            _font.translate( root_widget()->canvas() );
        }
        void compute_view_from_data() override
        {
            _caret_char_idx = 0;
            _caret_pixel_pos = 0;
            _first_vis_char_idx = 0;
            internal_select_all();
        }

        void mouse_motion(const Point &pos) override
        {
            if (Config::Mouse::is_button_down(1))
            {
                auto char_pos = find_character_at_pointer_position(pos);
                if (char_pos.first != _caret_char_idx)
                {
                    auto &new_pos = char_pos.first;
                    auto &new_offs = char_pos.second;
                    if (new_pos < _caret_char_idx)
                    {
                        _sel_start_char_idx = new_pos;
                        _sel_start_pixel_pos = new_offs;
                        _sel_end_char_idx = _caret_char_idx;
                        _sel_end_pixel_pos = _caret_pixel_pos;
                    }
                    else {
                        _sel_end_char_idx = new_pos;
                        _sel_end_pixel_pos = new_offs;
                        _sel_start_char_idx = _caret_char_idx;
                        _sel_start_pixel_pos = _caret_pixel_pos;
                    }
                    invalidate();
                }
            }
            else {
                Widget_t::mouse_motion(pos);
            }
        }
        void mouse_button(const Point &pos, int button, Key_state state) override
        {
            if (state == pressed)
            {
                if (!has_focus())
                {
                    take_focus();
                }

                move_caret_to_pointer_position(pos);
                collapse_selection_to_caret();
            }
            else {
                Widget_t::mouse_button(pos, button, state);
            }
        }
        void mouse_click(const Point &/*pos*/, int /*button*/, int count) override
        {
            if (count == 2) // double-click
            {
                select_all();
            }
        }
        void text_input(const char32_t *text, size_t count) override
        {
            delete_selected(); // TODO: make optional
            insert_characters(text, count);
            invalidate();
        }

        void mouse_enter() override
        {
            root_widget()->push_cursor( Config::Mouse::get_ibeam_cursor() );
        }
        void mouse_exit() override
        {
            root_widget()->pop_cursor();
        }

        void gained_focus() override
        {
            // TODO: more...
            invalidate();
        }
        void loosing_focus() override
        {
            // TODO: more
            invalidate();
        }

        void render(Canvas_t *canvas, const Point &offs) override
        {
            fill(canvas, offs, rgba_to_native({ 1, 1, 1, 1 })); // TODO: (VERY MUCH) PROVISIONAL, GET REAL COLOR!

            auto pos = offs + position();

            canvas->set_clipping_rect(pos.x + _inner_rect.pos.x, pos.y + _inner_rect.pos.y, _inner_rect.ext.w, _inner_rect.ext.h);

            // Selection background
            auto bg_clr = selected_text_background_color();

            canvas->fill_rect(pos.x + _txpos.x + _scroll_offs + _sel_start_pixel_pos, pos.y + _txpos.y - _ascent, 
                _sel_end_pixel_pos - _sel_start_pixel_pos, _ascent - _descent, rgba_to_native(bg_clr));

            // Text
            if (!_text.empty() && _first_vis_char_idx < _text.size())
            {
                canvas->render_text(_font.get(), pos.x + _txpos.x, pos.y + _txpos.y, 
                    _text.data() + _first_vis_char_idx, _text.size() - _first_vis_char_idx, _inner_rect.ext.w);
            }

            // Caret
            if (has_focus())
            {
                canvas->fill_rect(pos.x + _txpos.x + _scroll_offs + _caret_pixel_pos, pos.y + _txpos.y - _ascent, 
                    1, _ascent - _descent, rgba_to_native(caret_color())); // TODO: width from stylesheet
            }

            canvas->cancel_clipping();
        }

        bool handle_key_down(const Keycode &key) override
        {
            if      (Keyboard::is_left     (key)) { move_cursor_left    (Config::Keyboard::is_shift_down()); return true; }
            else if (Keyboard::is_right    (key)) { move_cursor_right   (Config::Keyboard::is_shift_down()); return true; }
            else if (Keyboard::is_home     (key)) { move_cursor_to_start(Config::Keyboard::is_shift_down()); return true; }
            else if (Keyboard::is_end      (key)) { move_cursor_to_end  (Config::Keyboard::is_shift_down()); return true; }
            else if (Keyboard::is_backspace(key)) { delete_before_caret ();                                ; return true; }
            else if (Keyboard::is_delete   (key)) { delete_after_caret  ();                                ; return true; }
            else return false;
        }

    protected:
        // Actions
        // TODO: may need to be made public again
        void move_cursor_left    (bool extend_sel)
        {
            if (_caret_char_idx > 0)
            {
                _caret_char_idx--;
                auto glyph = font().source()->lookup_glyph(0, _text[_caret_char_idx]); // TODO: support font variants ?
                _caret_pixel_pos -= glyph->cbox.adv_x; // TODO: support vertical advance
                if (extend_sel)
                {
                    if (_caret_char_idx == _sel_start_char_idx - 1)
                    {
                        _sel_start_char_idx = _caret_char_idx;
                        _sel_start_pixel_pos = _caret_pixel_pos;
                    }
                    else if (_caret_char_idx == _sel_end_char_idx - 1)
                    {
                        _sel_end_char_idx = _caret_char_idx;
                        _sel_end_pixel_pos = _caret_pixel_pos;
                    }
                }
                else {
                    collapse_selection_to_caret();
                }
                bring_caret_into_view();
                invalidate();
            }
            else {
                // TODO: produce warning "bump" sound or effect
            }
        }
        void move_cursor_right   (bool extend_sel)
        {
            if (_caret_char_idx < _text.size())
            {
                auto glyph = font().source()->lookup_glyph(0, _text[_caret_char_idx]); // TODO: support font variants ?
                _caret_char_idx ++;
                _caret_pixel_pos += glyph->cbox.adv_x; // TODO: support vertical advance
                if (extend_sel)
                {
                    if (_caret_char_idx == _sel_end_char_idx + 1)
                    {
                        _sel_end_char_idx = _caret_char_idx;
                        _sel_end_pixel_pos = _caret_pixel_pos;
                    }
                    else if (_caret_char_idx == _sel_start_char_idx + 1)
                    {
                        _sel_start_char_idx = _caret_char_idx;
                        _sel_start_pixel_pos = _caret_pixel_pos;
                    }
                }
                else {
                    collapse_selection_to_caret();
                }
                bring_caret_into_view();
                invalidate();
            }
            else {
                // TODO: produce warning "bump" sound or effect
            }
        }
        void move_cursor_to_start(bool extend_sel)
        {
            if (_caret_char_idx > 0)
            {
                while (_caret_char_idx > 0)
                {
                    _caret_char_idx--;
                    auto glyph = font().source()->lookup_glyph(0, _text[_caret_char_idx]); // TODO: support font variants ?
                    _caret_pixel_pos -= glyph->cbox.adv_x; // TODO: support vertical advance
                }
                if (extend_sel)
                {
                    _sel_start_char_idx = _caret_char_idx;
                    _sel_start_pixel_pos = _caret_pixel_pos;
                }
                else {
                    collapse_selection_to_caret();
                }
                bring_caret_into_view();
                invalidate();
            }
            else {
                // TODO: produce warning "bump" sound or effect
            }
        }
        void move_cursor_to_end  (bool extend_sel)
        {
            if (_caret_char_idx < _text.size())
            {
                while (_caret_char_idx < _text.size())
                {
                    auto glyph = font().source()->lookup_glyph(0, _text[_caret_char_idx]); // TODO: support font variants ?
                    _caret_char_idx++;
                    _caret_pixel_pos += glyph->cbox.adv_x; // TODO: support vertical advanc
                }
                if (extend_sel)
                {
                    _sel_end_char_idx = _caret_char_idx;
                    _sel_end_pixel_pos = _caret_pixel_pos;
                }
                else {
                    collapse_selection_to_caret();
                }
                bring_caret_into_view();
                invalidate();
            }
            else {
                // TODO: produce warning "bump" sound or effect
            }
        }
        void insert_characters(const char32_t *text, size_t count)
        {
            _text = _text.substr(0, _caret_char_idx) + std::u32string{ text, count } + _text.substr(_caret_char_idx); // TODO: support selection

            for (size_t i = 0; i < count; i++)
            {
                auto glyph = _font.source()->lookup_glyph(0, text[i]); // TODO: support font variants ?
                _caret_pixel_pos += glyph->cbox.adv_x;
            }

            _caret_char_idx += count;
            collapse_selection_to_caret();
            bring_caret_into_view();
        }
        void delete_before_caret()
        {
            if (have_selection())
            {
                delete_selected();
                invalidate();
            }
            else if (_caret_char_idx > 0)
            {
                auto glyph = _font.source()->lookup_glyph(0, _text[_caret_char_idx - 1]); // TODO: support font variants ?
                _caret_pixel_pos -= glyph->cbox.adv_x;
                _caret_char_idx --;
                auto new_text = _text.substr(0, _caret_char_idx) + _text.substr(_caret_char_idx + 1);
                collapse_selection_to_caret();
                bring_caret_into_view();
                _text = new_text;
                // TODO: automatically scroll forward if there are characters scrolled away to the left ?
                invalidate();
            }
            else {
                // TODO: "bump"
            }
        }
        void delete_after_caret()
        {
            if (have_selection())
            {
                delete_selected();
                invalidate();
            }
            else if (_caret_char_idx < _text.size())
            {
                _text = _text.substr(0, _caret_char_idx) + _text.substr(_caret_char_idx + 1);
                collapse_selection_to_caret();
                invalidate();
            }
            else {
                // TODO: "bump"
            }
        }
        void select_all()
        {
            internal_select_all();

            _caret_char_idx = _sel_end_char_idx;
            _caret_pixel_pos = _sel_end_pixel_pos;
            bring_caret_into_view();

            invalidate();
        }

        // Internal methods
        // TODO: most or all of this actually belongs to a yet-to-be-created "Textfield" class
        //      that can then 
        void internal_select_all()
        {
            _sel_start_char_idx = 0;
            _sel_end_char_idx   = _text.size();

            recalc_selection_strip();
        }
        void recalc_selection_strip()
        {
            // TODO: support vertical scripts

            Point pos = { 0, 0 };
            const Glyph_control_box *cbox;

            if (_sel_start_char_idx > 0)
            {
                cbox = advance_to_glyph_at(font().source(), _text, 0, _sel_start_char_idx, pos);
                //_sel_start_pixel_pos = pos.x + cbox->bounds.x_min;
                _sel_start_pixel_pos = pos.x;
            }
            else {
                _sel_start_pixel_pos = 0;
            }

            if (_sel_end_char_idx > _sel_start_char_idx)
            {
                cbox = advance_to_glyph_at(font().source(), _text, _sel_start_char_idx, _sel_end_char_idx - 1, pos);
                //_sel_end_pixel_pos = pos.x + cbox->bounds.x_max;
                _sel_end_pixel_pos = pos.x + cbox->adv_x;
            }
            else {
                _sel_end_pixel_pos = _sel_start_pixel_pos;
            }
        }
        void collapse_selection_to_caret()
        {
            _sel_start_char_idx = _sel_end_char_idx = _caret_char_idx;
            _sel_start_pixel_pos = _sel_end_pixel_pos = _caret_pixel_pos;

            invalidate();
        }
        bool have_selection() const
        { 
            return _sel_start_char_idx < _sel_end_char_idx; 
        }
        void delete_selected()
        {
            auto new_text = _text.substr(0, _sel_start_char_idx) + _text.substr(_sel_end_char_idx);
            _caret_char_idx = _sel_start_char_idx;
            _caret_pixel_pos = _sel_start_pixel_pos;
            collapse_selection_to_caret();
            bring_caret_into_view();
            _text = new_text;
        }
        void move_caret_to_pointer_position(const Point &pos)
        {
            auto char_pos = find_character_at_pointer_position(pos);

            _caret_char_idx = char_pos.first;
            _caret_pixel_pos = char_pos.second;
            bring_caret_into_view();

            invalidate();
        }
        auto find_character_at_pointer_position(const Point &pos) -> std::pair<size_t, int>
        {
            auto inner_pos = convert_position_to_inner(pos);

            int x = 0;
            unsigned i;
            for (i = 0; i < _text.size(); i++)
            {
                auto glyph = font().source()->lookup_glyph(0, _text[i]);
                auto w = glyph->cbox.bounds.x_max - glyph->cbox.bounds.x_min;

                if (pos.x < _scroll_offs + x + w / 2) break;

                x += glyph->cbox.adv_x;
            }

            return { i, x };
        }
        void bring_caret_into_view()
        {
            while ((_scroll_offs + _caret_pixel_pos) < 0)
            {
                assert(_first_vis_char_idx > 0);
                _first_vis_char_idx --;
                auto glyph = font().source()->lookup_glyph(0, _text[_first_vis_char_idx]);
                _scroll_offs += glyph->cbox.adv_x;
            }

            while ((_scroll_offs + _caret_pixel_pos) > (int) _inner_rect.ext.w)
            {
                assert(_first_vis_char_idx < (_text.size() - 1));
                auto glyph = font().source()->lookup_glyph(0, _text[_first_vis_char_idx]);
                _first_vis_char_idx ++;
                _scroll_offs -= glyph->cbox.adv_x;
            }
        }

        // Styling
        // TODO: make into aspect ?
        auto selected_text_background_color() -> Color
        {
            if (has_focus())
            {
                return { 0.4f, 0.7f, 1, 1 };
            }
            else {
                return { 0.8f, 0.8f, 0.8f, 1 };
            }
        }
        auto caret_color() -> Color
        {
            return Color{0, 0, 0, 1};
        }

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

        Textbox__Layouter()
        {
            _padding = default_padding();
        }

        void change_font(const Rasterized_font *)
        {
            p()->_font = font;
            compute_text_extents();
            this->layout();
        }

        void compute_text_extents()
        {
            // TODO: free the font handle

            if (p()->_font.source())
            {
                //p()->_fnthnd = p()->root_widget()->get_font_handle(p()->_font);
                // TODO: support other cultures
                auto bbox = p()->_font.source()->compute_text_extents(0, U"My", 2);
                p()->_ascent = bbox.y_max;
                p()->_descent = bbox.y_min;
                p()->_mean_char_width = (bbox.width() + 1) / 2;
            }
        }

        void init_layout() override
        {
            compute_text_extents();
            this->layout();
        }
        auto get_minimal_size() -> Extents override
        {
            // TODO: replace "10" with const
            // TODO: adjust for border, too
            return { 
                _padding[3] + (Length) (10 * p()->_mean_char_width  ) + _padding[1], 
                _padding[0] + (Length) (p()->_ascent - p()->_descent) + _padding[2] 
            };
        }
        void layout() override
        {
            auto ext = p()->extents();

            p()->_inner_rect = {
                (Position) _padding[3], (Position) _padding[0],
                ext.w - _padding[3] - _padding[1], ext.h - _padding[0] - _padding[1]
            };

            p()->_txpos = { p()->_inner_rect.pos.x, p()->_inner_rect.pos.y + p()->_ascent };
        }

        // "Stylesheet"
        static constexpr auto default_padding() -> Padding { return { 3, 3, 3, 3 }; }
    };

} // ns cppgui
