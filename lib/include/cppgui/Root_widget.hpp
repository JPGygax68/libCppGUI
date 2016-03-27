#pragma once

#include <functional>
#include <stack>

#include "./aspects.hpp"
#include "./Widget.hpp"
#include "./Container.hpp"

namespace cppgui {

    extern int dummy;

    template <class Config, bool With_layout> struct Root_widget_layouter {

        template <class Aspect_parent> struct Aspect {};
    };

    // Root widget

    template <class Config, bool With_layout>
    class Root_widget: 
        public Root_widget_layouter<Config, With_layout>::template Aspect< Config::template Root_widget_updater< Abstract_widget<Config, With_layout> > >,
        public Config::Font_mapper,
        public Config::template Root_widget_container_updater< Abstract_container<Config, With_layout> >
    {
    public:
        //using Renderer = typename Config::Renderer;
        using Keycode  = typename Config::Keyboard::Keycode;
        using Widget_t = typename Widget<Config, With_layout>;
        using Abstract_widget_t = typename Abstract_widget<Config, With_layout>;
        using Canvas_t = typename Abstract_widget_t::Canvas_t;
        //using Abstract_container_t = Abstract_container<Config, With_layout>;
        using Font_mapper = typename Config::Font_mapper;
        using Font_handle = typename Canvas_t::Font_handle;
        using Cursor_handle = typename Config::Cursor::Cursor_handle;

        Root_widget(Canvas_t *);

        //void add_child(Widget_t *);

        void init() override;

        // TODO: request mechanism ?
        void set_focus_to(Widget_t *);
        auto focused_widget() const { return _focused_widget; }

        void push_cursor(Cursor_handle);
        void pop_cursor();

        auto get_font_handle(const Rasterized_font *) -> Font_handle;

        void mouse_motion(const Position &) override;
        void mouse_button(const Position &, int button, Key_state) override;
        void mouse_click(const Position &, int button, int count) override;
        void mouse_wheel(const Position &) override;
        void text_input(const char32_t *, size_t) override;
        void key_down(const Keycode &) override;
        //void key_up(const Keycode &) override;

        void render(Canvas_t *, const Position &) override;

    private:
        Font_mapper                 _font_mapper;
        Canvas_t                   *_canvas = nullptr;
        Widget_t                   *_focused_widget = nullptr;
        std::stack<Cursor_handle>   _cursor_stack;
    };

    // Default implementation for Widget_updater aspect

    template <class Config, bool With_layout>
    struct Default_Root_widget_Updater {

        template <class Aspect_parent>
        struct Aspect : public Aspect_parent {
            using Abstract_container_t = Abstract_container<Config, With_layout>;
            using Invalidated_handler = std::function<void()>;
            using Root_widget_t = Root_widget<Config, With_layout>;

            void invalidate();

            auto root_widget() -> Root_widget_t * override { return static_cast<Root_widget_t*>(this); }
        };
    };

    // Default implementation for Container_Container_updater aspect

    template <class Config, bool With_layout>
    struct Default_Root_widget_Container_updater {

        template <class Aspect_parent> struct Aspect : public Aspect_parent {
            
            using Widget_t = Widget<Config, With_layout>;
            using Container_t = Container<Config, With_layout>;
            using Invalidated_handler = std::function<void()>;
            using Root_widget_t = Root_widget<Config, With_layout>;

            // Container_updater contract

            void child_invalidated(Widget_t *) override { _must_update = true; }

            auto container_root_widget() -> Root_widget_t * override { return static_cast<Root_widget_t*>(this); }

            // Specific functionality 

            void on_invalidated(Invalidated_handler handler) { _on_invalidated = handler; }

            void lock() { _must_update = false; }

            void unlock() { if (_on_invalidated && _must_update) _on_invalidated(); }

        private:
            Invalidated_handler _on_invalidated;
            bool                _must_update;
        };

    };

    // Layouting aspect

    template <class Config> struct Root_widget_layouter<Config, true> {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            using Root_widget_t = Root_widget<Config, true>;
            //using Abstract_container_t = typename Abstract_container<Config, true>;

            virtual void init_layout();
            virtual auto minimal_size() -> Extents { return {0, 0}; }
            virtual void layout();
        };
    };

} // ns cppgui