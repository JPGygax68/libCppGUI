#pragma once

#include "./Widget.hpp"
#include "./Box.hpp"
#include "./Container_base.hpp"

namespace cppgui {

    // Container base class: descended from Widget

    enum class Layout_type { none, 
        header_content, 
        content_footer, 
        head_content,           // horizontal: fixed-width head, followed by stretching content TODO: better name! 
        content_tail,           // opposite of head_content
        stack,
        end
    };

    // Forward-declarations

    template <class Config, bool With_layout, class Parent> struct Container__Layouter;

    // Main class

    /** This generic container class exists primarily to combine the functionalities of 
        Abstract_container with those of Widget. At the present (2016-05-03), it also offers 
        a choice of layouting algorithms, but that is bad design and will be removed.
     */
    template <class Config, bool With_layout>
    class Container: 
        public Container__Layouter<Config, With_layout,
            Container_base<Config, With_layout> >
    {
    public:

        void compute_view_from_data() override;

    protected:
        //auto paper_margin() -> unsigned int { return 2; } // TODO: remove (or move to Stack<>)
    };

    // Layouter aspect

    template <class Config, class Parent>
    struct Container__Layouter<Config, true, Parent>: 
        public Box__Layouter<Config, true, Parent >
    {
        using Widget_t = typename Widget<Config, true>;
        class Container_t: public Container<Config, true> { friend struct Container__Layouter; };

        // Layout contract

        void init_layout() override;
        auto get_minimal_size() -> Extents override;
        auto get_preferred_size() -> Extents override;
        void layout() override;

        // Specific interface

        void set_layout_type(Layout_type type ) { _layout_type = type; }

        void insert_child(Widget_t *); // TODO: find a better name OR support insertion index
        void drop_child(Widget_t *);

        void set_spacing(Length spacing) { _spacing = spacing; }

    private:
        auto p() { return static_cast<Container_t*>(this); }

        Layout_type     _layout_type = Layout_type::none;
        Length          _spacing = 0;
    };

    // Nil implementation of the Layouter aspect

    template <class Config, class Parent>
    struct Container__Layouter<Config, false, Parent> 
    {
        // void recalc_sizes() {}
    };
        
} // ns cppgui

#define CPPGUI_INSTANTIATE_CONTAINER(Config, With_layout) \
    template cppgui::Bordered_box       <Config, With_layout>; \
    template cppgui::Container          <Config, With_layout>;
