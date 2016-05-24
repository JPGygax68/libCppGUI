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

//#include <locale>
//#include <codecvt>

#include "./unicode.hpp"

#include "./Textbox.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::set_font(const Rasterized_font *font)
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

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::set_text(const std::u32string &text)
    {
        _text = text;
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::set_text(const std::string &text)
    {
        _text = utf8_to_utf32(text);
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::change_text(const std::u32string &text)
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

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::change_text(const std::string &text)
    {
        change_text( utf8_to_utf32(text) );
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::init()
    {
        _font.translate( root_widget()->canvas() );
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::compute_view_from_data()
    {
        _caret_char_idx = 0;
        _caret_pixel_pos = 0;
        _first_vis_char_idx = 0;
        internal_select_all();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_motion(const Point &pos)
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

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_button(const Point &pos, int button, Key_state state, Count clicks)
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

        Widget_t::mouse_button(pos, button, state, clicks);
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_click(const Point &, int /*button*/, Count count)
    {
        if (count == 2) // double-click
        {
            select_all();
        }
    }

    /*
    template<class GUIConfig, bool With_layout>
    void Textbox<GUIConfig, With_layout>::mouse_click(const Point &pos, int button, int count)
    {
        if (button == 1 && count == 1)
        {
            if (root_widget()->focused_widget() != this)
            {
                root_widget()->set_focus_to(this);
            }

            move_caret_to_pointer_position(pos);
            collapse_selection_to_caret();
        }
        else {
            Widget_t::mouse_click(pos, button, count);
        }
    }
    */

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::text_input(const char32_t *text, size_t count)
    {
        delete_selected(); // TODO: make optional
        insert_characters(text, count);
        invalidate();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_enter()
    {
        root_widget()->push_cursor( Config::Mouse::get_ibeam_cursor() );
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_exit()
    {
        root_widget()->pop_cursor();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::gained_focus()
    {
        // TODO: more...
        invalidate();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::loosing_focus()
    {
        // TODO: more
        invalidate();
    }

    template<class Config, bool With_layout>
    inline void Textbox<Config, With_layout>::render(Canvas_t *r, const Point &offs)
    {
        fill(r, offs, rgba_to_native({ 1, 1, 1, 1 })); // TODO: (VERY MUCH) PROVISIONAL, GET REAL COLOR!

        auto pos = offs + position();

        r->set_clipping_rect(pos.x + _inner_rect.pos.x, pos.y + _inner_rect.pos.y, _inner_rect.ext.w, _inner_rect.ext.h);

        // Selection background
        auto bg_clr = selected_text_background_color();

        r->fill_rect(pos.x + _txpos.x + _scroll_offs + _sel_start_pixel_pos, pos.y + _txpos.y - _ascent, 
            _sel_end_pixel_pos - _sel_start_pixel_pos, _ascent - _descent, rgba_to_native(bg_clr));

        // Text
        if (!_text.empty() && _first_vis_char_idx < _text.size())
        {
            r->render_text(_font.get(), pos.x + _txpos.x, pos.y + _txpos.y, 
                _text.data() + _first_vis_char_idx, _text.size() - _first_vis_char_idx, _inner_rect.ext.w);
        }

        // Caret
        if (has_focus())
        {
            r->fill_rect(pos.x + _txpos.x + _scroll_offs + _caret_pixel_pos, pos.y + _txpos.y - _ascent, 
                1, _ascent - _descent, rgba_to_native(caret_color())); // TODO: width from stylesheet
        }

        r->cancel_clipping();
    }

    template<class Config, bool With_layout>
    bool Textbox<Config, With_layout>::handle_key_down(const Keycode &key)
    {
        if      (Keyboard::is_left     (key)) { move_cursor_left    (Config::Keyboard::is_shift_down()); return true; }
        else if (Keyboard::is_right    (key)) { move_cursor_right   (Config::Keyboard::is_shift_down()); return true; }
        else if (Keyboard::is_home     (key)) { move_cursor_to_start(Config::Keyboard::is_shift_down()); return true; }
        else if (Keyboard::is_end      (key)) { move_cursor_to_end  (Config::Keyboard::is_shift_down()); return true; }
        else if (Keyboard::is_backspace(key)) { delete_before_caret ();                                ; return true; }
        else if (Keyboard::is_delete   (key)) { delete_after_caret  ();                                ; return true; }
        else return false;
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_left(bool extend_sel)
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

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_right(bool extend_sel)
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

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_to_start(bool extend_sel)
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

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_to_end(bool extend_sel)
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

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::insert_characters(const char32_t * text, size_t count)
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

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::delete_before_caret()
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

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::delete_after_caret()
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

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::select_all()
    {
        internal_select_all();

        _caret_char_idx = _sel_end_char_idx;
        _caret_pixel_pos = _sel_end_pixel_pos;
        bring_caret_into_view();

        invalidate();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_caret_to_pointer_position(const Point &pos)
    {
        auto char_pos = find_character_at_pointer_position(pos);

        _caret_char_idx = char_pos.first;
        _caret_pixel_pos = char_pos.second;
        bring_caret_into_view();

        invalidate();
    }

    template<class Config, bool With_layout>
    auto Textbox<Config, With_layout>::find_character_at_pointer_position(const Point & pos) -> std::pair<size_t, int>
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

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::bring_caret_into_view()
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

    template<class Config, bool With_layout>
    auto Textbox<Config, With_layout>::selected_text_background_color() -> Color
    {
        if (has_focus())
        {
            return { 0.4f, 0.7f, 1, 1 };
        }
        else {
            return { 0.8f, 0.8f, 0.8f, 1 };
        }
    }

    template<class Config, bool With_layout>
    auto Textbox<Config, With_layout>::caret_color() -> Color
    {
        return Color{0, 0, 0, 1};
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::internal_select_all()
    {
        _sel_start_char_idx = 0;
        _sel_end_char_idx   = _text.size();

        recalc_selection_strip();
    }

    /** New definition:
        Computes the graphical span from the insertion point of the first selected
        character to the insertion point after the last selected character.
        Old definition:
        Computes the graphical span from the left edge of the first selected character
        to the right edge of the last selected character.

        Must be called when _sel_start_char_idx and _sel_end_char_idx have been changed (which
        usually happens together).
     */
    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::recalc_selection_strip()
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

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::collapse_selection_to_caret()
    {
        _sel_start_char_idx = _sel_end_char_idx = _caret_char_idx;
        _sel_start_pixel_pos = _sel_end_pixel_pos = _caret_pixel_pos;

        invalidate();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::delete_selected()
    {
        auto new_text = _text.substr(0, _sel_start_char_idx) + _text.substr(_sel_end_char_idx);
        _caret_char_idx = _sel_start_char_idx;
        _caret_pixel_pos = _sel_start_pixel_pos;
        collapse_selection_to_caret();
        bring_caret_into_view();
        _text = new_text;
    }

    // Layouter aspect ----------------------------------------------

    template<class Config, class Parent>
    inline void Textbox__Layouter<Config, true, Parent>::init_layout()
    {
        compute_text_extents();
        this->layout();
    }

    template<class Config, class Parent>
    Textbox__Layouter<Config, true, Parent>::Textbox__Layouter()
    {
        _padding = default_padding();
    }

    template<class Config, class Parent>
    inline void Textbox__Layouter<Config, true, Parent>::change_font(const Rasterized_font *font)
    {
        p()->_font = font;
        compute_text_extents();
        this->layout();
    }

    template<class Config, class Parent>
    inline void Textbox__Layouter<Config, true, Parent>::compute_text_extents()
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

    template<class Config, class Parent>
    inline auto Textbox__Layouter<Config, true, Parent>::get_minimal_size() -> Extents
    {
        // TODO: replace "10" with const
        // TODO: adjust for border, too
        return { 
            _padding[3] + (Length) (10 * p()->_mean_char_width  ) + _padding[1], 
            _padding[0] + (Length) (p()->_ascent - p()->_descent) + _padding[2] 
        };
    }

    template<class Config, class Parent>
    inline void Textbox__Layouter<Config, true, Parent>::layout()
    {
        auto ext = p()->extents();

        p()->_inner_rect = {
            (Position) _padding[3], (Position) _padding[0],
            ext.w - _padding[3] - _padding[1], ext.h - _padding[0] - _padding[1]
        };

        p()->_txpos = { p()->_inner_rect.pos.x, p()->_inner_rect.pos.y + p()->_ascent };
    }

} // ns cppgui
