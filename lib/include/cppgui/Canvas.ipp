#include "./Canvas.hpp"

namespace cppgui {

    // Method implementations ---------------------------------------

    template<class Renderer>
    void Canvas<Renderer>::init()
    {
        Renderer::init();

        auto &himg = horizontal_stipple_image(); // TODO: variable size ? (would no longer belong into init() then)
        auto &vimg = vertical_stipple_image(); // TODO: variable size ? (would no longer belong into init() then)

        //_horz_stipple_img = register_rgba32_image(himg.w, himg.h, himg.pixels);
        //_vert_stipple_img = register_rgba32_image(vimg.w, vimg.h, vimg.pixels);
        _horz_stipple_img = register_mono8_image(himg.w, himg.h, himg.pixels);
        _vert_stipple_img = register_mono8_image(vimg.w, vimg.h, vimg.pixels);
    }

    template<class Renderer>
    void Canvas<Renderer>::cleanup()
    {
        // TODO: replace with yet-to-do release_all_images() (called internally by Renderer) ?
        release_rgba32_image(_horz_stipple_img);
        release_rgba32_image(_vert_stipple_img);

        Renderer::cleanup();
    }

    template<class Renderer>
    void Canvas<Renderer>::draw_stippled_rectangle_outline(int x, int y, int w, int h, const Rgba_norm &color)
    {
        auto nativ_clr = rgba_to_native(color);

        draw_greyscale_image_right_righthand(x        , y        , w    , 1, _horz_stipple_img, color, 0, 0);
        draw_greyscale_image_down_righthand (x + w    , y + 1    , h - 1, 1, _horz_stipple_img, color, 0, 0);
        draw_greyscale_image_left_righthand (x + w - 1, y + h    , w - 1, 1, _horz_stipple_img, color, 0, 0);
        draw_greyscale_image_up_righthand   (x        , y + h - 1, h - 1, 1, _horz_stipple_img, color, 0, 0);

        modulate_greyscale_image(x + 1    , y + h - 1, - 1      , - h + 1, _vert_stipple_img, color);

        //modulate_greyscale_image(x        , y        , w        , 1      , _horz_stipple_img, color);
        //modulate_greyscale_image(x + w - 1, y + 1    , 1        , h - 1  , _vert_stipple_img, color);
        //modulate_greyscale_image(x + w - 1, y + h    , - (w - 1), - 1    , _horz_stipple_img, color);

        // The following two lines do not respect "flow": they are drawn left-to-right and top-down 
        // instead of right-to-left and bottom-up
        //draw_image(x        , y + h - 1, w - 1, 1    , _horz_stipple_img);
        //draw_image(x        , y + 1    , 1    , h - 1, _vert_stipple_img);
    }

    /* template<class Renderer>
    auto Canvas<Renderer>::horizontal_stipple_image() -> const Image_definition &
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
    auto Canvas<Renderer>::horizontal_stipple_image() -> const Mono_image_definition &
    {
        static const Mono8 pixels[] = { 0xff, 0xff, 0x00, 0x00 };
        static const Mono_image_definition img_def = { 4, 1, pixels };
        
        return img_def;
    }

    /* template<class Renderer>
    auto Canvas<Renderer>::vertical_stipple_image() -> const Image_definition &
    {
        static const Pixel pixels[] = {
            { 0xff, 0xff, 0xff, 0xff }, { 0xff, 0xff, 0xff, 0xff }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }
        };
        static const Image_definition img_def = {
            1, 4,
            pixels
        };

        return img_def;
    } */

    template<class Renderer>
    auto Canvas<Renderer>::vertical_stipple_image() -> const Mono_image_definition &
    {
        static const Mono8 pixels[] = { 0xff, 0xff, 0x00, 0x00 };
        static const Mono_image_definition img_def = { 1, 4, pixels };
        
        return img_def;
    }

} // ns cppgui