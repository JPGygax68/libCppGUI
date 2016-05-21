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
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::set_text(const std::u32string &text)
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::set_text(const std::string &text)
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::change_text(const std::u32string &text)
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::change_text(const std::string &text)
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::init()
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::compute_view_from_data()
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_motion(const Point &pos)
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_button(const Point &pos, int button, Key_state state)
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_click(const Point &, int /*button*/, int count)
    {
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
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_enter()
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::mouse_exit()
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::gained_focus()
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::loosing_focus()
    {
    }

    template<class Config, bool With_layout>
    inline void Textbox<Config, With_layout>::render(Canvas_t *r, const Point &offs)
    {
    }

    template<class Config, bool With_layout>
    bool Textbox<Config, With_layout>::handle_key_down(const Keycode &key)
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_left(bool extend_sel)
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_right(bool extend_sel)
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_to_start(bool extend_sel)
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_cursor_to_end(bool extend_sel)
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::insert_characters(const char32_t * text, size_t count)
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::delete_before_caret()
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::delete_after_caret()
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::select_all()
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::move_caret_to_pointer_position(const Point &pos)
    {
    }

    template<class Config, bool With_layout>
    auto Textbox<Config, With_layout>::find_character_at_pointer_position(const Point & pos) -> std::pair<size_t, int>
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::bring_caret_into_view()
    {
    }

    template<class Config, bool With_layout>
    auto Textbox<Config, With_layout>::selected_text_background_color() -> Color
    {
    }

    template<class Config, bool With_layout>
    auto Textbox<Config, With_layout>::caret_color() -> Color
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::internal_select_all()
    {
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
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::collapse_selection_to_caret()
    {
    }

    template<class Config, bool With_layout>
    void Textbox<Config, With_layout>::delete_selected()
    {
    }

    // Layouter aspect ----------------------------------------------

    template<class Config, class Parent>
    inline void Textbox__Layouter<Config, true, Parent>::init_layout()
    {
    }

    template<class Config, class Parent>
    Textbox__Layouter<Config, true, Parent>::Textbox__Layouter()
    {
    }

    template<class Config, class Parent>
    inline void Textbox__Layouter<Config, true, Parent>::change_font(const Rasterized_font *font)
    {
    }

    template<class Config, class Parent>
    inline void Textbox__Layouter<Config, true, Parent>::compute_text_extents()
    {
    }

    template<class Config, class Parent>
    inline auto Textbox__Layouter<Config, true, Parent>::get_minimal_size() -> Extents
    {
    }

    template<class Config, class Parent>
    inline void Textbox__Layouter<Config, true, Parent>::layout()
    {
    }

} // ns cppgui
