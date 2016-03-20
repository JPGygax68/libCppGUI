#pragma once

#include <gpc/fonts/rasterized_font.hpp>

#include "./Full_resource_mapper.hpp"

namespace cppgui {

    // TODO: derive from a pseudo-concept to have some checks

    template <class Renderer>
    struct Default_font_mapper : public Full_resource_mapper<
        Default_font_mapper<Renderer>,
        Renderer,
        const gpc::fonts::rasterized_font *, 
        typename Renderer::font_handle
    >
    {
        using Rasterized_font = typename gpc::fonts::rasterized_font;
        using Font_handle = typename Renderer::font_handle;

        auto obtain(Renderer *r, const Rasterized_font *font) {
            return r->register_font(*font);
        }
        void release(Renderer *r, Font_handle hnd) {
            r->release_font(hnd);
        }
    };

} // ns cppgui
