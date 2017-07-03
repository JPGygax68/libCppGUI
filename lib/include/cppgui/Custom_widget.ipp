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
    void Custom_widget<Config, With_layout>::on_mouse_motion(Mouse_motion_handler handler)
    {
        assert(!_on_mouse_motion);

        _on_mouse_motion = handler;
    }

    template <class Config, bool With_layout>
    void Custom_widget<Config, With_layout>::on_mouse_button(Mouse_button_handler handler)
    {
        _on_mouse_button = handler;
    }

    template <class Config, bool With_layout>
    void Custom_widget<Config, With_layout>::mouse_motion(const Point &pos)
    {
        if (_on_mouse_motion)
        {
            _on_mouse_motion(pos);
        }
    }

    template <class Config, bool With_layout>
    void Custom_widget<Config, With_layout>::mouse_button(const Point &pos, int button, Key_state state, Count clicks)
    {
        if (_on_mouse_button)
        {
            _on_mouse_button(pos, button, state, clicks);
        }
    }

    template <class Config, bool With_layout>
    void Custom_widget<Config, With_layout>::render(Canvas_t *, const Point & offset)
    {
        
    }

} // ns cppgui