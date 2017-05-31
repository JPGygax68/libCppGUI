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

//#include <locale>
//#include <codecvt>
#include "./unicode.hpp"
#include "./Textbox.hpp"
#include "./Root_widget.hpp"


namespace cppgui {
    
    Textbox::Textbox():
        _size( 10 ) // TODO: use static method for default
    {
    }

    void Textbox::on_done(Done_handler handler)
    {
        assert(!_on_done);

        _on_done = handler;
    }

    void Textbox::set_font(const Rasterized_font *font)
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

    void Textbox::set_size(Count chars)
    {
        _size = chars;
    }

    void Textbox::set_text(const std::u32string &text)
    {
        _text = text;
    }

    void Textbox::set_text(const std::string &text)
    {
        _text = utf8_to_utf32(text);
    }

    void Textbox::change_text(const std::u32string &text)
    {
        set_text(text);

        _first_vis_char_idx = 0;
        _scroll_offs = 0;

        if (font().rasterized) // TODO: is there a way to avoid this check ? (i.e. use different set_text() before font is set ?)
        {
            internal_select_all();
            _caret_char_idx = _sel_start_char_idx;
            _caret_pixel_pos = _sel_start_pixel_pos;
            bring_caret_into_view();
            this->invalidate();
        }
    }

    void Textbox::change_text(const std::string &text)
    {
        change_text( utf8_to_utf32(text) );
    }

    void Textbox::init(Canvas *c)
    {
        _font.translate(c);
    }

    void Textbox::compute_view_from_data()
    {
        _caret_char_idx = 0;
        _caret_pixel_pos = 0;
        _first_vis_char_idx = 0;
        internal_select_all();
    }

