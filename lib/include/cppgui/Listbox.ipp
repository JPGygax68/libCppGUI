#include "./Listbox.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    Listbox<Config, With_layout>::Listbox()
    {
        _content_pane.set_layout_type(Layout_type::stack); // TODO: will need adapting

        set_content_pane(&_content_pane);
    }

    template<class Config, bool With_layout>
    void Listbox<Config, With_layout>::add_item(Layoutable_widget_t *item)
    {
        _content_pane.add_child(item);
    }

    template<class Config>
    template<class Aspect_parent>
    void Listbox__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        p()->_content_pane.set_extents({ p()->content_rect().ext.w, p()->_content_pane.get_minimal_size().h });

        Scrollbox_t::layout();

        auto full  = p()->_content_pane.children().size();
        auto shown = full * p()->content_rect().ext.h / p()->_content_pane.extents().h;

        p()->scrollbar().define_range(full, shown, 1);
    }

} // ns cppgui
