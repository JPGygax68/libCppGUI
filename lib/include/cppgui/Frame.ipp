#include <cppgui/Frame.hpp>

namespace cppgui {

    template<class Renderer>
    inline Frame<Renderer>::Frame()
    {
    }

    template<class Renderer>
    void Frame<Renderer>::set_widget(Widget *widget)
    {
        assert(!_widget);
        _widget = widget;
    }

    template<class Renderer>
    void Frame<Renderer>::mouse_motion(const Position &pos)
    {
        assert(_widget);
        _widget->mouse_motion(pos);
    }

    template<class Renderer>
    void Frame<Renderer>::render(Renderer *r)
    {
        assert(_widget);
        _widget->render(r, { 0, 0 });
    }

    /*
    template <class Renderer>
    inline void Frame<Renderer>::render(Renderer *r)
    {
        // TODO: the following is temporary - it must be made optional
        r->clear(r->rgba_to_native({ 0, 0.9f, 0.3f, 1 }));
    }
    */

} // ns cppgui