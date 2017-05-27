#pragma once

#include "./Widget.hpp"


namespace cppgui 
{
    
    class Widget;


    class Internal_popup_zone: public Widget
    {
    public:
        
        explicit Internal_popup_zone(Widget *owner);

    private:
        Widget              *_owner;
    };
    
} // ns cppgui