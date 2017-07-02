#include <cppgui/Positioned_bbox.hpp>


namespace cppgui
{
    auto Positioned_bbox::operator+(const Point &d) const -> Positioned_bbox
    {
        return { orig + d, bbox };
    }

} // ns cppgui