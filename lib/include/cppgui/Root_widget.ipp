#include <cppgui/Root_widget.hpp>

namespace cppgui {

    template <class Renderer>
    inline void Root_widget<Renderer>::render(Renderer *r)
    {
        // TODO: the following is temporary - it must be made optional
        r->clear(r->rgba_to_native({ 0, 0.9f, 0.3f, 1 }));
    }

    template<class Renderer>
    inline void Root_widget<Renderer>::set_size(int w, int h)
    {
        _w = w, _h = h;
    }

} // ns cppgui