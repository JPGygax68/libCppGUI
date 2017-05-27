#pragma once

#include <cppgui/Window.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>
#include <cppgui/Button.hpp>
#include <cppgui/Checkbox.hpp>
#include <cppgui/Glyph_button.hpp>
#include <cppgui/Vertical_scrollbar.hpp>
#include <cppgui/Listbox.hpp>
#include <cppgui/Dropdown_button.hpp>

#include "./Popup_test_button.hpp"


class Test_window: public cppgui::Window
{
public:
    explicit Test_window();

protected:

    void create_ui() override;
    void adjust_layout() override;

    void before_draw_ui(void *context) override;

private:
    using Test_button = cppgui::Boxed<cppgui::Glyph_button_base, cppgui::Borderless_button_box_styles>;

    std::vector<Test_button>            _button_list;

    cppgui::Label                       _label;
    cppgui::Textbox                     _textbox;
    cppgui::Button                      _button;
    cppgui::Checkbox                    _checkbox;
    cppgui::Glyph_button                _glyphbutton;
    cppgui::Boxed_vertical_scrollbar    _vscrollbar;
    cppgui::Listbox                     _listbox;
    //cppgui::Dropdown_button             _dropdownbtn;
    cppgui::Popup_test_button           _popupbtn;
};
