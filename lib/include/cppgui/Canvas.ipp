#include "./Canvas.hpp"

namespace cppgui {

    // Method implementations ---------------------------------------

    template<class Renderer>
    void Canvas<Renderer>::init()
    {
        Renderer::init();

        auto &img = stipple_image(); // TODO: variable size ? (would no longer belong into init() then)

        _stipple_img = register_mono8_image(img.w, img.h, img.pixels);
    }

    template<class Renderer>
    void Canvas<Renderer>::cleanup()
    {
        // TODO: replace with yet-to-do release_all_images() (called internally by Renderer) ?
        release_mono8_image(_stipple_img);

        Renderer::cleanup();
    }

    /*
    template<class Renderer>
    auto Canvas<Renderer>::translate_resource(const Rasterized_font *font) -> Font_handle
    {
        return _font_mapper.get_resource(this, font);
    }
    */

    template<class Renderer>
    void Canvas<Renderer>::draw_stippled_rectangle_outline(int x, int y, int w, int h, const Rgba_norm &color)
    {
        auto nativ_clr = rgba_to_native(color);

        draw_greyscale_image_right_righthand(x        , y        , w    , 1, _stipple_img, color, 0, 0);
        draw_greyscale_image_down_righthand (x + w    , y + 1    , h - 1, 1, _stipple_img, color, 0, 0);
        draw_greyscale_image_left_righthand (x + w - 1, y + h    , w - 1, 1, _stipple_img, color, 0, 0);
        draw_greyscale_image_up_righthand   (x        , y + h - 1, h - 1, 1, _stipple_img, color, 0, 0);
    }

    /* template<class Renderer>
    auto Canvas<Renderer>::stipple_image() -> const Image_definition &
    {
        static const Pixel pixels[] = {
            { 0xff, 0xff, 0xff, 0xff }, { 0xff, 0xff, 0xff, 0xff }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }
        };
        static const Image_definition img_def = {
            4, 1,
            pixels
        };
        
        return img_def;
    } */

    template<class Renderer>
    auto Canvas<Renderer>::stipple_image() -> const Mono_image_definition &
    {
        static const Mono8 pixels[] = { 0xff, 0xff, 0x00, 0x00 };
        static const Mono_image_definition img_def = { 4, 1, pixels };
        
        return img_def;
    }

} // ns cppgui