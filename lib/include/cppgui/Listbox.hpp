#pragma once

#include "./Boxed.hpp"
#include "./Scrolling_container.hpp"


namespace cppgui {
    
    class Listbox_base: public Scrolling_container 
    {
    public:

        Listbox_base();
        
        void add_item(Widget *);
    }; 
    

    // TODO: define and use a template called Container_boxed<> ?
    using Listbox = Boxed<Listbox_base, Simple_frame_box_styles>;
    //using Listbox = Listbox_base;

} // ns cppgui