#include <cppgui/Internal_popup.hpp>
#include <cppgui/Root_widget.hpp>


namespace cppgui
{
    
    Internal_popup::Internal_popup(Widget *owner):
        _owner{owner}
    {
    }

    void Internal_popup::bring_up(const Extents &min_size, const Extents &max_size, Relative_position pref_pos)
    {
    }

    void Internal_popup::remove()
    {
    }

    auto Internal_popup::absolute_position() -> Point
    {
        // Since popups are not nested within containers, their relative position is also their 
        // absolute one.
        return origin();
    }

    void Internal_popup::invalidate()
    {
        throw std::runtime_error("Internal_popup::invalidate() not implemented yet");
        // _owner->root_widget()->invalidate_popup(this);
    }

} // ns cppgui