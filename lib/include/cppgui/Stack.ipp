#include "./Stack.hpp"

namespace cppgui {

    // Layouter aspect ----------------------------------------------

    /*
    template<class Config>
    template<class Aspect_parent>
    inline void Stack_layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        compute_sizes();
    }

    template <class Config>
    template <class Aspect_parent>
    inline void Stack_layouter<Config, true>::Aspect<Aspect_parent>::compute_sizes()
    {
        // ...
    }
    */

    template <class Config>
    template <class Aspect_parent>
    inline auto Stack_layouter<Config, true>::Aspect<Aspect_parent>::minimal_size() -> Extents
    {
        _min_exts = {};

        for (auto child: children())
        {
            auto ext = child->minimal_size();
            if (ext.w > _min_exts.w) _min_exts.w = ext.w;
            _min_exts.h += ext.h + 1;
        }

        _min_exts.h -= 1;

        return _min_exts;
    }

    template<class Config>
    template<class Aspect_parent>
    inline void Stack_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        int y = 0;

        for (auto child: children())
        {
            auto ext = child->minimal_size();
            child->set_extents({_min_exts.w, ext.h});
            child->set_position({0, y});
            child->layout();
            y += ext.h + 1;
        }
    }

} // ns cppgui