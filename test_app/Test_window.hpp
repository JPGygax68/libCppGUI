#pragma once

#include <cppgui/Window.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>
#include <cppgui/Button.hpp>
#include <cppgui/Checkbox.hpp>
#include <cppgui/Glyph_button.hpp>
#include <cppgui/Vertical_scrollbar.hpp>
#include <cppgui/Listbox.hpp>
#include <cppgui/Grid_container.hpp>
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
    using Widget = cppgui::Widget;
    using Test_button = cppgui::Boxed<cppgui::Glyph_button_base, cppgui::Borderless_button_box_styles>;

    std::vector<Test_button>            _button_list;

    cppgui::Label                       _label;
    cppgui::Textbox                     _textbox;
    cppgui::Button                      _button;
    cppgui::Checkbox                    _checkbox;
    cppgui::Glyph_button                _glyphbutton;
    cppgui::Boxed_vertical_scrollbar    _vscrollbar;
    cppgui::Listbox                     _listbox;
       
    cppgui::Button                      _align_left_ref    , _align_left_tgt;
    cppgui::Button                      _align_origin_ref  , _align_origin_tgt;
    cppgui::Button                      _align_right_ref   , _align_right_tgt;
    cppgui::Button                      _align_center_ref  , _align_center_tgt;
    cppgui::Button                      _align_top_ref     , _align_top_tgt;
    cppgui::Button                      _align_baseline_ref, _align_baseline_tgt;
    cppgui::Button                      _align_bottom_ref  , _align_bottom_tgt;
    cppgui::Button                      _align_middle_ref  , _align_middle_tgt;

    cppgui::Grid_container              _grid;
    cppgui::Widget_bag                  _grid_widgets;
};
