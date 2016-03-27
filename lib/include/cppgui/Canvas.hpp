#pragma once

namespace cppgui {

    template <class Renderer>
    class Canvas: public Renderer {
    public:
        using Native_color      = typename Renderer::native_color;
        using Font_handle       = typename Renderer::font_handle;
    };

} // ns cppgui