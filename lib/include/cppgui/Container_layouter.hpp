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

        A "dummy" layouter that does not do anything expect providing a dummy implementation
        of get_mimimal_size().
        Inject this container layouter when your container does not need any layouting (e.g.
        because you position and size elements yourself).
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

    /*  Single_line_layout

        This container layouter arranges contained elements on a single line. It allows setting 
        the following constraints on each element:

        - minimum space before [FUTURE] [NOTE: for first element, take box-model padding into account!]
        - minimum space after [FUTURE] [NOTE: sim. for last element]
        - vertical alignment [FUTURE]
        - minimum width
        - maximum width

        More constraints and indications may be added in the future.

     */

    template<class Config, bool With_layout> struct Single_line_layout;

    template<class Config> struct Single_line_layout<Config, false> { template<class Class, class Parent> struct Aspect: Parent {}; };

    template<class Config>
    struct Single_line_layout<Config, true>
    {
        template<class Class, class Parent>
        struct Aspect: Parent
        {
            struct Element_ref
            {
                Element_ref(Widget<Config, true> *widget_): _widget{widget_} {}

                Widget<Config, true> *_widget = nullptr;
                Width _min_length = 0;
                Width _max_length = 0;

                auto& set_min_length(Length w) { _min_length = w; return *this; }
                auto& set_max_length(Length w) { _max_length = w; return *this; }
            };

            std::vector<std::unique_ptr<Element_ref>>   _elements;
            Width                                       _spacing = 0;

            void set_spacing(Width);

            void add_element(Widget<Config, true> * widget);

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

            auto p() { return static_cast<Class*>(this); }
        };
    };

    /*  Single_column_layout
     */

    template<class Config, bool With_layout> struct Single_column_layout;

    template<class Config> struct Single_column_layout<Config, false> { template<class Class, class Parent> struct Aspect: Parent {}; };

    template<class Config>
    struct Single_column_layout<Config, true>
    {
        template<class Class, class Parent>
        struct Aspect: Parent
        {
            struct Element_ref
            {
                Element_ref(Widget<Config, true> *widget_): _widget{widget_} {}

                Widget<Config, true> *_widget = nullptr;
                Width _min_length = 0;
                Width _max_length = 0;

                auto& set_min_length(Width w) { _min_length = w; return *this; }
                auto& set_max_length(Width w) { _max_length = w; return *this; }
            };

            std::vector<std::unique_ptr<Element_ref>>   _elements;
            Length                                      _spacing = 0;

            void set_spacing(Length);

            void add_element(Widget<Config, true> * widget);

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

            auto p() { return static_cast<Class*>(this); }
        };
    };

} // ns cppgui