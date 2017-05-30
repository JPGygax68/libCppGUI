#pragma once

#include "./UI_element.hpp"


namespace cppgui 
{
    
    class Widget;


    class Internal_popup: public UI_element
    {
    public:
        
        explicit Internal_popup(Widget *owner);

        void bring_up(const Extents &min_size, const Extents &max_size, Relative_position pref_pos = below);

        void remove();

        auto absolute_position() -> Point override;

        void invalidate() override;

    private:
        Widget              *_owner;
    };
    
} // ns cppgui