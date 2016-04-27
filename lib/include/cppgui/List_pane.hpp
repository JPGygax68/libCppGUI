#pragma once

#include "./Container.hpp"

namespace cppgui {

    // Forward declarations

    template<class Config, bool With_layout> struct List_pane__Layouter { template<class Aspect_parent> struct Aspect: Aspect_parent {}; };

    // Class definition

    template<class Config, bool With_layout>
    class List_pane: public List_pane__Layouter<Config, With_layout>::template Aspect< Container<Config, With_layout> >
    {
    public:
        using Container_t = Container<Config, With_layout>;
        using Canvas_t = typename Canvas<typename Config::Renderer>;

        //List_pane();

    protected:
    };

    // Layouter aspect

    template<class Config>
    struct List_pane__Layouter<Config, true> {

        template<class Aspect_parent> struct Aspect: Aspect_parent {

            struct Scrollbox_t: public List_pane<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<Scrollbox_t*>(this); }

            //void layout() override;
        };
    };

} // ns cppgui
