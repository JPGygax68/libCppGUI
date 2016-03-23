#include "./Textbox.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::set_font(const Rasterized_font *font)
    {
        if (font != _font)
        {
            _font = font;
            font_changed();
            _fnthnd = root_widget()->get_font_handle(_font);
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::set_text(const std::u32string &text)
    {
        _text = text;

        if (font()) // TODO: is there a way to avoid this check ? (i.e. use different set_text() before font is set ?)
        {
            internal_select_all();
            invalidate();
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::init()
    {
        internal_select_all();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_click(const Position &pos, int button, int count)
    {
        if (button == 1 && count == 1)
        {
            if (root_widget()->focused_widget() != this)
            {
                root_widget()->set_focus_to(this);
            }

            move_caret_to_pointer_position(pos);
        }
        else {
            Widget_t::mouse_click(pos, button, count);
        }
    }

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
        root_widget()->push_cursor( Config::Cursor::get_ibeam_cursor() );
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
    inline void Textbox<Config, With_layout>::render(Renderer *r, const Position &offs)
    {
        fill(r, offs, rgba_to_native(r, {1, 1, 1, 1})); // TODO: (VERY MUCH) PROVISIONAL, GET REAL COLOR!

        auto pos = offs + position() + _txpos;

        // Selection background
        // TODO: color from stylesheet
        r->fill_rect(pos.x + _sel_start_offs, pos.y - _ascent, _sel_end_offs - _sel_start_offs, _ascent - _descent, rgba_to_native(r, { 0.2f, 0.5f, 1, 0.5f }));

        // Text
        if (!_text.empty())
        {
            r->render_text(_fnthnd, pos.x, pos.y, _text.data(), _text.size());
        }

        // Caret
        r->fill_rect(pos.x + _caret_offs, pos.y - _ascent, 2, _ascent - _descent, rgba_to_native(r, { 0, 0.3f, 0.8f, 0.5f })); // TODO: width, color
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::font_changed()
    {
        if (font())
        {
            // TODO: support other cultures
            auto bbox = font()->compute_text_extents(0, U"My", 2);
            _ascent = bbox.y_max;
            _descent = bbox.y_min;
            _mean_char_width = (bbox.width() + 1) / 2;
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_left(bool extend_sel)
    {
        if (_caret_pos > 0)
        {
            _caret_pos--;
            auto glyph = font()->lookup_glyph(0, _text[_caret_pos]); // TODO: support font variants ?
            _caret_offs -= glyph->cbox.adv_x; // TODO: support vertical advance
            if (extend_sel)
            {
                if (_caret_pos == _sel_start_pos - 1)
                {
                    _sel_start_pos = _caret_pos;
                    _sel_start_offs = _caret_offs;
                }
                else if (_caret_pos == _sel_end_pos - 1)
                {
                    _sel_end_pos = _caret_pos;
                    _sel_end_offs = _caret_offs;
                }
            }
            else {
                collapse_selection_to_caret();
            }
            invalidate();
        }
        else {
            // TODO: produce warning "bump" sound or effect
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_right(bool extend_sel)
    {
        if (_caret_pos < _text.size())
        {
            auto glyph = font()->lookup_glyph(0, _text[_caret_pos]); // TODO: support font variants ?
            _caret_pos ++;
            _caret_offs += glyph->cbox.adv_x; // TODO: support vertical advance
            if (extend_sel)
            {
                if (_caret_pos == _sel_end_pos + 1)
                {
                    _sel_end_pos = _caret_pos;
                    _sel_end_offs = _caret_offs;
                }
                else if (_caret_pos == _sel_start_pos + 1)
                {
                    _sel_start_pos = _caret_pos;
                    _sel_start_offs = _caret_offs;
                }
            }
            else {
                collapse_selection_to_caret();
            }
            invalidate();
        }
        else {
            // TODO: produce warning "bump" sound or effect
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_to_start(bool extend_sel)
    {
        if (_caret_pos > 0)
        {
            while (_caret_pos > 0)
            {
                _caret_pos--;
                auto glyph = font()->lookup_glyph(0, _text[_caret_pos]); // TODO: support font variants ?
                _caret_offs -= glyph->cbox.adv_x; // TODO: support vertical advance
            }
            if (extend_sel)
            {
                _sel_start_pos = _caret_pos;
                _sel_start_offs = _caret_offs;
            }
            else {
                collapse_selection_to_caret();
            }
            invalidate();
        }
        else {
            // TODO: produce warning "bump" sound or effect
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_to_end(bool extend_sel)
    {
        if (_caret_pos < _text.size())
        {
            while (_caret_pos < _text.size())
            {
                auto glyph = font()->lookup_glyph(0, _text[_caret_pos]); // TODO: support font variants ?
                _caret_pos++;
                _caret_offs += glyph->cbox.adv_x; // TODO: support vertical advanc
            }
            if (extend_sel)
            {
                _sel_end_pos = _caret_pos;
                _sel_end_offs = _caret_offs;
            }
            else {
                collapse_selection_to_caret();
            }
            invalidate();
        }
        else {
            // TODO: produce warning "bump" sound or effect
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::insert_characters(const char32_t * text, size_t count)
    {
        _text = _text.substr(0, _caret_pos) + std::u32string{ text, count } + _text.substr(_caret_pos); // TODO: support selection

        for (size_t i = 0; i < count; i++)
        {
            auto glyph = _font->lookup_glyph(0, text[i]); // TODO: support font variants ?
            _caret_offs += glyph->cbox.adv_x;
        }

        _caret_pos += count;
        collapse_selection_to_caret();
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
        else if (_caret_pos > 0)
        {
            auto glyph = _font->lookup_glyph(0, _text[_caret_pos - 1]); // TODO: support font variants ?
            _caret_offs -= glyph->cbox.adv_x;
            _caret_pos --;
            _text = _text.substr(0, _caret_pos) + _text.substr(_caret_pos + 1);
            collapse_selection_to_caret();
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
        else if (_caret_pos < _text.size())
        {
            _text = _text.substr(0, _caret_pos) + _text.substr(_caret_pos + 1);
            collapse_selection_to_caret();
            invalidate();
        }
        else {
            // TODO: "bump"
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_caret_to_pointer_position(const Position &pos)
    {
        auto inner_pos = convert_position_to_inner(pos);

        int x = 0;
        unsigned i;
        for (i = 0; i < _text.size(); i++)
        {
            auto glyph = font()->lookup_glyph(0, _text[i]);
            auto w = glyph->cbox.bounds.x_max - glyph->cbox.bounds.x_min;

            if (pos.x < x + w / 2) break;

            x += glyph->cbox.adv_x;
        }

        _caret_offs = x;
        _caret_pos = i;

        invalidate();
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::internal_select_all()
    {
        _sel_start_pos = 0;
        _sel_end_pos   = _text.size();

        recalc_selection_strip();

        _caret_pos = _sel_start_pos;
        _caret_offs = _sel_start_offs;
    }

    /** New definition:
        Computes the graphical span from the insertion point of the first selected
        character to the insertion point after the last selected character.
        Old definition:
        Computes the graphical span from the left edge of the first selected character
        to the right edge of the last selected character.

        Must be called when _sel_start_pos and _sel_end_pos have been changed (which
        usually happens together).
     */
    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::recalc_selection_strip()
    {
        // TODO: support vertical scripts

        Position pos = { 0, 0 };
        const Glyph_control_box *cbox;

        if (_sel_start_pos > 0)
        {
            cbox = advance_to_glyph_at(font(), _text, 0, _sel_start_pos, pos);
            //_sel_start_offs = pos.x + cbox->bounds.x_min;
            _sel_start_offs = pos.x;
        }
        else {
            _sel_start_offs = 0;
        }

        if (_sel_end_pos > _sel_start_pos)
        {
            cbox = advance_to_glyph_at(font(), _text, _sel_start_pos, _sel_end_pos - 1, pos);
            //_sel_end_offs = pos.x + cbox->bounds.x_max;
            _sel_end_offs = pos.x + cbox->adv_x;
        }
        else {
            _sel_end_offs = _sel_start_offs;
        }
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::collapse_selection_to_caret()
    {
        _sel_start_pos = _sel_end_pos = _caret_pos;
        _sel_start_offs = _sel_end_offs = _caret_offs;
    }

    template<class Config, bool With_layout>
    void cppgui::Textbox<Config, With_layout>::delete_selected()
    {
        _text = _text.substr(0, _sel_start_pos) + _text.substr(_sel_end_pos);
        _caret_pos = _sel_start_pos;
        _caret_offs = _sel_start_offs;
        collapse_selection_to_caret();
    }

    // Layouter aspect ----------------------------------------------

    template <class Config>
    template <class Aspect_parent>
    inline auto Textbox_layouter<Config, true>::Aspect<Aspect_parent>::minimal_size() -> Extents
    {
        // TODO: replace "10" with const
        return { (unsigned) (10 * p()->_mean_char_width), (unsigned) (p()->_ascent - p()->_descent) };
    };

    template<class Config>
    template<class Aspect_parent>
    inline void Textbox_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        p()->_txpos = {0, p()->_ascent };
    }

} // ns cppgui
