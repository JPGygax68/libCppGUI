#pragma once

#include "./Container.hpp"

namespace cppgui {

    extern int dummy;

    template <class Renderer>
    class Root_widget {
    public:
        void render(Renderer *);

        void set_size(int w, int h);

    private:
        int _w, _h;
    };

} // ns cppgui