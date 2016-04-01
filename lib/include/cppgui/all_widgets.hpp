#pragma once

#include <cppgui/Root_widget.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>
#include <cppgui/Checkbox.hpp>
#include <cppgui/Button.hpp>
#include <cppgui/Stack.hpp>
#include <cppgui/Canvas.hpp>

#define CPPGUI_DEFINE_WIDGET_TYPES(Config, With_layout) \
    using Widget        = cppgui::Widget     <Config, With_layout>; \
    using Label         = cppgui::Label      <Config, With_layout>; \
    using Button        = cppgui::Button     <Config, With_layout>; \
    using Root_widget   = cppgui::Root_widget<Config, With_layout>; \
    using Textbox       = cppgui::Textbox    <Config, With_layout>; \
    using Checkbox      = cppgui::Checkbox   <Config, With_layout>; \
    using Stack         = cppgui::Stack      <Config, With_layout>; \
    //using Canvas_t = cppgui::Canvas<Renderer>; 

