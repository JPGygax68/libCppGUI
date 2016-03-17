#pragma once

#include "./Container.hpp"

namespace cppgui {

    extern int dummy;

    template <class Config, bool WithLayout>
    class Root_widget: public Abstract_widget<Config, WithLayout>,
        public Abstract_container<Config, WithLayout>
    {
    public:
        Root_widget();

        void add_child(Widget<Config, WithLayout> *);

        void mouse_motion(const Position &) override;

        void render(Renderer *);
    };

} // ns cppgui