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
    class _Container_layouter_base: public Parent
    {
    protected:

        void _add_element(ElementRef *);

        const auto& elements() const { return _elements; }

        auto sum_of_weights() -> float;

    private:
        std::vector<std::unique_ptr<ElementRef>> _elements;
    };

    /** A base class template for "detached" container layouters.
     */

    template<class Main>
    struct Detached_layouter_base
    {
        class Main_t: public Main { friend struct Detached_layouter_base; };

        void set_main(Main_t *main) { _p  = static_cast<Main_t*>(p); }

    protected:
        auto p() { return _p; }

    private:
        Main_t *_p;
    };

    /** A "delegating" implementation of the Container_layouter concept.
     */
    struct Delegating_layouter
    {
        template<class Impl, class Parent>
        struct Aspect: Parent
        {
            template<class Layouter> void create_layouter();

            void init_layout() override { layouter()->layouter_init_layout(); }
            auto get_minimal_size() -> Extents override { return layouter()->layouter_get_minimal_size(); }
            void layout() override { layouter()->layouter_layout(); }

            // TODO: insert_child() + drop_child() ?

        protected:
            class Detached_layouter_base_t: public Detached_layouter_base<Impl> { friend struct Aspect; };
            auto layouter() { return _layouter; }

        private:
            std::unique_ptr<Detached_layouter_base_t>   _layouter;
        };
    };

    template<class Impl, class Parent>
    struct Non_delegating_layouter_base: Parent
    {
        static void set_main(Impl *) {} // Do nothing, will obtain access via this pointer

        /** Note: in contrast to Delegating_layouter
         */
        void init_layout() override { p()->layouter_init_layout(); }
        auto get_minimal_size() -> Extents override { return p()->layouter_get_minimal_size(); }
        void layout() override { p()->layouter_layout(); }

    protected:
        class Implementation_t: public Impl { friend struct Aspect; };
        auto p() { return static_cast<Implementation_t*>(this); }
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
        struct Aspect: Non_delegating_layouter_base<Class, Parent>
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
        struct Aspect: public _Container_layouter_base<Aspect<Class, Parent>, Element_ref, Config, Parent>, Accessor
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