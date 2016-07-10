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
#include <cppgui/Slider.hpp>
#include <cppgui/Icon_resources.hpp>
#include <cppgui/Canvas.hpp>

namespace cppgui {

    template<class Config, bool With_layout, Box_model_definition BMDef>
    struct Default_widgets
    {
        using Widget_t             = Widget<Config, With_layout>;
        using Root_widget_t        = Root_widget_base<Config, With_layout>;
        using Label_t              = Label<Config, With_layout, BMDef>;
        using Textbox_t            = Textbox<Config, With_layout, BMDef>;
        using Button_t             = Button<Config, With_layout, BMDef>;
        using Glyph_button_t       = Glyph_button<Config, With_layout, BMDef>;
        using Checkbox_t           = Checkbox<Config, With_layout, BMDef>;
        //using Container_base_t     = Container_base<Config, With_layout>;
        //using Container_t          = Container<Config, With_layout, BMDef, TODO: layouter>;
        using Vertical_scrollbar_t = Vertical_scrollbar<Config, With_layout, BMDef>;
        using Listbox_t            = Listbox<Config, With_layout, BMDef>;
        using Stringlist_t         = Stringlist<Config, With_layout, BMDef>;
        //using Horizontal_slider_t  = Horizontal_slider<Config, With_layout>; // TODO
        using Vertical_slider_t    = Vertical_slider<Config, With_layout>;
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_FONT_RESOURCES(Size) \
    CPPGUI_INSTANTIATE_ICON_RESOURCES(Size)
