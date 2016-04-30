#pragma once

#include "./Scrollbox.hpp"

namespace cppgui {

    // Listbox ======================================================

    // Forward declarations

    template<class Config, bool With_layout> struct Listbox__Layouter { template<class Aspect_parent> struct Aspect: Aspect_parent {}; };

    template<class Config, bool With_layout> class List_pane;

    /** The Listbox 
     */
    template<class Config, bool With_layout>
    class Listbox: public Listbox__Layouter<Config, With_layout>::template Aspect< Scrollbox<Config, With_layout, List_pane<Config, With_layout>> >
    {
    public:
        using Container_t = Container<Config, With_layout>;
        //using Canvas_t = typename Canvas<typename Config::Renderer>;
        using List_pane_t = List_pane<Config, With_layout>;
        using Layoutable_widget_t = Widget<Config, true>; // THIS IS SPECIAL - only layoutable widgets can be added at runtime

        Listbox();

        // TODO: a variant that does not require the item to be layoutable
        void add_item(Layoutable_widget_t *);

        void init() override;

        void bring_item_into_view(int item_index);

    protected:
        using Vertical_scrollbar_t = Custom_vertical_scrollbar<Config, With_layout>;

        List_pane_t         _content_pane;
        unsigned int        _first_visible_item_index, _last_visible_item_index;
    };

    // Layouter aspect

    // TODO: is it really needed, or is the Scrollbox layouter sufficient ?

    template<class Config>
    struct Listbox__Layouter<Config, true> {

        template<class Aspect_parent> struct Aspect: Aspect_parent {

            using Scrollbox_t = Scrollbox<Config, true, List_pane<Config, true>>;

            struct Listbox_t: public Listbox<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<Listbox_t*>(this); }

            void layout() override;
        };
    };

    // List_pane ====================================================

    template <class Config, bool With_layout>
    struct List_pane__Layouter {
        template <class Aspect_parent> struct Aspect;
    };


    template<class Config, bool With_layout>
    class List_pane: public List_pane__Layouter<Config, With_layout>::template Aspect< Scrollable_pane<Config, With_layout> >
    {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Listbox_t = Listbox<Config, With_layout>;
        using Scrollable_pane_t = Scrollable_pane<Config, With_layout>;
        using Parent_class = Scrollable_pane<Config, With_layout>;

        void init() override;

        // Interface with Scrollbox container

        void scroll(Navigation_unit, Position initial_pos, Fraction<int> delta);

    protected:
        auto listbox() { return static_cast<Listbox_t*>(container()); }

        void compute_visible_item_range();
        bool child_fully_before_bottom(Widget_t *child);

        unsigned int _first_visible_item_index, _last_visible_item_index;
    };

    // Layouter aspect

    template<class Config>
    struct List_pane__Layouter<Config, true> {

        template<class Aspect_parent> struct Aspect: Aspect_parent {

            struct List_pane_t: public List_pane<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<List_pane_t*>(this); }

            auto get_minimal_size() -> Extents override;
            void layout() override;


        protected:
            auto content_rect() -> Rectangle;
        };
    };

} // ns cppgui