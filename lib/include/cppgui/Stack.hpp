#pragma once

#include "./Container.hpp"

namespace cppgui {

    // Forward-declare layouter aspect

    template <class Config, bool With_layout>
    struct Stack_layouter {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {};
    };

    // Stack container widget

    template<class Config, bool With_layout>
    class Stack: public Stack_layouter<Config, With_layout>::template Aspect< Container<Config, With_layout> >
    {
    public:
        void mouse_wheel(const Position &dist);
    };

    // Layouter aspect

    template <class Config>
    struct Stack_layouter<Config, true> {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {

            class Stack_t: public Stack<Config, true> { friend struct Aspect; };

            auto p() { return static_cast<Stack_t*>(this); }

            //void compute_sizes();

            //void init_layout() override;
            auto minimal_size() -> Extents override;
            void layout() override;

        private:
            Extents _min_exts;
        };
    };

} // ns cppgui