#pragma once

#include <cppgui/Root_widget.hpp>
#include <cppgui/Label.hpp>
#include <cppgui/Textbox.hpp>
#include <cppgui/Checkbox.hpp>
#include <cppgui/Button.hpp>
#include <cppgui/Glyph_button.hpp>
#include <cppgui/Stack.hpp>
#include <cppgui/Vertical_scrollbar.hpp>
#include <cppgui/Scrollbox.hpp>
#include <cppgui/Canvas.hpp>

#define CPPGUI_DEFINE_WIDGET_TYPES(Config, With_layout) \
    using Widget             = cppgui::Widget            <Config, With_layout>; \
    using Label              = cppgui::Label             <Config, With_layout>; \
    using Button             = cppgui::Button            <Config, With_layout>; \
    using Glyph_button       = cppgui::Glyph_button      <Config, With_layout>; \
    using Root_widget        = cppgui::Root_widget       <Config, With_layout>; \
    using Textbox            = cppgui::Textbox           <Config, With_layout>; \
    using Checkbox           = cppgui::Checkbox          <Config, With_layout>; \
    using Abstract_container = cppgui::Container         <Config, With_layout>; \
    using Container          = cppgui::Container         <Config, With_layout>; \
    using Stack              = cppgui::Stack             <Config, With_layout>; \
    using Vertical_scrollbar = cppgui::Vertical_scrollbar<Config, With_layout>; \
    using Scrollbox          = cppgui::Scrollbox         <Config, With_layout>; \
    //using Canvas_t = cppgui::Canvas<Renderer>; 

#define CPPGUI_INSTANTIATE_WIDGET_TYPES(Config, With_layout) \
    template cppgui::Widget                           <Config, With_layout>; \
    template cppgui::Widget__Layouter                 <Config, With_layout>; \
    template cppgui::Label                            <Config, With_layout>; \
    template cppgui::Label__Layouter                  <Config, With_layout>; \
    template cppgui::Button                           <Config, With_layout>; \
    template cppgui::Glyph_button                     <Config, With_layout>; \
    template cppgui::Glyph_button__Layouter           <Config, With_layout>; \
    template cppgui::Root_widget                      <Config, With_layout>; \
    template cppgui::Default__Root_widget__Updater    <Config, With_layout>; \
    template cppgui::Root_widget__Layouter            <Config, With_layout>; \
    template cppgui::Textbox                          <Config, With_layout>; \
    template cppgui::Textbox__Layouter                <Config, With_layout>; \
    template cppgui::Checkbox                         <Config, With_layout>; \
    template cppgui::Abstract_container               <Config, With_layout>; \
    template cppgui::Abstract_container__Layouter     <Config, With_layout>; \
    template cppgui::Container                        <Config, With_layout>; \
    template cppgui::Stack                            <Config, With_layout>; \
    template cppgui::Vertical_scrollbar               <Config, With_layout>; \
    template cppgui::Scrollbox                        <Config, With_layout>; 