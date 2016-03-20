#pragma once

#include <cassert>
#include <type_traits>

#include <gpc/fonts/rasterized_font.hpp>
#include <gpc/gui/renderer.hpp> // TODO: other source & namespace

#include "./aspects.hpp"

#include "./Resource_mapper.hpp"

namespace cppgui {

    using Rgba_norm       = gpc::gui::rgba_norm;
    using Rasterized_font = gpc::fonts::rasterized_font;

    struct Position {
        int x, y;
        auto operator + (const Position &offs) const { return Position{ x + offs.x, y + offs.y }; }
        auto operator - (const Position &offs) const { return Position{ x - offs.x, y - offs.y }; }
    };

    struct Extents {
        unsigned int w, h;
        bool contains(const Position &pos) const { 
            return pos.x >= 0 && pos.y >= 0 
                && (unsigned int) pos.x < w && (unsigned int) pos.y < h; 
        }
    };

    struct Rectangle {
        Position pos;
        Extents  ext;
        bool contains(const Position &p) const
        { 
            return ext.contains(p - pos);
        }
    };

    // Default font mapper

    template <class Renderer>
    struct Default_font_mapper : public Resource_mapper<
        Default_font_mapper<Renderer>,
        Renderer,
        const Rasterized_font *, typename Renderer::font_handle,
        Renderer::font_handles_are_resources
    > {
        using Font_handle = typename Renderer::font_handle;

        auto obtain(Renderer *r, const Rasterized_font *font) {
            return r->register_font(*font);
        }
        void release(Renderer *r, Font_handle hnd) {
            r->release_font(hnd);
        }
    };

    /** Abstract_widget: functionality common to both Root_widget and Widget, i.e. not including the ability
        to function as an element in a container.
     */
    template <class Config, CPPGUI_ASPECT(Layouting_aspect), CPPGUI_ASPECT(Updating_aspect)>
    class Abstract_widget:
        public Config::Color_mapper,
        public Config::Font_mapper,
        public Updating_aspect<Layouting_aspect<Nil_struct> >
    {
    public:
        using Renderer      = typename Config::Renderer;
        using Native_color  = typename Renderer::native_color;
        using Font_handle   = typename Renderer::font_handle;
        using Click_handler = std::function<void(const Position &, int button, int clicks)>; // TODO: support return value ?

        auto rectangle() const { return _rect; }
        auto position() const { return _rect.pos; }
        auto extents() const { return _rect.ext; }
        void set_position(const Position &);
        void set_extents(const Extents &);

        // TODO: color and other style definitions belong into stylesheets
        static auto button_face_color        () { return Rgba_norm{ 0.8f, 0.8f, 0.8f, 1 }; }
        static auto button_face_hovered_color() { return Rgba_norm{ 0.9f, 0.9f, 0.9f, 1 }; }

        virtual void mouse_motion(const Position &) {}
        virtual void mouse_click(const Position &, int button, int count);
        virtual void mouse_enter() {};
        virtual void mouse_exit() {};

        virtual void update_render_resources(Renderer *) {}
        void cleanup_render_resources(Renderer *);

        virtual void render(Renderer *, const Position &offset) = 0;

    protected:
        using Config::Color_mapper::get_resource;
        using Config::Font_mapper::get_resource;
        using Config::Color_mapper::release_resource;
        using Config::Font_mapper::release_resource;

        auto rgba_to_native(Renderer *, const Rgba_norm &) -> Native_color;
        void fill(Renderer *r, const Native_color &);

    private:
        Rectangle _rect;
    };

    template <class Aspect_parent>
    struct Nil_layouter: public Aspect_parent {};

    // Widget layouter (optional aspect)

    CPPGUI_DEFINE_ASPECT(Widget_layouter_base)
    {
        virtual auto minimal_size() -> Extents = 0;
        virtual void layout() = 0;
    };

    // Widget 

    template <
        class Config,
        bool With_layouting
    >
    class Widget: public Abstract_widget<Config, Widget_layouter_base, Config::Widget_updater> {
    public:
        using Renderer = typename Config::Renderer;
        using Font_handle = typename Renderer::font_handle;

        void on_click(Click_handler);

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

    template <class Config, bool With_layout> class Abstract_container;

    template <class Config, bool With_layout> class Default_container_updater;

    template <class Config, bool With_layout>
    struct Default_widget_updater {

        CPPGUI_DEFINE_ASPECT(Aspect)
        {
            using Widget_t = Widget<Config, With_layout>;
            using Abstract_container_t = Abstract_container<Config, With_layout>;

            void invalidate();
            void added_to_container(Abstract_container_t *);

            auto container() const { return _container; }

        private:
            Abstract_container_t *_container;
        };
    };

} // ns cppgui