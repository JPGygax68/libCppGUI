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
        auto nativ_clr = Renderer::rgba_to_native(color);

        draw_greyscale_image_right_righthand(x        , y        , w    , 1, _stipple_img, color, 0, 0);
        draw_greyscale_image_down_righthand (x + w    , y + 1    , h - 1, 1, _stipple_img, color, 0, 0);
        draw_greyscale_image_left_righthand (x + w - 1, y + h    , w - 1, 1, _stipple_img, color, 0, 0);
        draw_greyscale_image_up_righthand   (x        , y + h - 1, h - 1, 1, _stipple_img, color, 0, 0);
    }

    template <class Renderer>
    void Canvas<Renderer>::push_clipping_rect(const Rectangle &rect)
    {
        this->set_clipping_rect( rect.pos.x, rect.pos.y, rect.ext.w, rect.ext.h );

        _clipping_stack.push( rect );
    }

    template <class Renderer>
    void Canvas<Renderer>::pop_clipping_rect()
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