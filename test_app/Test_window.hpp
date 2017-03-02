#pragma once

#include <vector>

#include <cppgui/Container.hpp>
#include <cppgui/Root_widget.hpp>
#include <cppgui/Label.hpp>

#include "./SDL_window.hpp"
#include <cppgui/Label.hpp>


class Test_window: public SDL_window
{
public:

    Test_window();

    auto root_widget() { return &_root_widget; }

private:
    cppgui::fonts::Rasterized_font  _dflt_font;
    cppgui::Root_widget             _root_widget;
    cppgui::Label                   _label;
};
