#pragma once

#include <cassert>
#include <type_traits>

#include <gpc/gui/renderer.hpp> // TODO: other source & namespace

namespace cppgui {

    using Rgba_norm = gpc::gui::rgba_norm;

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

    template <class Renderer>
    class Widget: public Widget_base {
    public:
        using Native_color = typename Renderer::native_color;
        using Font_handle = typename Renderer::font_handle;

        virtual void update_resources(Renderer *) = 0;
        virtual void render(Renderer *, const Position &offset) = 0;

    protected:
        auto rgba_to_native(const Rgba_norm &) -> Native_color;
        void fill(Renderer *r, const Native_color &);
    };

    class Widget_layouter {
    public:
        virtual auto minimal_size() -> Extents = 0;
        virtual void layout() = 0;
    };

} // ns cppgui