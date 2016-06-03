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

#include <stack>

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

    #define CPPGUI_INSTANTIATE_CANVAS(Config) \
        template cppgui::Canvas<Config::Renderer>;

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
        void init   ();
        void cleanup();

        static constexpr auto adapt_resource(const Rgba_norm &color) { return color; }
        auto adapt_resource(const Rasterized_font *font) -> Font_handle { return _font_mapper.adapt_resource(this, font); }

        // TODO: move to Renderer ?
        void draw_stippled_rectangle_outline(int x, int y, int w, int h, const Rgba_norm &color);

        void push_clipping_rect(const Rectangle &);
        void pop_clipping_rect ();

    private:
        static auto stipple_image() -> const Mono_image_definition &;

        Font_mapper                 _font_mapper;

        Image_handle                _stipple_img;

        std::stack<Rectangle>       _clipping_stack;
    };

} // ns cppgui