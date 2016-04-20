#pragma once

#include "./Container.hpp"
#include "./Glyph_button.hpp"

namespace cppgui {

    // Forward declarations

    template <class Config, bool With_layout>
    struct Vertical_scrollbar__Layouter {
        template <class Aspect_parent> struct Aspect;
    };

    // Class declaration

    template<class Config, bool With_layout>
    class Vertical_scrollbar: public Vertical_scrollbar__Layouter<Config, With_layout>::template Aspect< Container<Config, With_layout> >
    {
    public:
        Vertical_scrollbar();

    protected:
        using Glyph_button_t = typename Glyph_button<Config, With_layout>;

        Glyph_button_t          _up_btn, _down_btn;
    };

    // Layouter aspect

    template <class Config>
    struct Vertical_scrollbar__Layouter<Config, true> {
        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            // Layouter contract

            //void init_layout() override;
            //auto get_minimal_size() -> Extents override;
            void layout() override;

            // Extra capabilities coming with layouting
            // TODO

        protected:
            class Vertical_scrollbar_t: public Vertical_scrollbar<Config, true> { friend struct Aspect; };

            auto p() { return static_cast<Vertical_scrollbar_t*>(this); }
        };
    };

} // ns cppgui

