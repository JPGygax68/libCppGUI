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
        _font = font;
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::set_text(const std::u32string &text)
    {
        _text = text;
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::change_text(const std::u32string &text)
    {
        set_text(text);

        if (font()) // TODO: is there a way to avoid this check ? (i.e. use different set_text() before font is set ?)
        {
            internal_select_all();
            _caret_char_idx = _sel_start_char_idx;
            _caret_pixel_pos = _sel_start_pixel_pos;
            bring_caret_into_view();
            invalidate();
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::init()
    {
        _fnthnd = root_widget()->get_font_handle(_font);
        _caret_char_idx = 0;
        _caret_pixel_pos = 0;
        _first_vis_char_idx = 0;
        internal_select_all();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_motion(const Position &pos)
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
    void Textbox<Config, With_layout>::mouse_button(const Position &pos, int button, Key_state state)
    {
        if (state == pressed)
        {
            if (root_widget()->focused_widget() != this)
            {
                root_widget()->set_focus_to(this);
            }

            move_caret_to_pointer_position(pos);
            collapse_selection_to_caret();
        }
        else {
            Widget_t::mouse_button(pos, button, state);
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_click(const Position &, int /*button*/, int count)
    {
        if (count == 2) // double-click
        {
            select_all();
        }
    }

    /*
    template<class GUIConfig, bool With_layout>
    void Textbox<GUIConfig, With_layout>::mouse_click(const Position &pos, int button, int count)
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
        insert_characters(text, count);
        invalidate();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::key_down(const Keycode &key)
    {
        if      (Keyboard::is_left     (key)) move_cursor_left    (Config::Keyboard::is_shift_down());
        else if (Keyboard::is_right    (key)) move_cursor_right   (Config::Keyboard::is_shift_down());
        else if (Keyboard::is_home     (key)) move_cursor_to_start(Config::Keyboard::is_shift_down());
        else if (Keyboard::is_end      (key)) move_cursor_to_end  (Config::Keyboard::is_shift_down());
        else if (Keyboard::is_backspace(key)) delete_before_caret ();
        else if (Keyboard::is_delete   (key)) delete_after_caret  ();
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
    inline void Textbox<Config, With_layout>::render(Canvas_t *r, const Position &offs)
    {
        fill(r, offs, rgba_to_native(r, {1, 1, 1, 1})); // TODO: (VERY MUCH) PROVISIONAL, GET REAL COLOR!

        auto pos = offs + position() + _txpos;

        r->set_clipping_rect(offs.x + _inner_rect.pos.x, offs.y + _inner_rect.pos.y, _inner_rect.ext.w, _inner_rect.ext.h);

        // Selection background
        auto bg_clr = selected_text_background_color();

        r->fill_rect(pos.x + _scroll_offs + _sel_start_pixel_pos, pos.y - _ascent, 
            _sel_end_pixel_pos - _sel_start_pixel_pos, _ascent - _descent, rgba_to_native(r, bg_clr));

        // Text
        if (!_text.empty())
        {
            r->render_text(_fnthnd, pos.x, pos.y, _text.data() + _first_vis_char_idx, _text.size() - _first_vis_char_idx, _inner_rect.ext.w);
        }

        // Caret
        if (has_focus())
        {
            r->fill_rect(pos.x + _scroll_offs + _caret_pixel_pos, pos.y - _ascent, 
                1, _ascent - _descent, rgba_to_native(r, caret_color())); // TODO: width from stylesheet
        }

        r->cancel_clipping();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_left(bool extend_sel)
    {
        if (_caret_char_idx > 0)
        {
            _caret_char_idx--;
            auto glyph = font()->lookup_glyph(0, _text[_caret_char_idx]); // TODO: support font variants ?
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
            auto glyph = font()->lookup_glyph(0, _text[_caret_char_idx]); // TODO: support font variants ?
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
                auto glyph = font()->lookup_glyph(0, _text[_caret_char_idx]); // TODO: support font variants ?
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
                auto glyph = font()->lookup_glyph(0, _text[_caret_char_idx]); // TODO: support font variants ?
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
            auto glyph = _font->lookup_glyph(0, text[i]); // TODO: support font variants ?
            _caret_pixel_pos += glyph->cbox.adv_x;
        }

        _caret_char_idx += count;
        collapse_selection_to_caret();
        bring_caret_into_view();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::delete_before_caret()
    {
        // TODO: support deleting selection
        if (have_selection())
        {
            delete_selected();
            invalidate();
        }
        else if (_caret_char_idx > 0)
        {
            auto glyph = _font->lookup_glyph(0, _text[_caret_char_idx - 1]); // TODO: support font variants ?
            _caret_pixel_pos -= glyph->cbox.adv_x;
            _caret_char_idx --;
            _text = _text.substr(0, _caret_char_idx) + _text.substr(_caret_char_idx + 1);
            collapse_selection_to_caret();
            bring_caret_into_view();
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
    void Textbox<Config, With_layout>::move_caret_to_pointer_position(const Position &pos)
    {
        auto char_pos = find_character_at_pointer_position(pos);

        _caret_char_idx = char_pos.first;
        _caret_pixel_pos = char_pos.second;
        bring_caret_into_view();

        invalidate();
    }

    template<class Config, bool With_layout>
    auto Textbox<Config, With_layout>::find_character_at_pointer_position(const Position & pos) -> std::pair<size_t, int>
    {
        auto inner_pos = convert_position_to_inner(pos);

        int x = 0;
        unsigned i;
        for (i = 0; i < _text.size(); i++)
        {
            auto glyph = font()->lookup_glyph(0, _text[i]);
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
            auto glyph = font()->lookup_glyph(0, _text[_first_vis_char_idx]);
            _scroll_offs += glyph->cbox.adv_x;
        }

        while ((_scroll_offs + _caret_pixel_pos) > (int) _inner_rect.ext.w)
        {
            assert(_first_vis_char_idx < (_text.size() - 1));
            auto glyph = font()->lookup_glyph(0, _text[_first_vis_char_idx]);
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

        Position pos = { 0, 0 };
        const Glyph_control_box *cbox;

        if (_sel_start_char_idx > 0)
        {
            cbox = advance_to_glyph_at(font(), _text, 0, _sel_start_char_idx, pos);
            //_sel_start_pixel_pos = pos.x + cbox->bounds.x_min;
            _sel_start_pixel_pos = pos.x;
        }
        else {
            _sel_start_pixel_pos = 0;
        }

        if (_sel_end_char_idx > _sel_start_char_idx)
        {
            cbox = advance_to_glyph_at(font(), _text, _sel_start_char_idx, _sel_end_char_idx - 1, pos);
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
    }

    template<class Config, bool With_layout>
    void cppgui::Textbox<Config, With_layout>::delete_selected()
    {
        _text = _text.substr(0, _sel_start_char_idx) + _text.substr(_sel_end_char_idx);
        _caret_char_idx = _sel_start_char_idx;
        _caret_pixel_pos = _sel_start_pixel_pos;
        collapse_selection_to_caret();
        bring_caret_into_view();
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    inline void Textbox_layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        compute_label_size();
        this->layout();
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Textbox_layouter<Config, true>::Aspect<Aspect_parent>::change_font(const Rasterized_font *font)
    {
        p()->_font = font;
        compute_label_size();
        this->layout();
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Textbox_layouter<Config, true>::Aspect<Aspect_parent>::compute_label_size()
    {
        // TODO: free the font handle

        if (p()->_font)
        {
            p()->_fnthnd = p()->root_widget()->get_font_handle(p()->_font);
            // TODO: support other cultures
            auto bbox = p()->_font->compute_text_extents(0, U"My", 2);
            p()->_ascent = bbox.y_max;
            p()->_descent = bbox.y_min;
            p()->_mean_char_width = (bbox.width() + 1) / 2;
        }
    }

    template <class Config>
    template <class Aspect_parent>
    inline auto Textbox_layouter<Config, true>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        // TODO: replace "10" with const
        // TODO: adjust for border, padding
        return { (unsigned) (10 * p()->_mean_char_width), (unsigned) (p()->_ascent - p()->_descent) };
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Textbox_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        // TODO: adjust for border, padding
        p()->_inner_rect = p()->rectangle();
        p()->_txpos = {0, p()->_ascent };
        //p()->_txmaxlen = p()->extents().w;
    }

} // ns cppgui
