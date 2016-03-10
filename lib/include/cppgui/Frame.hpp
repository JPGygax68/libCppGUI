#pragma once

#include <vector>

#include "./Widget.hpp"

namespace cppgui {

    extern int dummy;

    template <class Renderer>
    class Frame {
    public:
        using Widget = Widget<Renderer>;

        Frame();

        /** TODO: should the widget be mandatorily assigned in the ctor? This
                would mean that frames can only be created once the UI for
                the window the frame will interface with has been completely
                loaded or defined.
         */
        void set_widget(Widget *);

        void mouse_motion(const Position &);

        void render(Renderer *);

        // TODO: do we need non-inherited event injection methods ?

    private:
        Widget         *_widget = nullptr;
    };

} // ns cppgui