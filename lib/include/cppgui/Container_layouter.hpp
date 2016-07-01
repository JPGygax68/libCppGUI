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

    template<class Config>
    struct Element_ref_base
    {
        using Widget_t = Widget<Config, true>;

        explicit Element_ref_base(Widget_t *widget_, float weight_ = 0): widget{widget_}, weight{weight_} {}

        auto& set_weight(float weight_) { weight = weight_; return *this; }

        Widget_t   *widget = nullptr;
        float       weight = 0;
    };

    struct Horizontal_element_accessor
    {
        static auto forward_position(Point &point) -> Position & { return point.x; }
        static auto sideways_position(Point &point) -> Position & { return point.y; }
        static auto forward_length(Extents &ext) -> Length & { return ext.w; }
        static auto sideways_width(Extents &ext) -> Length & { return ext.h; }
    };

    struct Vertical_element_accessor
    {
        static auto forward_position(Point &point) -> Position & { return point.y; }
        static auto sideways_position(Point &point) -> Position & { return point.x; }
        static auto forward_length(Extents &ext) -> Length & { return ext.h; }
        static auto sideways_width(Extents &ext) -> Length & { return ext.w; }
    };

    /** Base class for common functionality.
     */
    template<class Class, class ElementRef, class Config, class Parent>
    class Container_layouter_base: public Parent
    {
    protected:

        void _add_element(ElementRef *);

        const auto& elements() const { return _elements; }

        auto sum_of_weights() -> float;

    private:
        std::vector<std::unique_ptr<ElementRef>> _elements;
    };

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
                Element_ref(Widget<Config, true> *widget_, const Fraction<Length> &size_ = {}): widget{widget_}, size{size_} {}

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

    /*  Single_beam_flow_layout
     */

    template<class Config, bool With_layout, class Accessor> struct Single_beam_flow_layout;

    template<class Config, class Accessor> 
    struct Single_beam_flow_layout<Config, false, Accessor> { template<class Class, class Parent> struct Aspect: Parent {}; };

    template<class Config, class Accessor>
    struct Single_beam_flow_layout<Config, true, Accessor>
    {
        struct Element_ref: Element_ref_base<Config>
        {
            using Element_ref_base<Config>::Element_ref_base;
            
            Element_ref() = delete;

            Length _min_length = 0;
            //Length _max_length = 0;

            auto& set_min_length(Width w) { _min_length = w; return *this; }
            //auto& set_max_length(Width w) { _max_length = w; return *this; }
        };

        template<class Class, class Parent>
        struct Aspect: public Container_layouter_base<Aspect<Class, Parent>, Element_ref, Config, Parent>, Accessor
        {
            void set_spacing(Length);

            auto add_element(Widget<Config, true> * widget, float weight = 0) -> Class &;
            auto add_spacer(float weight = 1) -> Class &;

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

            auto p() { return static_cast<Class*>(this); }

            Length  _spacing = 0;
        };
    };

    template<class Config, bool With_layout> 
    using Single_row_flow_layout = Single_beam_flow_layout<Config, With_layout, Horizontal_element_accessor>;

    template<class Config, bool With_layout> 
    using Single_column_flow_layout = Single_beam_flow_layout<Config, With_layout, Vertical_element_accessor>;

} // ns cppgui