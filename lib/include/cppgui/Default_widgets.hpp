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
#include <cppgui/Vertical_slider.hpp>
#include <cppgui/Icon_resources.hpp>
#include <cppgui/Canvas.hpp>

namespace cppgui {

    template<class Config, bool With_layout>
    struct Default_widgets
    {
        template<class Parent> using Default_fixed_padding_box_model = Fixed_padding_box_model<5, Parent>;
        template<class Parent> using Default_fixed_border_and_padding_box_model = Fixed_border_and_padding_box_model<1, 5, Parent>;

        using Widget_t             = Widget<Config, With_layout>;
        using Root_widget_t        = Root_widget_base<Config, With_layout>;
        using Label_t              = Label<Config, With_layout>;
        using Textbox_t            = Textbox<Config, With_layout>;
        using Button_t             = Button<Config, With_layout, Default_fixed_border_and_padding_box_model>;
        using Glyph_button_t       = Glyph_button<Config, With_layout, Default_fixed_border_and_padding_box_model>;
        using Checkbox_t           = Checkbox<Config, With_layout>;
        using Container_base_t     = Container_base<Config, With_layout>;
        //using Container_t          = Container<Config, With_layout, Default_fixed_border_and_padding_box_model>;
        using Vertical_scrollbar_t = Vertical_scrollbar<Config, With_layout>;
        using Listbox_t            = Listbox<Config, With_layout>;
        using Stringlist_t         = Stringlist<Config, With_layout, 2>;
        using Vertical_slider_t    = Vertical_slider<Config, With_layout>;
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_FONT_RESOURCES(Size) \
    CPPGUI_INSTANTIATE_ICON_RESOURCES(Size)
