#pragma once

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
