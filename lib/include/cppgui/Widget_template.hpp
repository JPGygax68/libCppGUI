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

    // Main class declaration ---------------------------------------

    template<class Config>
    struct _my_widget
    {
        template<bool With_layout, class Parent> struct Layouter;

        // Main class declaration -----------------------------------

        #define _CPPGUI_INSTANTIATE_MY_WIDGET_BASE(Config, With_layout, ...) \
            template cppgui::_my_widget<Config>::Base<__VA_ARGS__, With_layout>; \
            template cppgui::_my_widget<Config>::Layouter<__VA_ARGS__, With_layout, \
                cppgui::Widget<Config, With_layout> >;

        /** The class is called "Base" because it is intended to be customized via template parameters.
         */
        template<bool With_layout>
        class Base: 
            public Layouter<With_layout,            // Layouter aspect, parameterized with ...
                Widget<Config, With_layout> >       // ... the actual parent class: Widget<>
        {
        public:

            void init() override;

            void compute_view_from_data() override;

            void render(Canvas_t *, const Point &offset) override;
        };

        // Layouter aspect ----------------------------------------------

        /** This macro definition must mirror the layouter aspect class declaration.
            It should instantiate not just all specializations of the Layouter aspect
            itself (meaning both the empty one for With_layout = false and the real one
            wgere With_layout = true), but also any aspects it injects on  its own 
            (this example does not inject any, but see Label.hpp for an example).
         */
        #define _CPPGUI_INSTANTIATE_MY_WIDGET_LAYOUTER(Config, With_layout, ...) \
            template cppgui::_my_widget<Config>::Layouter<With_layout, __VA_ARGS__>; \
            template cppgui::Box__Layouter<Config, With_layout, __VA_ARGS__>;

        /** Dummy template specialization for when With_layout = false.
         */
        template<class Parent> struct Layouter<false, Parent>: public Parent {};

        /** "Real" layouter specialization that will be selected when With_layout = true.
         */
        template<class Parent>
        struct Layouter<true, Parent>: public 
            Box__Layouter<Config, With_layout,
                Parent >
        {
            void init_layout() override;

            auto get_minimal_size() -> Extents override;

            void layout() override;

        protected:
            class My_widget_t: public Base<true> { friend struct Layouter; };

            /** Gives access to the main class, including protected and private sections.
            */
            auto p() { return static_cast<My_widget_t*>(this); }
        };

    }; // templated ns _my_widget

    // Export section -----------------------------------------------

    /** RECIPE:

        This is where you export your widget class template into the cppgui namespace,
        under a meaningful name.

        An instantion macro must be provided for every templated widget type.
        Do not forget to add a call to this macro to the CPPGUI_INSTANTIATE_ALL_WIDGETS()
        macro in `all_widgets.hpp`.
     */

    #define CPPGUI_INSTANTIATE_MY_WIDGET(Config, With_layout) \
        template cppgui::My_widget<Config, With_layout>; \
        template cppgui::_my_widget<Config>; \
        _CPPGUI_INSTANTIATE_MY_WIDGET_BASE(Config, With_layout, cppgui::My_widget<Config, With_layout>);

    template<class Config, bool With_layout>
    class My_widget: public _my_widget<Config>::template Base<My_widget<Config, With_layout>, With_layout> { };

} // ns cppgui
