#pragma once

#include <memory>

#include "./basic_types.hpp"
#include "./Widget.hpp"

namespace cppgui
{
    /*
    template<class Config>
    struct Container_layouter
    {
        
    };
    */

    template<class Config>
    struct Horizontal_box
    {
        template<class Class, class Parent>
        struct Aspect: Parent
        {
            void set_left  (Widget<Config, true> *, const Fraction<Length> &width);
            void set_center(Widget<Config, true> *);
            void set_right (Widget<Config, true> *, const Fraction<Length> &width);

            struct Element_ref
            {
                Element_ref(Widget<Config, true> *widget_, const Fraction<Length> &size_ = 0): widget{widget_}, size{size_} {}

                Widget<Config, true>       *widget = nullptr;
                Fraction<Length>            size;
            };

            std::unique_ptr<Element_ref>    _left;
            std::unique_ptr<Element_ref>    _center;
            std::unique_ptr<Element_ref>    _right;

            void init_layout() override;
            void layout() override;

            auto p() { return static_cast<Class*>(this); }
        };
    };

} // ns cppgui