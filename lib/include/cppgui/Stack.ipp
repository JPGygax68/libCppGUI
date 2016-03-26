#include "./Stack.hpp"

namespace cppgui {

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    inline void Stack_layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        compute_sizes();
        this->layout();
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Stack_layouter<Config, true>::Aspect<Aspect_parent>::compute_sizes()
    {
        // ...
    }

    template <class Config>
    template <class Aspect_parent>
    inline auto Stack_layouter<Config, true>::Aspect<Aspect_parent>::minimal_size() -> Extents
    {
        // TODO
        return {};
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Stack_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
    }

} // ns cppgui