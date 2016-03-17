#include <cppgui/Root_widget.hpp>

namespace cppgui {

    template <class Config, bool WithLayout>
    inline Root_widget<Config, WithLayout>::Root_widget()
    {
        set_position({ 0, 0 }); // TODO: this would not be necessary if Root_widget did not inherit from Container
    }

    template<class Config, bool WithLayout>
    void Root_widget<Config, WithLayout>::add_child(Widget<Config, WithLayout> *child)
    {
        _add_child(child);
        static_cast<Config::Widget_update_handler*>(child)->added_to_container(static_cast<Abstract_container<Config, WithLayout>*>(this));
    }

    template<class Config, bool WithLayout>
    void Root_widget<Config, WithLayout>::mouse_motion(const Position &pos)
    {
        handle_mouse_motion(pos); // Abstract_container
    }

    template<class Config, bool WithLayout>
    inline void Root_widget<Config, WithLayout>::render(Renderer *r)
    {
        // TODO: the following is temporary - it must be made optional
        r->clear(r->rgba_to_native({ 0, 0.5f, 0.2f, 1 }));
        
        for (auto& child : children())
        {
            child->render(r, { 0, 0 });
        }
    }

} // ns cppgui