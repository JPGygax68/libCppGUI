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

#include "./Scrollbox.hpp"
#include "./Box_model.hpp"

namespace cppgui {

    // Listbox ======================================================

    // Forward declarations

    template<class Config, bool With_layout>
    struct Listbox__Layouter
    {
        template<class Class, class Parent>
        struct Aspect: Parent {};
    };

    template<class Config, bool With_layout, Box_model_definition BMDef> class List_pane_base;

    /** The Listbox 
     */
    template<class Config, bool With_layout, Box_model_definition BMDef>
    class Listbox: public 
        Listbox__Layouter<Config, With_layout>::template Aspect< Listbox<Config, With_layout, BMDef>,
        Scrollbox<Config, With_layout, List_pane_base<Config, With_layout, BMDef>, BMDef> >
    {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Container_base_t = Container_base<Config, With_layout>;
        //using Canvas_t = typename Canvas<typename Config::Renderer>;
        using List_pane_t = List_pane_base<Config, With_layout, BMDef>;
        using Layoutable_widget_t = Widget<Config, true>; // THIS IS SPECIAL - only layoutable widgets can be added at runtime

        Listbox();

        void ensure_item_in_view(int item_index);

        // TODO: a variant that does not require the item to be layoutable
        void add_item(Layoutable_widget_t *);

        auto selected_item() -> Index;
        auto item_index(Widget_t *) -> Index;

        void update_scrollbar_position();

    protected:
        using Vertical_scrollbar_t = Custom_vertical_scrollbar<Config, With_layout, BMDef>; // NOTE: this may the wrong variant - use Standalone ?

        List_pane_t         _content_pane;
    };

    // Layouter aspect

    // TODO: is it really needed, or is the Scrollbox layouter sufficient ?

    template<class Config>
    struct Listbox__Layouter<Config, true>
    {
        template<class Class, class Parent>
        struct Aspect: Parent
        {
            //using Scrollbox_t = Scrollbox<Config, true, List_pane_base<Config, true, BMDef>, BMDef>;

            void layout() override;

            auto get_preferred_size() -> Extents;

        protected:
            struct Listbox_t: Class { friend struct Aspect; };
            auto p() { return static_cast<Listbox_t*>(this); }
        };
    };

    // List_pane ====================================================

    template <class Config, bool With_layout>
    struct List_pane__Layouter
    {
        template<class Class, class Parent>
        struct Aspect {};
    };

    template<class Config, bool With_layout, Box_model_definition BMDef>
    class List_pane_base: public 
        List_pane__Layouter<Config, With_layout>::template Aspect< List_pane_base<Config, With_layout, BMDef>, 
        Scrollable_pane<Config, With_layout> >
    {
    public:
        using Widget_t = Widget<Config, With_layout>;
        using Canvas_t = typename Widget_t::Canvas_t;
        using Listbox_t = Listbox<Config, With_layout, BMDef>;
        using Scrollable_pane_t = Scrollable_pane<Config, With_layout>;
        using Parent_t = Scrollable_pane_t;
        using Parent_class = Scrollable_pane<Config, With_layout>;

        void set_separator(const Separator &);

        void init() override;

        void compute_view_from_data() override;

        void render(Canvas_t *, const Point &offset) override;

        // Interface with Scrollbox container

        void scroll(Navigation_unit, Fraction<int> delta);

    protected:
        friend class Listbox_t;
        using Mapped_separator_t = Mapped_separator<typename Config::Renderer::native_color>;

        auto listbox() { return static_cast<Listbox_t*>(this->container()); }

        void compute_visible_item_range();
        bool child_fully_after_top    (Widget_t *child, Position_delta offset = 0);
        bool child_fully_before_bottom(Widget_t *child, Position_delta offset = 0);
        auto first_visible_child() { return this->children()[_first_visible_item]; }
        auto last_visible_child () { return this->children()[_last_visible_item]; }
        void scroll_down(Count items = 1);
        void scroll_up  (Count items = 1);
        void scroll_by_items(int delta);
        void scroll_by_pages(int delta);
        auto visible_items() const { return _last_visible_item - _first_visible_item + 1;  }
        auto hidden_items() { return visible_items() < (int) this->children().size() ? (int) (this->children().size()) - visible_items() : 0; }

        Mapped_separator_t  _separator {};

        Length              _vert_item_padding; // vertical padding to add to height of each item

        Index               _first_visible_item, _last_visible_item;
    };

    // List_pane Layouter aspect

    template<class Config>
    struct List_pane__Layouter<Config, true>
    {
        template<class Class, class Parent>
        struct Aspect: Parent
        {
            void set_item_padding(const Extents &);

            auto get_minimal_size() -> Extents override;

            void compute_and_set_extents(const Extents &container_extents);

            void layout() override;

        protected:
            struct List_pane_t: public Class { friend struct Aspect; };
            auto p() { return static_cast<List_pane_t*>(this); }

            Extents _item_padding {};
        };
    };

    // Concrete class 

    template<class Config, bool With_layout, Box_model_definition BMDef>
    class List_pane: public List_pane_base<Config, With_layout, BMDef>
    {
    };

} // ns cppgui
