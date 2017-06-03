#include <cppgui/Container.hpp>
#include <cppgui/UI_builder.hpp>


namespace cppgui
{
    void UI_builder_base::internal_add(Widget *w)
    {
        container().add_child(w);

        _widgets.push_back(std::unique_ptr<Widget>{w});
    }

} // ns cppgui