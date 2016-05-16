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

#include "./Widget_template.hpp"

namespace cppgui {

    // Main class -------------------------------------------------------------

    template<class Config, bool With_layout>
    inline void My_widget<Config, With_layout>::init()
    {
        /** This is where the widget establishes its connection with the backends
            (available via root_widget()->canvas() etc. ).
         */
    }

    template<class Config, bool With_layout>
    void My_widget<Config, With_layout>::compute_view_from_data()
    {
        /** This is another entry point that gets called recursively upon
            initialization. Its name says what it is intended for: to 
            set internal state so as to reflect the bound data.
         */
    }

    template<class Config, bool With_layout>
    void My_widget<Config, With_layout>::render(Canvas_t *, const Point & offset)
    {
        /** The render() method is responsible for rendering the widget via
            the canvas. 
            By convention, the widget's absolute position is determined by
            adding the offset parameter to the position() property.
         */
    }

    // Layouter aspect --------------------------------------------------------

    template<class Config, class Parent>
    inline void My_widget__Layouter<Config, true, Parent>::init_layout()
    {
        /** The init_layout() method is called on the complete widget tree 
            before either get_minimal_size() or layout(). It is intended as 
            an occasion to compute measurements (typically of text strings) 
            that can then be used in both get_minimal_size() and layout().
         */
    }

    template<class Config, class Parent>
    auto My_widget__Layouter<Config, true, Parent>::get_minimal_size() -> Extents
    {
        /** The get_minimal_size() method is intended to be called recursively
            by containers, or from a container's layout() method to help it
            decide how to place and size its children.
         */
    }

    template<class Config, class Parent>
    void My_widget__Layouter<Config, true, Parent>::layout()
    {
        /** The layout() method is usually called once, recursively, on the 
            whole widget tree. This is where the Layouter aspect must
            position and size all contained elements.
         */
    }

} // ns cppgui

/** This macro should be used when explicit instantiation is needed.
 */
#define CPPGUI_INSTANTIATE_MY_WIDGET(Config, With_layout) \
    template cppgui::My_widget<Config, With_layout>; \
    template cppgui::My_widget__Layouter<Config, With_layout, cppgui::My_widget<Config, With_layout>>;
