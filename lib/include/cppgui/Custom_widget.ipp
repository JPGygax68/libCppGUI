#pragma once

#include "./Custom_widget.hpp"

#pragma warning(disable: 4505)

namespace cppgui
{

    template <class Config>
    template <class Class, class Parent>
    void _custom_widget<Config>::Layouter<Class, true, Parent>::init_layout()
    {
        // TODO
    }

    template <class Config>
    template <class Class, class Parent>
    auto _custom_widget<Config>::Layouter<Class, true, Parent>::get_minimal_size() -> Extents
    {
        return {}; // TODO
    }

    template <class Config>
    template <class Class, class Parent>
    void _custom_widget<Config>::Layouter<Class, true, Parent>::layout()
    {
        // TODO
    }

    template <class Config, bool With_layout>
    void Custom_widget<Config, With_layout>::render(Canvas_t *, const Point & offset)
    {
        // TODO
    }

} // ns cppgui