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

        void init() override;

        void compute_view_from_data() override;

        void render(Canvas_t *, const Point &offset) override;
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
        void init_layout() override;

        auto get_minimal_size() -> Extents override;

        void layout() override;
    };

} // ns cppgui
