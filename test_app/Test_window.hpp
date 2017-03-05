#pragma once

#include <cppgui/Window.hpp>
#include <cppgui/Root_widget.hpp>
#include <cppgui/Label.hpp>


class Test_window: public cppgui::Window
{
public:

    Test_window();

    auto root_widget() { return &_root_widget; }

protected:
    void size_changed(int w, int h) override;

private:
    cppgui::fonts::Rasterized_font  _dflt_font;
    cppgui::Root_widget             _root_widget;
    cppgui::Label                   _label;
};
