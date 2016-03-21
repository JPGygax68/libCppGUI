#pragma once

#include <functional>

#include "./aspects.hpp"
#include "./Widget.hpp"
#include "./Container.hpp"

namespace cppgui {

    extern int dummy;

    // Layouting aspect for Root_widget

    template <class Config, bool With_layout> struct Root_widget_layouter {

        template <class Aspect_parent> struct Aspect {};
    };

    template <class Config> struct Root_widget_layouter<Config, true> {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            using Root_widget_t = Root_widget<Config, true>;
            //using Abstract_container_t = typename Abstract_container<Config, true>;

            virtual auto minimal_size() -> Extents { return {0, 0}; }
            virtual void layout();
        };
    };

    // Root widget

    template <class Config, bool With_layout>
    class Root_widget: 
        public Root_widget_layouter<Config, With_layout>::template Aspect< Config::template Root_widget_updater< Abstract_widget<Config, With_layout> > >,
        public Config::Font_mapper,
        public Config::template Root_widget_container_updater< Abstract_container<Config, With_layout> >
    {
    public:
        using Renderer = typename Config::Renderer;
        //using Abstract_widget_t = typename Abstract_widget<Config, With_layout, Nil_aspect, Nil_aspect>;
        //using Abstract_container_t = Abstract_container<Config, With_layout>;
        using Font_mapper = typename Config::Font_mapper;
        using Font_handle = typename Renderer::font_handle;

        Root_widget(Renderer *);

        void add_child(Widget<Config, With_layout> *);

        auto get_font_handle(const Rasterized_font *) -> Font_handle;

        void mouse_motion(const Position &) override;
        void mouse_click(const Position &, int button, int count) override;

        void render(Renderer *, const Position &) override;

    private:
        Font_mapper     _font_mapper;
        Renderer       *_renderer = nullptr;
    };

    // Default implementation for Widget_updater aspect

    template <class Config, bool With_layout>
    struct Default_root_widget_updater {

        template <class Aspect_parent>
        struct Aspect : public Aspect_parent {
            using Abstract_container_t = Abstract_container<Config, With_layout>;
            using Invalidated_handler = std::function<void()>;
            using Root_widget_t = Root_widget<Config, With_layout>;

            void invalidate();

            auto root_widget() -> Root_widget_t * override { return static_cast<Root_widget_t*>(this); }

        private:
            Invalidated_handler _on_invalidated;
        };
    };

    // Default implementation for Container_updater aspect

    template <class Config, bool With_layout>
    struct Default_root_widget_container_updater {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {
            
            using Widget_t = Widget<Config, With_layout>;
            using Container_t = Container<Config, With_layout>;
            using Invalidated_handler = std::function<void()>;
            using Root_widget_t = Root_widget<Config, With_layout>;

            void on_invalidated(Invalidated_handler handler) { _on_invalidated = handler; }

            void child_invalidated(Widget_t *) override {

                assert(_on_invalidated);
                _on_invalidated();
            }

            auto _root_widget() -> Root_widget_t * override { return static_cast<Root_widget_t*>(this); }

        private:
            Invalidated_handler _on_invalidated;
        };

    };

} // ns cppgui