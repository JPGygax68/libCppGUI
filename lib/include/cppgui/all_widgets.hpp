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
    using Scrollbox                 = cppgui::Scrollbox                 <Config, With_layout>; /* Note: default Scrollbox */ \
    using Scrollable_pane           = cppgui::Scrollable_pane           <Config, With_layout>; \
    using Listbox                   = cppgui::Listbox                   <Config, With_layout>; \
    using List_pane                 = cppgui::List_pane                 <Config, With_layout>;
    //using Canvas_t = cppgui::Canvas<Renderer>; 

#define CPPGUI_INSTANTIATE_WIDGET_TYPES(Config, With_layout) \
    template cppgui::Widget                           <Config, With_layout>; \
    template cppgui::Widget__Layouter                 <Config, With_layout>; \
    template cppgui::Label                            <Config, With_layout>; \
    template cppgui::Box__Layouter                    <cppgui::Label<Config, With_layout>>; \
    template cppgui::Label__Layouter                  <Config, With_layout>; \
    template cppgui::Button                           <Config, With_layout>; \
    template cppgui::Button__Layouter                 <Config, With_layout>; \
    template cppgui::Glyph_button                     <Config, With_layout>; \
    template cppgui::Glyph_button__Layouter           <Config, With_layout>; \
    template cppgui::Root_widget                      <Config, With_layout>; \
    template cppgui::Default__Root_widget__Updater    <Config, With_layout>; \
    template cppgui::Root_widget__Layouter            <Config, With_layout>; \
    template cppgui::Textbox                          <Config, With_layout>; \
    template cppgui::Box__Layouter                    <cppgui::Textbox<Config, With_layout>>; \
    template cppgui::Textbox__Layouter                <Config, With_layout>; \
    template cppgui::Checkbox                         <Config, With_layout>; \
    template cppgui::Abstract_container               <Config, With_layout>; \
    template cppgui::Abstract_container__Layouter     <Config, With_layout>; \
    template cppgui::Container_base                   <Config, With_layout>; \
    template cppgui::Container                        <Config, With_layout>; \
    template cppgui::Container__Layouter              <Config, With_layout>; \
    template cppgui::Custom_vertical_scrollbar        <Config, With_layout>; \
    /* template cppgui::Scrollbox                        <Config, With_layout>; */ \
    /* template cppgui::Scrollable_pane                  <Config, With_layout>; */ \
    template cppgui::List_pane                        <Config, With_layout>; \
    template cppgui::Listbox                          <Config, With_layout>;
