#pragma once

#include "./basic_types.hpp"

namespace cppgui {

    template <class Renderer>
    class Canvas: public Renderer {
    public:
        using Native_color      = typename Renderer::native_color;
        using Font_handle       = typename Renderer::font_handle;
        using Image_handle      = typename Renderer::image_handle;
        using Pixel             = typename gpc::gui::rgba32; // TODO: move this to Renderer concept itself

        struct Image_definition {
            Length          w, h;
            const Pixel    *pixels;
            // TODO: extra parameters ?
        };

        // Must be called when renderer is ready (for resource allocation at least [TODO: formal state definitions])
        void init();
        void cleanup();

        // TODO: move to Renderer ?
        void draw_stippled_rectangle_outline(int x, int y, int w, int h);

    private:
        Image_handle        _horz_stipple_img, _vert_stipple_img;

        static auto horizontal_stipple_image() -> const Image_definition &;
        static auto vertical_stipple_image() -> const Image_definition &;

    };

} // ns cppgui