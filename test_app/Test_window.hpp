#pragma once

#include <cppgui/Root_widget.hpp>
#include <cppgui/Label.hpp>

#include "./SDL_window.hpp"


class Test_window: public SDL_window
{
public:

    Test_window();

    auto root_widget() { return &_root_widget; }

protected:
    void size_changed(Sint32 data1, Sint32 data2) override;

private:
    cppgui::fonts::Rasterized_font  _dflt_font;
    cppgui::Root_widget             _root_widget;
    cppgui::Label                   _label;
};
