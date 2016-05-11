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

        void update_scrollbar_position();

    protected:
        using Vertical_scrollbar_t = Custom_vertical_scrollbar<Config, With_layout>;

        List_pane_t         _content_pane;
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

            auto get_preferred_size() -> Extents override;
        };
    };

    // List_pane ====================================================

    template <class Config, bool With_layout> struct List_pane__Layouter { template <class Aspect_parent> struct Aspect; };


    template<class Config, bool With_layout>
    class List_pane: public List_pane__Layouter<Config, With_layout>::template Aspect< Scrollable_pane<Config, With_layout> >
    {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Listbox_t = Listbox<Config, With_layout>;
        using Scrollable_pane_t = Scrollable_pane<Config, With_layout>;
        using Parent_t = Scrollable_pane_t;
        using Parent_class = Scrollable_pane<Config, With_layout>;

        void init() override;

        void compute_view_from_data() override;

        // Interface with Scrollbox container

        void scroll(Navigation_unit, /* Position initial_pos, */ Fraction<int> delta);

    protected:
        auto listbox() { return static_cast<Listbox_t*>(container()); }

        void compute_visible_item_range();
        bool child_fully_after_top    (Widget_t *child, Position_delta offset = 0);
        bool child_fully_before_bottom(Widget_t *child, Position_delta offset = 0);
        auto first_visible_child() { return children()[_first_visible_item]; }
        auto last_visible_child() { return children()[_last_visible_item]; }
        void scroll_down(Count items = 1);
        void scroll_up  (Count items = 1);
        void scroll_by_items(int delta);
        void scroll_by_pages(int delta);
        auto visible_items() const { return _last_visible_item - _first_visible_item + 1;  }
        auto hidden_items() { return visible_items() < (int) children().size() ? (int) (children().size()) - visible_items() : 0; }

        Index _first_visible_item, _last_visible_item;
    };

    // Layouter aspect

    template<class Config>
    struct List_pane__Layouter<Config, true> {

        template<class Aspect_parent> struct Aspect: Aspect_parent {

            struct List_pane_t: public List_pane<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<List_pane_t*>(this); }

            auto get_minimal_size() -> Extents override;

            void compute_and_set_extents(const Extents &container_extents);

            void layout() override;
        };
    };

} // ns cppgui

#define CPPGUI_INSTANTIATE_LISTBOX(Config, With_layout) \
    template cppgui::Listbox  <Config, With_layout>; \
    template cppgui::List_pane<Config, With_layout>;
