#pragma once
#include "Widget.hpp"


namespace cppgui
{
    
    /*
     * Widget that uses text in some way (i.e. a font).
     * 
     * TODO: it would probably be better to implement this functionality as a "decorator" template,
     * to be wrapped around Widget.
     */
    class Text_widget: public Widget
    {
    public:

        void init() override;

    protected:
        auto font() { return _font_rsrc->rasterized; }
        auto font_handle() { return _font_rsrc->get(); }

    private:
        const Font_resource        *_font_rsrc = nullptr;
    };

} // ns cppgui