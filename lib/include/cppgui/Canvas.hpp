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
#include <cppgui_config.hpp>
#include "./basic_types.hpp"
#include "./Resource.hpp"
#include CPPGUI_RENDERER_HEADER
#include "./Full_resource_mapper.hpp" // TODO: replace with auto-selecting Resource_mapper.hpp as soon as ready


namespace cppgui {

    namespace _canvas {

        /** TODO: Auto-select the correct mapper type by querying static properties of Renderer.
         */
        struct Font_mapper : public Full_resource_mapper<
            Font_mapper,
            CPPGUI_RENDERER_CLASS,
            const fonts::Rasterized_font *, 
            Font_handle 
        >
        {
            auto obtain (CPPGUI_RENDERER_CLASS *r, const Rasterized_font *font) { return r->register_font(*font); }
            void release(CPPGUI_RENDERER_CLASS *r, Font_handle hnd)             { r->release_font(hnd); }
        };
    }

    class Canvas;

    class _Font_mapper
    {
    public:

        auto translate(CPPGUI_RENDERER_CLASS *r, const Rasterized_font *f) -> Font_handle;

        void release  (CPPGUI_RENDERER_CLASS *r, Font_handle h);

    private:
        std::map<const Rasterized_font *, Font_handle>  _map;
        //std::vector<SourceType>             _laundry;
    };

    struct Font_resource
    {
        // TODO: it might be possible to put these two fields into a union
        const Rasterized_font  *rasterized;
        Font_handle             handle = 0;

        void assign(const Rasterized_font *font);
        auto get() const -> Font_handle;

        void translate(Canvas * canvas);
        void release(Canvas * canvas);
    };

    inline void Font_resource::assign(const Rasterized_font * font)
    {
        assert(!handle);
        rasterized = font;
    }

    inline auto Font_resource::get() const -> Font_handle
    {
        assert(handle);
        return handle;
    }


    class Canvas: public CPPGUI_RENDERER_CLASS
    {
    public:
        //using Font_resource     = Resource<const Rasterized_font *, Font_handle, Renderer, false>;
        //using Font_mapper       = _canvas::Font_mapper;

        struct Image_definition {
            Length          w, h;
            const RGBA32   *pixels;
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

        //static constexpr auto adapt_resource(const RGBA &color) { return color; }
        //auto adapt_resource(const Rasterized_font *font) -> Font_handle { return _font_mapper.adapt_resource(this, font); }

        // Font management
        auto register_font(const Rasterized_font *rf) -> Font_handle;
        void release_font(Font_handle h);

        // TODO: move to Renderer ?
        void draw_stippled_rectangle_outline(int x, int y, int w, int h, const RGBA &color);

        void push_clipping_rect(const Rectangle &);
        void pop_clipping_rect ();

    private:
        static auto stipple_image() -> const Mono_image_definition &;

        _Font_mapper                _font_mapper;

        Image_handle                _stipple_img;

        std::stack<Rectangle>       _clipping_stack;
    };

} // ns cppgui