    bool Textbox::mouse_motion(const Point &pos)
    {
        if (is_mouse_button_down(1))
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
            return true;
        }
        else {
            return Super::mouse_motion(pos);
        }
    }

    bool Textbox::mouse_button(const Point &p, int button, Key_state state, Count clicks)
    {
        if (state == pressed)
        {
            if (!has_focus())
            {
                take_focus();
            }

            move_caret_to_pointer_position(p);
            collapse_selection_to_caret();

            return true;
        }

        return Super::mouse_button(p, button, state, clicks);
    }

    bool Textbox::mouse_click(const Point &, int /*button*/, Count count)
    {
        if (count == 2) // double-click
        {
            select_all();
        }

        return true;
    }

    /*
    void Textbox::mouse_click(const Point &p, int button, int count)
    {
        if (button == 1 && count == 1)
        {
            if (root_widget()->focused_widget() != this)
            {
                root_widget()->set_focus_to(this);
            }

            move_caret_to_pointer_position(p);
            collapse_selection_to_caret();
        }
        else {
            Widget::mouse_click(p, button, count);
        }
    }
    */

    bool Textbox::text_input(const char32_t *text, size_t count)
    {
        delete_selected(); // TODO: make optional
        insert_characters(text, count);
        invalidate();

        return true;
    }

    void Textbox::mouse_enter()
    {
        root_widget()->push_cursor( get_ibeam_cursor() );
    }

    void Textbox::mouse_exit()
    {
        root_widget()->pop_cursor();
    }

    void Textbox::gained_focus()
    {
        // TODO: more...

        Super::gained_focus();
    }

    void Textbox::loosing_focus()
    {
        notify_done();

        this->invalidate();
    }

    void Textbox::render(Canvas *c, const Point &offs)
    {
        auto p = offs + origin();

        draw_background_and_border(c, p, bbox(), visual_states());

        //c->set_clipping_rect(p.x + _inner_rect.p.x, p.y + _inner_rect.p.y, _inner_rect.ext.w, _inner_rect.ext.h);
        c->push_clipping_rect(Rectangle{_inner_bbox} + p);

        // Selection background
        auto sel_bbox = _inner_bbox;
        sel_bbox.x_min = _scroll_offs + _sel_start_pixel_pos;
        sel_bbox.x_max = _scroll_offs + _sel_end_pixel_pos;
        c->fill_rect(Rectangle{sel_bbox} + p, selected_text_background_color());

        // Text
        if (!_text.empty() && _first_vis_char_idx < _text.size())
        {
            c->set_text_color({0, 0, 0, 1}); // TODO: make configurable!
            c->render_text(_font.get(), p.x, p.y, 
                _text.data() + _first_vis_char_idx, _text.size() - _first_vis_char_idx, _inner_bbox.width());
        }

        // Caret
        if (has_focus())
        {
            // TODO: get caret width from stylesheet
            Rectangle r {p.x + _scroll_offs + _caret_pixel_pos, p.y + - _ascent, 1, _ascent - _descent};
            c->fill_rect(r, caret_color());
        }

        //c->cancel_clipping();
        c->pop_clipping_rect();
    }

    bool Textbox::key_down(const Keycode &key)
    {
        if (is_left     (key)) { move_cursor_left    (is_shift_down()); return true; }
        if (is_right    (key)) { move_cursor_right   (is_shift_down()); return true; }
        if (is_home     (key)) { move_cursor_to_start(is_shift_down()); return true; }
        if (is_end      (key)) { move_cursor_to_end  (is_shift_down()); return true; }
        if (is_backspace(key)) { delete_before_caret ()               ; return true; }
        if (is_delete   (key)) { delete_after_caret  ()               ; return true; }
        if (is_return   (key)) { done                ()               ; return true; }                

        return Super::key_down(key);
    }

    void Textbox::move_cursor_left(bool extend_sel)
    {
        if (_caret_char_idx > 0)
        {
            _caret_char_idx--;
            auto glyph = font().rasterized->lookup_glyph(0, _text[_caret_char_idx]); // TODO: support font variants ?
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

    void Textbox::move_cursor_right(bool extend_sel)
    {
        if (_caret_char_idx < _text.size())
        {
            auto glyph = font().rasterized->lookup_glyph(0, _text[_caret_char_idx]); // TODO: support font variants ?
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

    void Textbox::move_cursor_to_start(bool extend_sel)
    {
        if (_caret_char_idx > 0)
        {
            while (_caret_char_idx > 0)
            {
                _caret_char_idx--;
                auto glyph = font().rasterized->lookup_glyph(0, _text[_caret_char_idx]); // TODO: support font variants ?
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

    void Textbox::move_cursor_to_end(bool extend_sel)
    {
        if (_caret_char_idx < _text.size())
        {
            while (_caret_char_idx < _text.size())
            {
                auto glyph = font().rasterized->lookup_glyph(0, _text[_caret_char_idx]); // TODO: support font variants ?
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

    void Textbox::insert_characters(const char32_t * text, size_t count)
    {
        _text = _text.substr(0, _caret_char_idx) + std::u32string{ text, count } + _text.substr(_caret_char_idx); // TODO: support selection

        for (size_t i = 0; i < count; i++)
        {
            auto glyph = _font.rasterized->lookup_glyph(0, text[i]); // TODO: support font variants ?
            _caret_pixel_pos += glyph->cbox.adv_x;
        }

        _caret_char_idx += count;
        collapse_selection_to_caret();
        bring_caret_into_view();
    }

    void Textbox::delete_before_caret()
    {
        if (have_selection())
        {
            delete_selected();
            invalidate();
        }
        else if (_caret_char_idx > 0)
        {
            auto glyph = _font.rasterized->lookup_glyph(0, _text[_caret_char_idx - 1]); // TODO: support font variants ?
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

    void Textbox::delete_after_caret()
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

    void Textbox::select_all()
    {
        internal_select_all();

        _caret_char_idx = _sel_end_char_idx;
        _caret_pixel_pos = _sel_end_pixel_pos;
        bring_caret_into_view();

        invalidate();
    }

    void Textbox::done()
    {
        notify_done();
    }

    void Textbox::move_caret_to_pointer_position(const Point &pos)
    {
        auto char_pos = find_character_at_pointer_position(pos);

        _caret_char_idx = char_pos.first;
        _caret_pixel_pos = char_pos.second;
        bring_caret_into_view();

        invalidate();
    }

    auto Textbox::find_character_at_pointer_position(const Point &p) -> std::pair<size_t, int>
    {
        int x = 0;
        unsigned i;
        for (i = 0; i < _text.size(); i++)
        {
            auto glyph = font().rasterized->lookup_glyph(0, _text[i]);
            auto w = glyph->cbox.bounds.x_max - glyph->cbox.bounds.x_min;

            if (p.x < _scroll_offs + x + w / 2) break;

            x += glyph->cbox.adv_x;
        }

        return { i, x };
    }

    void Textbox::bring_caret_into_view()
    {
        while ((_scroll_offs + _caret_pixel_pos) < 0)
        {
            assert(_first_vis_char_idx > 0);
            _first_vis_char_idx --;
            auto glyph = font().rasterized->lookup_glyph(0, _text[_first_vis_char_idx]);
            _scroll_offs += glyph->cbox.adv_x;
        }

        while ((_scroll_offs + _caret_pixel_pos) > _inner_bbox.width())
        {
            assert(_first_vis_char_idx < (_text.size() - 1));
            auto glyph = font().rasterized->lookup_glyph(0, _text[_first_vis_char_idx]);
            _first_vis_char_idx ++;
            _scroll_offs -= glyph->cbox.adv_x;
        }
    }

    void Textbox::notify_done()
    {
        if (_on_done) _on_done( text() );
    }

    auto Textbox::selected_text_background_color() -> RGBA
    {
        if (has_focus())
        {
            return { 0.5f, 0.8f, 1, 1 };
        }
        else {
            return { 0.8f, 0.8f, 0.8f, 1 };
        }
    }

    auto Textbox::caret_color() -> RGBA
    {
        return {0, 0, 0, 1};
    }

    void Textbox::internal_select_all()
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
        usually happen together).
     */
    void Textbox::recalc_selection_strip()
    {
        // TODO: support vertical scripts

        Point pos = { 0, 0 };
        const Glyph_control_box *cbox;

        if (_sel_start_char_idx > 0)
        {
            //cbox = advance_to_glyph_at(font().rasterized, _text, 0, _sel_start_char_idx, pos);
            //_sel_start_pixel_pos = p.x + cbox->bounds.x_min;
            _sel_start_pixel_pos = pos.x;
        }
        else {
            _sel_start_pixel_pos = 0;
        }

        if (_sel_end_char_idx > _sel_start_char_idx)
        {
            cbox = advance_to_glyph_at(font().rasterized, _text, _sel_start_char_idx, _sel_end_char_idx - 1, pos);
            //_sel_end_pixel_pos = p.x + cbox->bounds.x_max;
            _sel_end_pixel_pos = pos.x + cbox->adv_x;
        }
        else {
            _sel_end_pixel_pos = _sel_start_pixel_pos;
        }
    }

    void Textbox::collapse_selection_to_caret()
    {
        _sel_start_char_idx = _sel_end_char_idx = _caret_char_idx;
        _sel_start_pixel_pos = _sel_end_pixel_pos = _caret_pixel_pos;

        invalidate();
    }

    void Textbox::delete_selected()
    {
        auto new_text = _text.substr(0, _sel_start_char_idx) + _text.substr(_sel_end_char_idx);
        _caret_char_idx = _sel_start_char_idx;
        _caret_pixel_pos = _sel_start_pixel_pos;
        collapse_selection_to_caret();
        bring_caret_into_view();
        _text = new_text;
    }

    // Layouter aspect ----------------------------------------------

    /*
     * TODO: changing the font at runtime is a risky endeavour, as the bounding box has already
     * been set and the new font may not fit well at all. Maybe it would be best to simply remove
     * this methods.
     */
    void Textbox::change_font(const Rasterized_font *font)
    {
        _font.assign(font);
        compute_text_extents();
        compute_layout(bbox());
    }

    void Textbox::compute_text_extents()
    {
        // TODO: free the font handle

        if (_font.rasterized)
        {
            //p()->_fnthnd = p()->root_widget()->get_font_handle(p()->_font);
            // TODO: support other cultures
            auto bbox = _font.rasterized->compute_text_extents(0, U"My", 2);
            _ascent = bbox.y_max;
            _descent = bbox.y_min;
            _mean_char_width = (bbox.width() + 1) / 2;
        }
    }

    void Textbox::init_layout()
    {
        compute_text_extents();
    }

    auto Textbox::get_minimal_bounds() -> Bbox
    {
        Bbox b{ 0, size() * _mean_char_width, _descent, _ascent };

        return adjust_box_bounds(b);
    }

    void Textbox::compute_layout(Bbox_cref b)
    {
        _inner_bbox = adjust_box_bounds(b, -1);
    }

} // ns cppgui
