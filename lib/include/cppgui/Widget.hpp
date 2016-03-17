#pragma once

#include <cassert>
#include <type_traits>

#include <gpc/fonts/rasterized_font.hpp>
#include <gpc/gui/renderer.hpp> // TODO: other source & namespace

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

    // Forward declarations
    template <class Config, bool WithLayout> class Abstract_container;
    template <class Config, bool WithLayout> class Widget;

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

    // Default update handlers 

    template <class Config, bool WithLayout>
    struct Default_container_update_handler_base {
        void child_added(Widget<Config, WithLayout> *);
        void child_invalidated(Widget<Config, WithLayout> *);
        virtual void handle_child_invalidated(Widget<Config, WithLayout> *) = 0;
    };

    template <class Config, bool WithLayout>
    struct Default_container_update_handler: public Default_container_update_handler_base<Config, WithLayout> {
        void handle_child_invalidated(Widget<Config, WithLayout> *) override;
    };

    template <class Config, bool WithLayout>
    struct Default_widget_update_handler {
        void invalidate();
        void added_to_container(Abstract_container<Config, WithLayout> *);
    private:
        Abstract_container<Config, WithLayout> *_container;
    };

    template <class Config, bool WithLayout>
    class Abstract_widget:
        public Config::Color_mapper,
        public Config::Font_mapper
    {
    public:
        using Renderer     = typename Config::Renderer;
        using Native_color = typename Renderer::native_color;
        using Font_handle  = typename Renderer::font_handle;

        auto rectangle() const { return _rect; }
        auto position() const { return _rect.pos; }
        auto extents() const { return _rect.ext; }
        void set_position(const Position &);
        void set_extents(const Extents &);

        // TODO: color and other style definitions belong into stylesheets
        static auto button_face_color        () { return Rgba_norm{ 0.8f, 0.8f, 0.8f, 1 }; }
        static auto button_face_hovered_color() { return Rgba_norm{ 0.9f, 0.9f, 0.9f, 1 }; }

        virtual void mouse_motion(const Position &) {}
        virtual void mouse_enter() {};
        virtual void mouse_exit() {};

        virtual void update_render_resources(Renderer *) {}
        void cleanup_render_resources(Renderer *);

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

    template <class Config, bool WithLayout>
    class Widget: public Abstract_widget<Config, WithLayout>,
        public Config::Widget_update_handler
    {
    public:
        // TODO: should the following be protected ?
        bool hovered() const { return _hovered; }

        void mouse_enter() override;
        void mouse_exit() override;

        virtual void render(Renderer *, const Position &offset) = 0;

    protected:
        virtual void invalidate();

    private:
        bool _hovered = false;
    };

    class Widget_layouter {
    public:
        virtual auto minimal_size() -> Extents = 0;
        virtual void layout() = 0;
    };

} // ns cppgui