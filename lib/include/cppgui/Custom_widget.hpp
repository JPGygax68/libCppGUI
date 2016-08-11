#pragma once

#include "./Widget.hpp"

namespace cppgui
{

    template<class Config>
    struct _custom_widget
    {
        
        template<class Class, bool With_layout, class Parent> struct Layouter: Parent {};

        template<class Class, class Parent>
        struct Layouter<Class, true, Parent>: Parent
        {
            void init_layout() override;
            auto get_minimal_size  () -> Extents override;
            void layout() override;
        };

    }; // templated ns _custom_widget

    template<class Config, bool With_layout>
    class Custom_widget: public 
        _custom_widget<Config>::template Layouter<Custom_widget<Config, With_layout>, With_layout, 
        Widget<Config, With_layout> >
    {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;

        void render(Canvas_t *, const Point &offset) override;

    };

} // ns cppgui
