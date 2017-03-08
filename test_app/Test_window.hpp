#pragma once

#include <cppgui/Window.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>
#include <cppgui/Button.hpp>


class Test_window: public cppgui::Window
{
public:
    explicit Test_window();

protected:

    void create_ui() override;
    void adjust_ui_layout() override;

private:
    cppgui::Label                   _label;
    cppgui::Textbox                 _textbox;
    cppgui::Button                  _button;
};
