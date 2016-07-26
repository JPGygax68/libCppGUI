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

    /** A base class template for "detached" container layouters.
     */

    template<class Main>
    struct Detached_layouter_base
    {
        virtual ~Detached_layouter_base() {}

        class Main_t: public Main { friend struct Detached_layouter_base; };

        void set_main(Main *main) { _p  = static_cast<Main_t*>(main); }

        virtual void init_layout() {}
        virtual auto get_minimal_size() -> Extents { return {}; }
        virtual void layout() {}

    protected:
        auto p() { return _p; }

    private:
        Main_t *_p;
    };

    /** A "delegating" implementation of the Container_layouter concept.
     */
    struct Delegating_layouter
    {
        template<class Main, class Parent>
        struct Aspect: Parent
        {
            template<class Layouter> void create_layouter();

            void init_layout() override { layouter()->init_layout(); }
            auto get_minimal_size() -> Extents override { return layouter()->get_minimal_size(); }
            void layout() override { layouter()->layout(); }
            //void insert_child() ...
            //void drop_child() ...

        protected:
            //class Detached_layouter_base_t: public Detached_layouter_base<Main> { friend struct Aspect; };
            using Detached_layouter_base_t = Detached_layouter_base<Main>;
            auto layouter() { return _layouter.get(); }

        private:
            std::unique_ptr<Detached_layouter_base_t>   _layouter;
        };
    };

    /** This implementation of the Container_layouter concept is intended as a convenience
        base class for all non-delegating Container_layouter implementations.
     */
    template<class Impl, class Parent>
    struct Non_delegating_layouter_base: Parent
    {
        static void set_main(Impl *) {} // Do nothing, will obtain access via this pointer

        //void init_layout() override { p()->cont_init_layout(); }
        //auto get_minimal_size() -> Extents override { return p()->cont_get_minimal_size(); }
        //void layout() override { p()->cont_layout(); }

    protected:
        class Implementation_t: public Impl { friend struct Aspect; };
        auto p() { return static_cast<Implementation_t*>(this); }
    };

    /** Template that injects Detached_layouter_base<> into the aspect chain if the template
        parameter Injection is "detached", or Non_delegating_layouter_base<> if the Injection
        = "by_inheritance".
     */
    
    template <Aspect_injection Injection, class Main, class Parent> struct Container_layouter_injection_base;

    template <class Main, class Parent> 
    struct Container_layouter_injection_base<Aspect_injection::detached, Main, Parent>: Detached_layouter_base<Main> {};

    template <class Main, class Parent> 
    struct Container_layouter_injection_base<Aspect_injection::by_inheritance, Main, Parent>: Non_delegating_layouter_base<Main, Parent> {};

    /** Base class for common functionality.
     */
    template<class Main, class ElementRef, class Config, class Parent, Aspect_injection Injection>
    class Container_layouter_base: public Container_layouter_injection_base<Injection, Main, Parent>
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

    template<class Config, bool With_layout, Aspect_injection Injection> 
    struct Unordered_layouter;

    template<class Config, Aspect_injection Injection> 
    struct Unordered_layouter<Config, false, Injection> { template<class Class, class Parent> struct Aspect: Parent {}; };

    template<class Config, Aspect_injection Injection>
    struct Unordered_layouter<Config, true, Injection>
    {
        template<class Class, class Parent>
        struct Aspect: Container_layouter_injection_base<Injection, Class, Parent>
        {
            //void init_layout();
            auto get_minimal_size() -> Extents override { return {0, 0}; }
            //void layout();
        };
    };

    /*  Horizontal_box (IN DEVELOPMENT):

        A container layouter that divides space horizontally between fixed-width left and right
        parts (both optional), and a center part that is given the remaining space.

        TODO: use new "accessors" to obtain implementation that works for both axes
     */

    template<class Config, bool With_layout, Aspect_injection Injection = Aspect_injection::detached> 
    struct Horizontal_box;

    template<class Config, Aspect_injection Injection> 
    struct Horizontal_box<Config, false, Injection> { template<class Class, class Parent> struct Aspect: Parent {}; };

    template<class Config, Aspect_injection Injection>
    struct Horizontal_box<Config, true, Injection>
    {
        struct Element_ref
        {
            explicit Element_ref(Widget<Config, true> *widget_, const Fraction<Length> &size_ = {}): widget{widget_}, size{size_} {}

            Widget<Config, true>       *widget = nullptr;
            Fraction<Length>            size;
        };

        template<class Main, class Parent>
        struct Aspect: Container_layouter_base<Main, Element_ref, Config, Parent, Injection>
        {
            void set_left  (Widget<Config, true> *, const Fraction<Length> &width);
            void set_center(Widget<Config, true> *);
            void set_right (Widget<Config, true> *, const Fraction<Length> &width);

            std::unique_ptr<Element_ref>    _left;
            std::unique_ptr<Element_ref>    _center;
            std::unique_ptr<Element_ref>    _right;

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

        private:
            using Parent_t = Container_layouter_base;
            // auto p() { return static_cast<Class*>(this); }
        };
    };

    /*  Single_beam_flow_layout
     */

    template<class Config, bool With_layout, Orientation Orientation, Aspect_injection Injection> 
    struct Single_beam_flow_layout;

    template<class Config, Orientation Orientation, Aspect_injection Injection> 
    struct Single_beam_flow_layout<Config, false, Orientation, Injection> { template<class Class, class Parent> struct Aspect: Parent {}; };

    template<class Config, Orientation Orientation, Aspect_injection Injection>
    struct Single_beam_flow_layout<Config, true, Orientation, Injection>
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

        template<class Main, class Parent>
        struct Aspect: Container_layouter_base<Main, Element_ref, Config, Parent, Injection>
        {
            void set_spacing(Length);

            auto add_element(Widget<Config, true> * widget, float weight = 0) -> Main &;
            auto add_spacer(float weight = 1) -> Main &;

            void init_layout() override;
            auto get_minimal_size() -> Extents override;
            void layout() override;

        private:
            using Parent_t = Container_layouter_base;

            Length  _spacing = 0;
        };
    };

    // TODO: better name
    template<class Config, bool With_layout, Aspect_injection Injection = cppgui::Aspect_injection::by_inheritance> 
    using Single_row_flow_layout = Single_beam_flow_layout<Config, With_layout, left_to_right, Injection>;

    // TODO: better name
    template<class Config, bool With_layout, Aspect_injection Injection = cppgui::Aspect_injection::by_inheritance> 
    using Single_column_flow_layout = Single_beam_flow_layout<Config, With_layout, bottom_up, Injection>;

} // ns cppgui