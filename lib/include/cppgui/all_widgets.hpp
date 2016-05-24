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

#include <cppgui/Box.hpp>
#include <cppgui/Root_widget.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>
#include <cppgui/Checkbox.hpp>
#include <cppgui/Button.hpp>
#include <cppgui/Glyph_button.hpp>
#include <cppgui/Vertical_scrollbar.hpp>
#include <cppgui/Abstract_container.hpp>
#include <cppgui/Container_base.hpp>
#include <cppgui/Container.hpp>
//#include <cppgui/List_pane.hpp>
#include <cppgui/Scrollbox.hpp>
#include <cppgui/Listbox.hpp>
#include <cppgui/Stringlist.hpp>
#include <cppgui/Canvas.hpp>

namespace cppgui {


    template<class Config, bool With_layout>
    struct All_widgets
    {
        using Widget             = Widget<Config, With_layout>;
        using Root_widget        = Root_widget<Config, With_layout>;
        using Label              = Label<Config, With_layout>;
        using Textbox            = Textbox<Config, With_layout>;
        using Button             = Button<Config, With_layout>;
        using Glyph_button       = Glyph_button<Config, With_layout>;
        using Checkbox           = Checkbox<Config, With_layout>;
        using Container          = Container<Config, With_layout>;
        using Vertical_scrollbar = Vertical_scrollbar<Config, With_layout>;
        using Listbox            = Listbox<Config, With_layout>;
        using Stringlist         = Stringlist<Config, With_layout>;
    };

} // ns cppgui

#define CPPGUI_DEFINE_WIDGET_TYPES(Config, With_layout) \
    using Widget                    = cppgui::Widget                    <Config, With_layout>; \
    using Label                     = cppgui::Label                     <Config, With_layout>; \
    using Button                    = cppgui::Button                    <Config, With_layout>; \
    using Glyph_button              = cppgui::Glyph_button              <Config, With_layout>; \
    using Root_widget               = cppgui::Root_widget               <Config, With_layout>; \
    using Textbox                   = cppgui::Textbox                   <Config, With_layout>; \
    using Checkbox                  = cppgui::Checkbox                  <Config, With_layout>; \
    using Abstract_container        = cppgui::Abstract_container        <Config, With_layout>; \
    using Container_base            = cppgui::Container_base            <Config, With_layout>; \
    using Container                 = cppgui::Container                 <Config, With_layout>; \
    using Custom_vertical_scrollbar = cppgui::Custom_vertical_scrollbar <Config, With_layout>; \
    using Vertical_scrollbar        = cppgui::Vertical_scrollbar        <Config, With_layout>; \
    using Listbox                   = cppgui::Listbox                   <Config, With_layout>; \
    using List_pane_base            = cppgui::List_pane_base            <Config, With_layout>; \
    using Stringlist                = cppgui::Stringlist                <Config, With_layout>;
//using Canvas_t = cppgui::Canvas<Renderer>; 

#define CPPGUI_INSTANTIATE_WIDGET_TYPES(Config, With_layout) \
    CPPGUI_INSTANTIATE_WIDGET            (Config, With_layout); \
    CPPGUI_INSTANTIATE_LABEL             (Config, With_layout); \
    CPPGUI_INSTANTIATE_BUTTON            (Config, With_layout); \
    CPPGUI_INSTANTIATE_GLYPH_BUTTON      (Config, With_layout); \
    CPPGUI_INSTANTIATE_ROOT_WIDGET       (Config, With_layout); \
    CPPGUI_INSTANTIATE_TEXTBOX           (Config, With_layout); \
    CPPGUI_INSTANTIATE_CHECKBOX          (Config, With_layout); \
    CPPGUI_INSTANTIATE_ABSTRACT_CONTAINER(Config, With_layout); \
    CPPGUI_INSTANTIATE_CONTAINER_BASE    (Config, With_layout); \
    CPPGUI_INSTANTIATE_CONTAINER         (Config, With_layout); \
    CPPGUI_INSTANTIATE_VERTICAL_SCROLLBAR(Config, With_layout); \
    CPPGUI_INSTANTIATE_LISTBOX           (Config, With_layout);
