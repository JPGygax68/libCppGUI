#pragma once

#include "./Widget.hpp"
#include "./Vertical_scrollbar.hpp"

namespace cppgui {

    // Forward declarations

    template<class Config, bool With_layout> struct Scrollbox__Layouter { template<class Aspect_parent> struct Aspect: Aspect_parent {}; };

    // Class definition

    template<class Config, bool With_layout>
    class Scrollbox: public Scrollbox__Layouter<Config, With_layout>::template Aspect< Container<Config, With_layout> >
    {
    public:
        using Container_t = Container<Config, With_layout>;
        using Canvas_t = typename Canvas<typename Config::Renderer>;

        Scrollbox();

        void set_content(Container_t *);

        void render(Canvas_t *, const Point &offset) override;

    protected:
        using Vertical_scrollbar_t = Vertical_scrollbar<Config, With_layout>;

        Container              *_content = nullptr;
        Vertical_scrollbar_t    _vert_sbar;
    };

    // Layouter aspect

    template<class Config>
    struct Scrollbox__Layouter<Config, true> {

        template<class Aspect_parent> struct Aspect: Aspect_parent {

            struct Scrollbox_t: public Scrollbox<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<Scrollbox_t*>(this); }

            void layout() override;
        };
    };

} // ns cppgui