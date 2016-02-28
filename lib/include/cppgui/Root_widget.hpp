#pragma once

namespace cppgui {

    extern int dummy;

    template <class Renderer>
    class Root_widget {
    public:
        void render(Renderer &);
    };

} // ns cppgui