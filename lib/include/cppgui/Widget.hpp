#pragma once

#include <cassert>
#include <type_traits>
#include <initializer_list>

#include <gpc/gui/renderer.hpp> // TODO: other source & namespace

#include "./basic_types.hpp"
#include "./aspects.hpp"

//#include "./Stylesheet.hpp"
#include "./Full_resource_mapper.hpp"

namespace cppgui {

    enum Key_state { pressed, released };

    template <class Renderer> class Canvas;

    template <class Config, bool With_layout> class Root_widget;

    /** Abstract_widget: functionality common to both Root_widget and Widget, i.e. not including the ability
        to function as an element in a container.
     */
    template <class Config, bool With_layout>
    class Abstract_widget: public Config::Color_mapper
    {
    public:
        using Keyboard          = typename Config::Keyboard;
        using Keycode           = typename Keyboard::Keycode;
        using Abstract_widget_t = Abstract_widget;
        using Root_widget_t     = Root_widget<Config, With_layout>;
        using Canvas_t          = typename Canvas<typename Config::Renderer>;
        using Native_color      = typename Canvas_t::Native_color;
        using Font_handle       = typename Canvas_t::Font_handle;
        using Click_handler     = std::function<void(const Position &, int button, int clicks)>; // TODO: support return value ?

        auto& rectangle() const { return _rect; }
        auto& position() const { return _rect.pos; }
        auto& extents() const { return _rect.ext; }
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
        virtual void mouse_wheel(const Position &) {}
        virtual void text_input(const char32_t *, size_t) {}
        virtual void key_down(const Keycode &) {}
        //virtual void key_up(const Keycode &) {}

        virtual void mouse_enter() {}       // TODO: provide "entry point" parameter ?
        virtual void mouse_exit() {}        // TODO: provide "exit point" parameter ?
        virtual void gained_focus() {}
        virtual void loosing_focus() {}

        bool has_focus() { return root_widget()->focused_widget() == this; }
        bool disabled() const { return false; } // TODO!!!

        /** Convention: the provided position is an offset to be added to the widget's
            own coordinates.
         */
        virtual void render(Canvas_t *, const Position &offs) = 0;

    protected:
        auto rgba_to_native(Canvas_t *, const Color &) -> Native_color;
        void fill_rect(Canvas_t *, const Rectangle &rect, const Native_color &);
        void fill_rect(Canvas_t *, const Rectangle &rect, const Position &offs, const Native_color &);
        void fill_rect(Canvas_t *, const Position &pos, const Extents &ext, const Native_color &);
        void fill(Canvas_t *, const Position &offs, const Native_color &);
        void draw_borders(Canvas_t *, const Position &offs, Width width, const Color &color);
        void draw_borders(Canvas_t *, const Rectangle &rect, const Position &offs, Width width, const Color &color);
        void draw_borders(Canvas_t *, const Rectangle &rect, const Position &offs, 
            Width width, const Color &top, const Color &right, const Color &bottom, const Color &left);
        auto convert_position_to_inner(const Position &) -> Position;
        auto advance_to_glyph_at(const Rasterized_font *, const std::u32string &text, size_t from, size_t to, Position &pos) 
            -> const Glyph_control_box *;

        // Experimental & temporary: implement more sophisticated (and flexible!) styling
        // - May not / should not stay static; make const if possible

        static auto stroke_width() -> int { return 1; }
        static auto stroke_color() -> Color { return { 0, 0, 0, 1 }; }
        //static auto padding() -> int { return 5; }
        static auto paper_color() -> Color { return {1, 1, 1, 1}; }

    private:
        Rectangle _rect = {};
    };

    template <class Config, bool With_layout> struct Widget__Layouter {

        template <class Aspect_parent> struct Aspect {
            void init_layout() {}
        };
    };

    // Widget 

    template <class Config, bool With_layout>
    class Widget: 
        public Config::template Widget_updater< Abstract_widget<Config, With_layout> >,
        public Widget__Layouter<Config, With_layout>::template Aspect<Nil_struct>
    {
    public:
        using Renderer = typename Config::Renderer;
        using Font_handle = typename Renderer::font_handle;
        //using Abstract_widget_t = typename Abstract_widget<GUIConfig, With_layout, Widget__Layouter, GUIConfig::Abstract_widget_updater>;
        using Click_handler = typename Abstract_widget::Click_handler;

        void set_background_color(const Color &);
        auto background_color() const;

        void on_click(Click_handler);

        // void init();

        // TODO: should the following be protected ?
        bool hovered() const { return _hovered; }

        void mouse_enter() override;
        void mouse_exit() override;

        void mouse_click(const Position &, int button, int count) override;

    protected:
        // Static styles
        // TODO: move to "stylesheet"
        static auto default_dialog_background_color() -> Color { return {0.6f, 0.6f, 0.6f, 1}; }

        // Styling
        // TODO: move to new class Abstract_button<> ?
        auto button_face_color() -> Color;
        auto button_border_color() -> Color;
        auto button_border_width() -> int;

        Rectangle               _inner_rect;

    private:
        Color                   _bkgnd_clr = {0, 0, 0, 0};
        Click_handler           _click_hndlr;
        bool                    _hovered = false;
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
    template<class Config, bool With_layout> class Abstract_container;
    template<class Config, bool With_layout> class Default_container_updater;

    template<class Config, bool With_layout>
    struct Default__Widget__Updater {
        
        template<class Aspect_parent> struct Aspect: public Aspect_parent
        {
            using Widget_t = Widget<Config, With_layout>;
            using Abstract_container_t = Abstract_container<Config, With_layout>;
            using Root_widget_t = Root_widget<Config, With_layout>;

            void invalidate();
            void added_to_container(Abstract_container_t *);
            void removed_from_container(Abstract_container_t *);

            auto container() const { return _container; }

            auto root_widget() -> Root_widget_t * override { return _container->container_root_widget(); }

        private:
            Abstract_container_t *_container;
        };
    };

    // Layouting aspect

    template <class Config> struct Widget__Layouter<Config, true> {

        template <class Aspect_parent> struct Aspect: public Aspect_parent {

            /** It is up to the implementation to guarantee that any internal state/data
            needed for layouting (including computing/returning the get_minimal_size())
            is kept up to date.
            */

            // Layouter aspect contract

            virtual void init_layout() = 0;
            virtual auto get_minimal_size() -> Extents = 0; // TODO: evolve to get_minimal_bounding_box()
                                                            // TODO: replace with non-virtual property accessor ?
            virtual void layout() = 0;

            void set_padding(Width);
            void set_padding(const std::initializer_list<Width> &);

            void set_rectangle(const Position &nw, const Position &se);
            void set_rectangle_nw(const Position &, const Extents &);
            void set_rectangle_ne(const Position &, const Extents &);
            void set_rectangle_se(const Position &, const Extents &);
            void set_rectangle_sw(const Position &, const Extents &);

        protected:
            using Padding = std::array<Width, 4>; // TODO: move to basic_types.hpp

            class Widget_t: public Widget<Config, true> { friend struct Aspect; };
            auto p() { return static_cast<Widget_t*>(this); }

            static constexpr auto button_padding() -> Padding { return { 5, 5, 5, 5 }; }

            void compute_inner_rect();

            Padding                 _padding = {};  // TODO: provide accessor ?
        };
    };

} // ns cppgui