#pragma once

#include <cppgui/Window.hpp>
#include <cppgui/Root_widget.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>


class Test_window: public cppgui::Window
{
public:
    explicit Test_window();

protected:

    void create_ui() override;
    void adjust_ui_layout() override;

private:
    cppgui::fonts::Rasterized_font  _dflt_font;
    cppgui::Label                   _label;
    cppgui::Textbox                 _textbox;
};
