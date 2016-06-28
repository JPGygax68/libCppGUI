#pragma once

#include <memory>

#include "./basic_types.hpp"
#include "./Widget.hpp"

namespace cppgui
{
    /*  Like normal widget layouters, of which they are specializations, container layouters are "aspects", 
        i.e. class templates intended to be chained via inheritance.

        However, unlike widget layouters, they are not specific to a specific widget class and can be 
        injected into any widget ultimately derived from Container_base<>; also, they are "wrapped": the outer 
        template takes the configuration parameters (at minimum the "Config" and "With_layout" parameters, 
        but they can have more), and an inner, normalized one that always takes the same two parameters 
        "Class" and "Parent". "Parent" is used for chaining, like with all aspect templates, while "Class" 
        is the exact widget type that the container layouter will work upon.

        Just as the widget class templates
     */

    /*  Unordered_layouter:

        A "dummy" layouter that does not do anything.
     */

    template<class Config, bool With_layout> struct Unordered_layouter;

    template<class Config> struct Unordered_layouter<Config, false> { template<class Class, class Parent> struct Aspect: Parent {}; };

    template<class Config>
    struct Unordered_layouter<Config, true>
    {
        template<class Class, class Parent>
        struct Aspect: Parent
        {
            //virtual void init_layout();
            auto get_minimal_size() -> Extents override { return {0, 0}; }
            //virtual void layout();
        };
    };

    /*  Horizontal_box (IN DEVELOPMENT):

        A container layouter that divides space horizontally between fixed-width left and right
        parts (both optional), and a center part that is given the remaining space.
     */

    template<class Config, bool With_layout> struct Horizontal_box;

    template<class Config> struct Horizontal_box<Config, false> { template<class Class, class Parent> struct Aspect: Parent {}; };

    template<class Config>
    struct Horizontal_box<Config, true>
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
            auto get_minimal_size() -> Extents override;
            void layout() override;

            auto p() { return static_cast<Class*>(this); }
        };
    };

} // ns cppgui