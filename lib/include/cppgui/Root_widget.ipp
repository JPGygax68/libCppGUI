#include <cppgui/Root_widget.hpp>

namespace cppgui {

    template<class Renderer>
    inline Root_widget<Renderer>::Root_widget()
    {
        set_position({ 0, 0 }); // TODO: this would not be necessary if Root_widget did not inherit from Container
    }

    /*
    template <class Renderer>
    inline void Root_widget<Renderer>::render(Renderer *r)
    {
        // TODO: the following is temporary - it must be made optional
        r->clear(r->rgba_to_native({ 0, 0.9f, 0.3f, 1 }));
    }
    */

} // ns cppgui