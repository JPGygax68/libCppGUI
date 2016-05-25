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

    /** This macro definition must mirror the template class declaration that immediately follows.
     */
    #define CPPGUI_INSTANTIATE_MY_WIDGET(Config, With_layout) \
        template cppgui::My_widget<Config, With_layout>; \
        _CPPGUI_INSTANTIATE_MY_WIDGET_LAYOUTER(Config, With_layout, cppgui::Widget<Config, With_layout>)

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

    /** As for the main class, this macro definition must mirror the layouter aspect class declaration.
        It should instantiate not just the Layouter aspect itself, but also any aspects it injects on
        its own (this example does not inject any, but see Label.hpp for an example).
     */
    #define _CPPGUI_INSTANTIATE_MY_WIDGET_LAYOUTER(Config, With_layout, ...) \
        template cppgui::My_widget__Layouter<Config, With_layout, __VA_ARGS__>; \
        template cppgui::Box__Layouter<Config, With_layout, __VA_ARGS__>;

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

    protected:
        class My_widget_t: public My_widget<Config, true> { friend struct Layouter; };

        /** Gives access to the main class, including protected and private sections.
        */
        auto p() { return static_cast<My_widget_t*>(this); }
    };

} // ns cppgui
