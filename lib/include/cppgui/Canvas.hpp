#pragma once

/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016 Hans-Peter Gygax

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "./basic_types.hpp"

#include "./Full_resource_mapper.hpp" // TODO: replace with auto-selecting Resource_mapper.hpp as soon as ready

namespace cppgui {

    namespace _canvas {

        /** TODO: Auto-select the correct mapper type by querying static properties of Renderer.
         */
        template <class Renderer>
        struct Font_mapper : public Full_resource_mapper<
            Font_mapper<Renderer>,
            Renderer,
            const gpc::fonts::rasterized_font *, 
            typename Renderer::font_handle >
        {
            using Font_handle = typename Renderer::font_handle;

            auto obtain (Renderer *r, const Rasterized_font *font) { return r->register_font(*font); }
            void release(Renderer *r, Font_handle hnd)             { r->release_font(hnd); }
        };
    }

    template <class Renderer>
    class Canvas: public Renderer /*, public _canvas::Font_mapper<Renderer> */
    {
    public:
        using Native_color      = typename Renderer::native_color;
        using Native_mono       = typename Renderer::native_mono;
        using Font_handle       = typename Renderer::font_handle;
        using Image_handle      = typename Renderer::image_handle;
        using Rgba_norm         = typename gpc::gui::rgba_norm;
        using Rgba32            = typename gpc::gui::rgba32;
        using Mono8             = typename gpc::gui::mono8;
        using Font_mapper       = typename _canvas::Font_mapper<Renderer>;

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
        void init()
        {
            Renderer::init();
            auto &img = stipple_image(); // TODO: variable size ? (would no longer belong into init() then)
            _stipple_img = register_mono8_image(img.w, img.h, img.pixels);
        }
        void cleanup()
        {
            // TODO: replace with yet-to-do release_all_images() (called internally by Renderer) ?
            release_mono8_image(_stipple_img);
            Renderer::cleanup();
        }

        static constexpr auto adapt_resource(const Rgba_norm &color) { return color; }

        auto adapt_resource(const Rasterized_font *font) -> Font_handle { return _font_mapper.adapt_resource(this, font); }

        // TODO: move to Renderer ?
        void draw_stippled_rectangle_outline(int x, int y, int w, int h, const Rgba_norm &color)
        {
            auto nativ_clr = rgba_to_native(color);

            draw_greyscale_image_right_righthand(x        , y        , w    , 1, _stipple_img, color, 0, 0);
            draw_greyscale_image_down_righthand (x + w    , y + 1    , h - 1, 1, _stipple_img, color, 0, 0);
            draw_greyscale_image_left_righthand (x + w - 1, y + h    , w - 1, 1, _stipple_img, color, 0, 0);
            draw_greyscale_image_up_righthand   (x        , y + h - 1, h - 1, 1, _stipple_img, color, 0, 0);
        }

    private:
        static auto stipple_image() -> const Mono_image_definition & {

            static const Mono8 pixels[] = { 0xff, 0xff, 0x00, 0x00 };
            static const Mono_image_definition img_def = { 4, 1, pixels };
            return img_def;
        }

        Font_mapper                 _font_mapper;

        Image_handle                _stipple_img;
    };

} // ns cppgui