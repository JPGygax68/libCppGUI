#include <cppgui/Grid_container.hpp>


namespace cppgui
{

#ifndef CPPGUI_EXCLUDE_LAYOUTING

    auto Grid_container::get_minimal_bbox() -> Bbox
    {
        // TODO!!!

        return {0, 500, 300, 0};
    }

#endif // !CPPGUI_EXCLUDE_LAYOUTING

} // ns cppgui