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

#include "./Canvas.hpp"

#include "./Full_resource_mapper.ipp"

namespace cppgui {

    auto _Font_mapper::translate(CPPGUI_RENDERER_CLASS * r, const Rasterized_font * f) -> Font_handle
    {
        auto it = _map.find(f);
        if (it != end(_map)) return it->second;

        auto h = r->register_font(*f);
        _map[f] = h;
        return h;
    }

    void _Font_mapper::release(CPPGUI_RENDERER_CLASS * r, Font_handle h)
    {
        // NO-OP for the time being: we do not release OpenGL resources created for fonts
        // TODO: actual implementation
        // TODO: reference-counting!
    }

    void Font_resource::translate(Canvas * canvas)
    {
        assert(!handle);
        handle = canvas->register_font(rasterized);
    }

    void Font_resource::release(Canvas * canvas)
    {
        assert(handle);
        canvas->release_font(handle);
        handle = 0;
    }

    void Canvas::init()
    {
        CPPGUI_RENDERER_CLASS::init();

        auto &img = stipple_image(); // TODO: variable size ? (would no longer belong into init() then)

        _stipple_img = register_mono8_image(img.w, img.h, img.pixels);
    }

    void Canvas::cleanup()
    {
        // TODO: replace with yet-to-do release_all_images() (called internally by Renderer) ?
        release_mono8_image(_stipple_img);

        CPPGUI_RENDERER_CLASS::cleanup();
    }

    auto Canvas::register_font(const Rasterized_font * rf) -> Font_handle
    {
        return _font_mapper.translate(this, rf);
    }

    void Canvas::release_font(Font_handle h)
    {
        /* NO-OP: we do not release font textures for the time being; to do this properly
         * would require LRU, and possibly more sophisticated management. */
    }

    void Canvas::draw_stippled_rectangle_outline(int x, int y, int w, int h, const RGBA &color)
    {
        auto nativ_clr = rgba_to_native(color);

        draw_greyscale_image_right_righthand(x        , y        , w    , 1, _stipple_img, color, 0, 0);
        draw_greyscale_image_down_righthand (x + w    , y + 1    , h - 1, 1, _stipple_img, color, 0, 0);
        draw_greyscale_image_left_righthand (x + w - 1, y + h    , w - 1, 1, _stipple_img, color, 0, 0);
        draw_greyscale_image_up_righthand   (x        , y + h - 1, h - 1, 1, _stipple_img, color, 0, 0);
    }

    void Canvas::push_clipping_rect(const Rectangle &rect)
    {
        this->set_clipping_rect( rect.pos.x, rect.pos.y, rect.ext.w, rect.ext.h );

        _clipping_stack.push( rect );
    }

    void Canvas::pop_clipping_rect()
    {
        _clipping_stack.pop();

        if (!_clipping_stack.empty())
        {
            auto &rect = _clipping_stack.top();
            this->set_clipping_rect( rect.pos.x, rect.pos.y, rect.ext.w, rect.ext.h );
        }
        else
        {
            this->cancel_clipping();
        }
    }

    /* 
    auto Canvas::stipple_image() -> const Image_definition &
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

    auto Canvas::stipple_image() -> const Mono_image_definition &
    {
        static const Mono8 pixels[] = { 0xff, 0xff, 0x00, 0x00 };
        static const Mono_image_definition img_def = { 4, 1, pixels };
        
        return img_def;
    }

} // ns cppgui