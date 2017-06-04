#pragma once

#include "./Container.hpp"


namespace cppgui
{
    class Grid_container: public Container_base
    {
    public:

        #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:
        void set_column_count(uint);
        void set_row_count(uint);

        auto get_minimal_bbox() -> Bbox override;


    private:
        uint        _col_count = 0;
        uint        _row_count = 0;
    #endif 
    };

} // ns cppgui