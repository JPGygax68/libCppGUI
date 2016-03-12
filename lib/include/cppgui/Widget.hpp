#pragma once

#include <cassert>
#include <type_traits>

#include <gpc/fonts/rasterized_font.hpp>
#include <gpc/gui/renderer.hpp> // TODO: other source & namespace

#include "./Resource_mapper.hpp"

namespace cppgui {

    using Rgba_norm = gpc::gui::rgba_norm;
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

    class Widget_base {
    public:
        auto rectangle() const { return _rect; }
        auto position() const { return _rect.pos; }
        auto extents() const { return _rect.ext; }
        void set_position(const Position &);
        void set_extents(const Extents &);

        virtual void mouse_motion(const Position &) {};

        virtual void mouse_enter();
        virtual void mouse_exit();

        bool hovered() const { return _hovered; }

    protected:
        void trigger_redraw();

        Rectangle   _rect;

        bool        _hovered = false;
    };

    /** Concept Widget_config
     */
    template <typename Renderer>
    struct Widget_config {

        using Font_handle = typename Renderer::font_handle;

        struct Font_mapper : public Resource_mapper<
            Font_mapper, 
            Renderer, 
            const Rasterized_font *, Font_handle, 
            Renderer::font_handles_are_resources
        > {
            auto obtain(Renderer *r, const Rasterized_font *font) { 
                return r->register_font(*font); }
            void release(Renderer *r, Font_handle hnd) { 
                r->release_font(hnd); }
        };

        //using Color_mapper = ...;

        //static const bool color_mapping_expensive = false;
        //static const bool font_mapping_expensive = true;
    };

    template <class Config>
    class Widget: public Widget_base,
        protected Config::Color_mapper,
        protected Config::Font_mapper
    {
    public:
        using Renderer     = typename Config::Renderer;
        using Native_color = typename Renderer::native_color;
        using Font_handle  = typename Renderer::font_handle;

        virtual void update_render_resources(Renderer *) = 0;
        
        virtual void render(Renderer *, const Position &offset) = 0;

        void cleanup_render_resources(Renderer *);

    protected:
        using Config::Color_mapper::get_resource;
        using Config::Font_mapper::get_resource;
        using Config::Color_mapper::release_resource;
        using Config::Font_mapper::release_resource;

        auto rgba_to_native(Renderer *, const Rgba_norm &) -> Native_color;
        void fill(Renderer *r, const Native_color &);
    };

    class Widget_layouter {
    public:
        virtual auto minimal_size() -> Extents = 0;
        virtual void layout() = 0;
    };

} // ns cppgui