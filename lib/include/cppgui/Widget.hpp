#pragma once

#include <gpc/gui/renderer.hpp> // TODO: other source & namespace

namespace cppgui {

    using Rgba_norm = gpc::gui::rgba_norm; // TODO: 

    struct Position {
        int x, y;
    };

    struct Extents {
        unsigned int w, h;
    };

    struct Rectangle {
        Position pos;
        Extents  ext;
    };

    // TODO?: rename to Widget_base, rename "Derived" to "Widget" ?

    template <class Derived, class Renderer>
    class Widget {
    public:
        using Native_color = typename Renderer::native_color;
        using Font_handle = typename Renderer::font_handle;

        virtual auto minimal_size() -> Extents = 0;

        auto rectangle() const { return _rect; }
        auto position() const { return _rect.pos; }
        auto extents() const { return _rect.ext; }
        void set_position(const Position &);
        void set_extents(const Extents &);

        virtual void update_resources(Renderer *) = 0;

        virtual void render(Renderer *) = 0;

        void update_layout();

    protected:
        auto rgba_to_native(const Rgba_norm &) -> Native_color;
        void fill(Renderer *r, const Native_color &);

    private:
        Rectangle   _rect;
    };

} // ns cppgui