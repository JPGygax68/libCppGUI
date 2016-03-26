#pragma once

#include <cassert>
#include <type_traits>

#include <gpc/gui/renderer.hpp> // TODO: other source & namespace

#include "./basic_types.hpp"
#include "./aspects.hpp"

//#include "./Stylesheet.hpp"
#include "./Full_resource_mapper.hpp"

namespace cppgui {

    enum Key_state { pressed, released };

    template <class Config, bool With_layout> class Root_widget;

    /** Abstract_widget: functionality common to both Root_widget and Widget, i.e. not including the ability
        to function as an element in a container.
     */
    template <class Config, bool With_layout>
    class Abstract_widget:
        public Config::Color_mapper
        //public Config::Styler
        //public Config::Font_mapper
        //public Config::Cursor
    {
    public:
        using Keyboard          = typename Config::Keyboard;
        using Keycode           = typename Keyboard::Keycode;
        using Abstract_widget_t = Abstract_widget;
        using Root_widget_t     = Root_widget<Config, With_layout>;
        using Renderer          = typename Config::Renderer;
        using Native_color      = typename Renderer::native_color;
        using Font_handle       = typename Renderer::font_handle;
        using Click_handler     = std::function<void(const Position &, int button, int clicks)>; // TODO: support return value ?

        auto rectangle() const { return _rect; }
        auto position() const { return _rect.pos; }
        auto extents() const { return _rect.ext; }
        void set_position(const Position &);
        void set_extents(const Extents &);

        // TODO: color and other style definitions belong into stylesheets
        //static auto button_face_color        () { return Color{ 0.8f, 0.8f, 0.8f, 1 }; }
        //static auto button_face_hovered_color() { return Color{ 0.9f, 0.9f, 0.9f, 1 }; }

        virtual void init() {}

        virtual auto root_widget() -> Root_widget_t * = 0;

        // Input event injection

        /** By convention, mouse positions are passed to a widget as relative to
            their own origin (meaning that it falls to the caller, i.e. usually
            the container, to subtract the child widget's position() from the
            coordinates it gets from yet higher up).
         */
        virtual void mouse_motion(const Position &) {}
        virtual void mouse_button(const Position &, int /*button*/, Key_state) {}
        virtual void mouse_click(const Position &, int button, int count);
        virtual void text_input(const char32_t *, size_t) {}
        virtual void key_down(const Keycode &) {}
        //virtual void key_up(const Keycode &) {}

        virtual void mouse_enter() {}       // TODO: provide "entry point" parameter ?
        virtual void mouse_exit() {}        // TODO: provide "exit point" parameter ?
        virtual void gained_focus() {}
        virtual void loosing_focus() {}

        bool has_focus() { return root_widget()->focused_widget() == this; }
        bool disabled() const { return false; } // TODO!!!

        /** TODO: Having a virtual function to update render resources is imperfect
            in the sense that it is not always necessary to defer the mapping of render
            resources until pre-render time. In fact, in most cases
         */
        //virtual void update_render_resources(Renderer *) {}
        //void cleanup_render_resources(Renderer *);

        /** Convention: the provided position is an offset to be added to the widget's
            own coordinates.
         */
        virtual void render(Renderer *, const Position &offs) = 0;

    protected:
        auto rgba_to_native(Renderer *, const Color &) -> Native_color;
        void fill_rect(Renderer *r, const Rectangle &rect, const Native_color &);
        void fill_rect(Renderer *r, const Rectangle &rect, const Position &offs, const Native_color &);
        void fill_rect(Renderer *r, const Position &pos, const Extents &ext, const Native_color &);
        void fill(Renderer *r, const Position &offs, const Native_color &);
        void draw_borders(Renderer *, const Rectangle &rect, const Position &offs, 
            unsigned int width, const Color &top, const Color &right, const Color &bottom, const Color &left);
        auto convert_position_to_inner(const Position &) -> Position;
        auto advance_to_glyph_at(const Rasterized_font *, const std::u32string &text, size_t from, size_t to, Position &pos) 
            -> const Glyph_control_box *;

    private:
        Rectangle _rect = {};
    };

    // Layouting aspect

    template <bool With_layout> struct Widget_layouter {

        template <class Aspect_parent> struct Aspect {
            void init_layout() {}
        };
    };

    template <> struct Widget_layouter<true> {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            /** It is up to the implementation to guarantee that any internal state/data
                needed for layouting (including computing/returning the minimal_size())
                is kept up to date.
             */

            virtual void init_layout() = 0;
            virtual auto minimal_size() -> Extents = 0;
            virtual void layout() = 0;
        };
    };

    // Widget 

    template <class Config, bool With_layout>
    class Widget: 
        public Config::template Widget_updater< Abstract_widget<Config, With_layout> >,
        public Widget_layouter<With_layout>::template Aspect<Nil_struct>
    {
    public:
        using Renderer = typename Config::Renderer;
        using Font_handle = typename Renderer::font_handle;
        //using Abstract_widget_t = typename Abstract_widget<Config, With_layout, Widget_layouter, Config::Abstract_widget_updater>;
        using Click_handler = typename Abstract_widget::Click_handler;

        void on_click(Click_handler);

        // void init();

        // TODO: should the following be protected ?
        bool hovered() const { return _hovered; }

        void mouse_enter() override;
        void mouse_exit() override;

        void mouse_click(const Position &, int button, int count) override;

    private:
        Click_handler   _click_hndlr;
        bool            _hovered = false;
    };

    // Default implementations for Updating_aspect

    /** The "Updater" aspect of a widget is responsible for making sure it gets
        redrawn when invalidate() has been called.

        The default implementation uses a pointer to parent to pass up redraw
        requests until they reach the root widget, which "handles" the request
        by passing it along to callback function.

        Because there is chance that a different implementation may not need
        to follow that route (e.g. by calling render() directly), the pointer
        to container may not be needed, so it is a member of this aspect
        implementation rather than of Widget<> itself.

        TODO: THIS IS PROVISIONAL. If it turns out, during further development, 
        that a pointer to container is needed for reasons that are not dependent
        on the Updater (or any other) aspect family, that pointer, and the
        methods associated with it, should be moved to the Widget<> stem class.
     */
    template <class Config, bool With_layout> class Abstract_container;
    template <class Config, bool With_layout> class Default_container_updater;

    template <class Config, bool With_layout>
    struct Default_widget_updater {

        CPPGUI_DEFINE_ASPECT(Aspect)
        {
            using Widget_t = Widget<Config, With_layout>;
            using Abstract_container_t = Abstract_container<Config, With_layout>;
            using Root_widget_t = Root_widget<Config, With_layout>;

            void invalidate();
            void added_to_container(Abstract_container_t *);

            auto container() const { return _container; }

            auto root_widget() -> Root_widget_t * override { return _container->_root_widget(); }

        private:
            Abstract_container_t *_container;
        };
    };

} // ns cppgui