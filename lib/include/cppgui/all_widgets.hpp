#pragma once

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
#include <cppgui/Canvas.hpp>

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
    using List_pane                 = cppgui::List_pane                 <Config, With_layout>;
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

/*
#define CPPGUI_EXTERN_WIDGET_INSTANTIATIONS(Config, With_layout) \
    extern CPPGUI_INSTANTIATE_WIDGET            (Config, With_layout); \
    extern CPPGUI_INSTANTIATE_LABEL             (Config, With_layout); \
    extern CPPGUI_INSTANTIATE_BUTTON            (Config, With_layout); \
    extern CPPGUI_INSTANTIATE_GLYPH_BUTTON      (Config, With_layout); \
    extern CPPGUI_INSTANTIATE_ROOT_WIDGET       (Config, With_layout); \
    extern CPPGUI_INSTANTIATE_TEXTBOX           (Config, With_layout); \
    extern CPPGUI_INSTANTIATE_CHECKBOX          (Config, With_layout); \
    extern CPPGUI_INSTANTIATE_ABSTRACT_CONTAINER(Config, With_layout); \
    extern CPPGUI_INSTANTIATE_CONTAINER_BASE    (Config, With_layout); \
    extern CPPGUI_INSTANTIATE_CONTAINER         (Config, With_layout); \
    extern CPPGUI_INSTANTIATE_VERTICAL_SCROLLBAR(Config, With_layout); \
    extern CPPGUI_INSTANTIATE_LISTBOX           (Config, With_layout);
*/