#pragma once

#include "./Container.hpp"

namespace cppgui {

    extern int dummy;

    template <class Config, bool With_layout>
    class Root_widget: 
        public Abstract_widget<Config, Nil_aspect, Config::Root_widget_updater>,
        public Config::template Root_widget_container_updater<Abstract_container<Config, With_layout>>
    {
    public:
        using Abstract_container_t = Abstract_container<Config, With_layout>;

        Root_widget();

        void add_child(Widget<Config, With_layout> *);

        void mouse_motion(const Position &) override;

        void render(Renderer *, const Position &) override;
    };

    // Default implementation for Widget_updater aspect

    template <class Config, bool With_layout>
    struct Default_root_widget_updater {

        template <class Next_aspects>
        struct Aspect : public Next_aspects {
            using Abstract_container_t = Abstract_container<Config, With_layout>;

            void invalidate();
            void added_to_container(Abstract_container_t *);
        };
    };

    // Default implementation for Container_updater aspect

    template <class Config, bool With_layout>
    struct Default_root_widget_container_updater {

        template <class Next_aspects>
        struct Aspect : public Next_aspects {
            using Widget_t = Widget<Config, With_layout>;
            using Container_t = Container<Config, With_layout>;

            void child_invalidated(Widget_t *) override;
        };
    };

} // ns cppgui