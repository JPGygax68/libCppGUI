#pragma once
#include "Widget.hpp"


namespace cppgui
{
    
    /*
     * Widget that uses text in some way (i.e. a font).
     */
    class Text_widget: public Widget
    {
    public:

        void set_font(const Rasterized_font *);

        void init(Canvas *) override;

    protected:
        auto& font() const { return _font; }
        auto& font() { return _font; }

    private:
        Font_resource           _font;
    };

} // ns cppgui