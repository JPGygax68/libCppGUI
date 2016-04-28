#pragma once

#include "./Scrollbox.hpp"

namespace cppgui {

    // Forward declarations

    template<class Config, bool With_layout> struct Listbox__Layouter { template<class Aspect_parent> struct Aspect: Aspect_parent {}; };

    // Class definition

    template<class Config, bool With_layout>
    class Listbox: public Listbox__Layouter<Config, With_layout>::template Aspect< Scrollbox<Config, With_layout> >
    {
    public:
        using Container_t = Container<Config, With_layout>;
        //using Canvas_t = typename Canvas<typename Config::Renderer>;
        //using List_pane_t = List_pane<Config, With_layout>;
        using Layoutable_widget_t = Widget<Config, true>; // THIS IS SPECIAL - only layoutable widgets can be added at runtime

        Listbox();

        // TODO: a variant that does not require the item to be layoutable
        void add_item(Layoutable_widget_t *);

    protected:
        using Vertical_scrollbar_t = Vertical_scrollbar<Config, With_layout>;

        Container_t             _content_pane;
    };

    // Layouter aspect

    template<class Config>
    struct Listbox__Layouter<Config, true> {

        template<class Aspect_parent> struct Aspect: Aspect_parent {

            using Scrollbox_t = Scrollbox<Config, true>;

            struct Listbox_t: public Listbox<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<Listbox_t*>(this); }

            void layout() override;
        };
    };

} // ns cppgui