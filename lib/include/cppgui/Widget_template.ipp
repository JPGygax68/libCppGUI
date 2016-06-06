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

    template<class Config>
    template<class Class, bool With_layout>
    void _my_widget<Config>::Base<Class, With_layout>::init()
    {
        /** This is where the widget establishes its connection with the backends
            (available via root_widget()->canvas() etc. ).
         */
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _my_widget<Config>::Base<Class, With_layout>::compute_view_from_data()
    {
        /** This is another entry point that gets called recursively upon
            initialization. Its name says what it is intended for: to 
            set internal state so as to reflect the bound data.
         */
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _my_widget<Config>::Base<Class, With_layout>::render(Canvas_t *canvas, const Point & offset)
    {
        /** The render() method is responsible for rendering the widget via
            the canvas. 
            By convention, the widget's absolute position is determined by
            adding the offset parameter to the position() property.
         */

        // TODO: PLACEHOLDER
        this->fill_rect(canvas, this->rectangle(), offset, Canvas_t::rgba_to_native({0.8f, 0, 0.7f, 1}));
    }

    // Layouter aspect --------------------------------------------------------

    template<class Config>
    template<class Class, class Parent>
    void _my_widget<Config>::Layouter<Class, true, Parent>::init_layout()
    {
        /** The init_layout() method is called on the complete widget tree 
            before either get_minimal_size() or layout(). It is intended as 
            an occasion to compute measurements (typically of text strings) 
            that can then be used in both get_minimal_size() and layout().
         */
    }

    template<class Config>
    template<class Class, class Parent>
    auto _my_widget<Config>::Layouter<Class, true, Parent>::get_minimal_size() -> Extents
    {
        /** The get_minimal_size() method is intended to be called recursively
            by containers, or from a container's layout() method to help it
            decide how to place and size its children.
         */
    }

    template<class Config>
    template<class Class, class Parent>
    void _my_widget<Config>::Layouter<Class, true, Parent>::layout()
    {
        /** The layout() method is usually called once, recursively, on the 
            whole widget tree. This is where the Layouter aspect must
            position and size all contained elements.
         */

         /** For containers, you will usually want to call the parent layout() method:
         */
        Parent::layout();
    }

} // ns cppgui
