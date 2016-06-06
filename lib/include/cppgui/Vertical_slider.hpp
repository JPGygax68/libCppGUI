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
    struct _vertical_slider
    {
        template<class Class, bool With_layout, class Parent> struct Layouter;

        // Main class declaration -----------------------------------

        #define _CPPGUI_INSTANTIATE_VERTICAL_SLIDER_BASE(Config, With_layout, ...) \
            template cppgui::_vertical_slider<Config>::Base<__VA_ARGS__, With_layout>; \
            template cppgui::_vertical_slider<Config>::Layouter<__VA_ARGS__, With_layout, \
                cppgui::Widget<Config, With_layout> >;

        /** The class is called "Base" because it is intended to be customized via template parameters.
         */
        template<class Class, bool With_layout>
        class Base: 
            public Layouter<Class, With_layout,     // Layouter aspect, parameterized with ...
                Widget<Config, With_layout> >       // ... the actual parent class: Widget<>
        {
        public:
            using Widget_t = Widget<Config, With_layout>;
            using Parent_t = Widget_t;
            using Canvas_t = typename Widget_t::Canvas_t;

            void init() override;

            void compute_view_from_data() override;

            void render(Canvas_t *, const Point &offset) override;

        protected:

            static constexpr auto slide_width() -> Width { return 10; }

            Rectangle           _slide_rect;
        };

        // Layouter aspect ----------------------------------------------

        /** This macro definition must mirror the layouter aspect class declaration.
            It should instantiate not just all specializations of the Layouter aspect
            itself (meaning both the empty one for With_layout = false and the real one
            wgere With_layout = true), but also any aspects it injects on  its own 
            (this example does not inject any, but see Label.hpp for an example).
         */
        #define _CPPGUI_INSTANTIATE_VERTICAL_SLIDER_LAYOUTER(Config, With_layout, ...) \
            template cppgui::_vertical_slider<Config>::Layouter<With_layout, __VA_ARGS__>; \
            template cppgui::Box__Layouter<Config, With_layout, __VA_ARGS__>;

        /** Dummy template specialization for when With_layout = false.
         */
        template<class Class, class Parent> struct Layouter<Class, false, Parent>: public Parent {};

        /** "Real" layouter specialization that will be selected when With_layout = true.
         */
        template<class Class, class Parent>
        struct Layouter<Class, true, Parent>: public Parent
        {
            void init_layout() override;

            auto get_minimal_size() -> Extents override;

            void layout() override;

        protected:
            class Vertical_slider_t: public Base<Class, true> { friend struct Layouter; };

            /** Gives access to the main class, including protected and private sections.
            */
            auto p() { return static_cast<Vertical_slider_t*>(this); }
        };

    }; // templated ns _vertical_slider

    // Export section -----------------------------------------------

    #define CPPGUI_INSTANTIATE_VERTICAL_SLIDER(Config, With_layout) \
        template cppgui::Vertical_slider<Config, With_layout>; \
        template cppgui::_vertical_slider<Config>; \
        _CPPGUI_INSTANTIATE_VERTICAL_SLIDER_BASE(Config, With_layout, cppgui::Vertical_slider<Config, With_layout>);

    template<class Config, bool With_layout>
    class Vertical_slider: public _vertical_slider<Config>::template Base<Vertical_slider<Config, With_layout>, With_layout> { };

} // ns cppgui
