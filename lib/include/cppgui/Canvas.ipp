#include "./Canvas.hpp"

namespace cppgui {

    // Method implementations ---------------------------------------

    template<class Renderer>
    void Canvas<Renderer>::init()
    {
        Renderer::init();

        const Image_definition &img_def = stipple_image(); // TODO: variable size ? (would no longer belong into init() then)

        _stipple_img = register_rgba32_image(img_def.w, img_def.h, img_def.pixels);
    }

    template<class Renderer>
    void Canvas<Renderer>::cleanup()
    {
        release_rgba32_image(_stipple_img);

        Renderer::cleanup();
    }

    template<class Renderer>
    auto Canvas<Renderer>::stipple_image() -> const Image_definition &
    {
        static const Pixel pixels[] = {
            { 0xff, 0xff, 0xff, 0xff }, { 0xff, 0xff, 0xff, 0xff } 
        };
        static const Image_definition img_def = {
            2, 1,
            pixels
        };
        
        return img_def;
    }

} // ns cppgui