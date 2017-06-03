#pragma once

#include "./Container.hpp"


namespace cppgui
{
    class Grid_container: public Container_base
    {
    public:

        //Grid_container() {}
        //Grid_container(const Grid_container &) = delete;
        //Grid_container(Grid_container &&) = default;

        #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:
        auto get_minimal_bbox() -> Bbox override;
    #endif 
    };

} // ns cppgui