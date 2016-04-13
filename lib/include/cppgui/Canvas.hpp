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

    private:
        Image_handle        _stipple_img;

        static auto stipple_image() -> const Image_definition &;

    };

} // ns cppgui