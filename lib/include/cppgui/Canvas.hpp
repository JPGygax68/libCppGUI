#pragma once

#include "./basic_types.hpp"

namespace cppgui {

    template <class Renderer>
    class Canvas: public Renderer {
    public:
        using Native_color      = typename Renderer::native_color;
        using Native_mono       = typename Renderer::native_mono;
        using Font_handle       = typename Renderer::font_handle;
        using Image_handle      = typename Renderer::image_handle;
        using Rgba_norm         = typename gpc::gui::rgba_norm;
        using Rgba32            = typename gpc::gui::rgba32;
        using Mono8             = typename gpc::gui::mono8;

        struct Image_definition {
            Length          w, h;
            const Rgba32   *pixels;
            // TODO: extra parameters ?
        };

        struct Mono_image_definition {
            Length          w, h;
            const Mono8    *pixels;
            // TODO: extra parameters ?
        };

        // Must be called when renderer is ready (for resource allocation at least [TODO: formal state definitions])
        void init();
        void cleanup();

        // TODO: move to Renderer ?
        void draw_stippled_rectangle_outline(int x, int y, int w, int h, const Rgba_norm &color);

    private:
        Image_handle        _horz_stipple_img, _vert_stipple_img;

        static auto horizontal_stipple_image() -> const Mono_image_definition &;
        static auto vertical_stipple_image() -> const Mono_image_definition &;

    };

} // ns cppgui