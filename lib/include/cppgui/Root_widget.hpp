#pragma once

#include <functional>

#include "./Container.hpp"

namespace cppgui {

    extern int dummy;

#define __aspect

    template <class Config, bool With_layout>
    class Root_widget: 
        public Abstract_widget<Config, Nil_aspect, Nil_aspect>, // Config::Root_widget_updater>,
        public __aspect Config::template Root_widget_container_updater<Abstract_container<Config, With_layout>>
    {
    public:
        using Abstract_container_t = Abstract_container<Config, With_layout>;

        Root_widget();

        void add_child(Widget<Config, With_layout> *);

        void mouse_motion(const Position &) override;
        void mouse_click(const Position &, int button, int count) override;

        void render(Renderer *, const Position &) override;
    };

    // Default implementation for Widget_updater aspect

#ifdef NOT_DEFINED
    template <class Config, bool With_layout>
    struct Default_root_widget_updater {

        template <class Aspect_parent>
        struct Aspect : public Aspect_parent {
            using Abstract_container_t = Abstract_container<Config, With_layout>;
            using Invalidated_handler = std::function<void()>;

            void invalidate();

        private:
            Invalidated_handler _on_invalidated;
        };
    };
#endif

    // Default implementation for Container_updater aspect

    template <class Config, bool With_layout>
    struct Default_root_widget_container_updater {

        CPPGUI_DEFINE_ASPECT(Aspect)
        {
            using Widget_t = Widget<Config, With_layout>;
            using Container_t = Container<Config, With_layout>;
            using Invalidated_handler = std::function<void()>;

            void on_invalidated(Invalidated_handler handler) { _on_invalidated = handler; }

            void child_invalidated(Widget_t *) override {

                assert(_on_invalidated);
                _on_invalidated();
            }

        private:
            Invalidated_handler _on_invalidated;
        };
    };

} // ns cppgui