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

        using Mouse_motion_handler = std::function<void(const Point &)>;
        using Mouse_button_handler = std::function<void(const Point &, int button, Key_state, Count clicks)>;

        void on_mouse_motion(Mouse_motion_handler);
        void on_mouse_button(Mouse_button_handler);

        void mouse_motion(const Point &) override;
        void mouse_button(const Point &, int /*button*/, Key_state, Count clicks) override;

        void render(Canvas_t *, const Point &offset) override;

    private:
        Mouse_motion_handler        _on_mouse_motion;
        Mouse_button_handler        _on_mouse_button;
    };

} // ns cppgui
