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

#include "./Widget.hpp"

namespace cppgui {

    // Forward declarations

    template<class Config, bool With_layout, class Parent> struct My_widget__Layouter;

    // Main class declaration ---------------------------------------

    template<class Config, bool With_layout>
    class My_widget: 
        public My_widget__Layouter<Config, With_layout, // Layouter aspect, parameterized with ...
            Widget<Config, With_layout> >               // ... the actual parent class: Widget<>
    {
    public:

        void init() override
        {
            /** This is where the widget establishes its connection with the backends
                (available via root_widget()->canvas() etc. ).
             */
        }

        void compute_view_from_data() override
        {
            /** This is another entry point that gets called recursively upon
                initialization. Its name says what it is intended for: to 
                set internal state so as to reflect the bound data.
            */
        }

        void render(Canvas_t *canvas, const Point &offset) override
        {
            /** The render() method is responsible for rendering the widget via
                the canvas. 
                By convention, the widget's absolute position is determined by
                adding the offset parameter to the position() property.
            */
        }
    };

    // Layouter aspect ----------------------------------------------

    /** Dummy template specialization for when With_layout = false.
     */
    template<class Config, class Parent> struct My_widget__Layouter<Config, false, Parent>: public Parent {};

    /** "Real" layouter specialization that will be selected when With_layout = true.
     */
    template<class Config, class Parent>
    struct My_widget__Layouter<Config, true, Parent>: public Parent
    {
        void init_layout() override
        {
            /** The init_layout() method is called on the complete widget tree 
                before either get_minimal_size() or layout(). It is intended as 
                an occasion to compute measurements (typically of text strings) 
                that can then be used in both get_minimal_size() and layout().
            */
        }

        auto get_minimal_size() -> Extents override
        {
            /** The get_minimal_size() method is intended to be called recursively
                by containers, or from a container's layout() method to help it
                decide how to place and size its children.
            */
        }

        void layout() override;
    };

} // ns cppgui

  /** This macro should be used when explicit instantiation is needed.
  */
#define CPPGUI_INSTANTIATE_MY_WIDGET(Config, With_layout) \
    template cppgui::My_widget<Config, With_layout>; \
    template cppgui::My_widget__Layouter<Config, With_layout, cppgui::My_widget<Config, With_layout>>;
