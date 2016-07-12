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
#include "./Container.hpp"
#include "./Widget.hpp"

namespace cppgui
{
    template<class Config>
    struct Element_ref_base
    {
        using Widget_t = Widget<Config, true>;

        explicit Element_ref_base(Widget_t *widget_, float weight_ = 0): widget{widget_}, weight{weight_} {}

        auto& set_weight(float weight_) { weight = weight_; return *this; }

        Widget_t   *widget = nullptr;
        float       weight = 0;
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

    /** The following template generates a base class for Container_layouter implementations to
        inherit from.
        Its main purpose is to abstract away the difference between "compiled-in" and "detached"
        container layouters. (Since container layouters are aspects, I hope to later generalize 
        this abstraction and make it potentially available to all types of aspects.)
     */

    template<class Config, Aspect_injection Locality>
    struct Container_layouter_base
    {
        template<class Impl, class Parent>
        struct Aspect;
    };

    template<class Config>
    struct Container_layouter_base<Config, Aspect_injection::by_inheritance>
    {
        template<class Impl, class Parent>
        struct Aspect: Parent
        {
        protected:
            class Implementation_t: public Impl { friend class Aspect; };
            auto p() { return static_cast<Implementation_t*>(this); }
        };
    };

    template<class Config>
    struct Container_layouter_base<Config, Aspect_injection::detached>
    {
        template<class Impl, class Parent>
        struct Aspect: _container_base<Config>::template Layouter<Impl, Nil_struct>
        {
            explicit Aspect(Impl *main): _p{ static_cast<Implementation_t*>(p) } {}

        protected:
            class Implementation_t: public Impl { friend class Aspect; };
            auto p() { return _p; }

        private:
            Implementation_t *_p;
        };
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

        TODO: use new "accessors" to obtain implementation that works for both axes
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
        using Widget_t = Widget<Config, true>;

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
    using Single_row_flow_layout = Single_beam_flow_layout<Config, With_layout, Horizontal_geometry_accessor>;

    template<class Config, bool With_layout> 
    using Single_column_flow_layout = Single_beam_flow_layout<Config, With_layout, Vertical_geometry_accessor>;

} // ns cppgui