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

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Aspect_parent>
    void Listbox__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto& content_pane = p()->_content_pane;
        auto w = p()->content_rect().ext.w;
        auto h = content_pane.get_minimal_size().h;
        //h = n * ((h + content_pane.children()[0]->extents().h - 1) / n); // TODO: graceful failure when there are no children
        p()->_content_pane.set_extents({ w, h });

        Scrollbox_t::layout();

        auto full = h;
        auto h_item = h / content_pane.children().size();
        auto shown = h_item * (p()->content_rect().ext.h / h_item);
        p()->scrollbar().define_range(full, shown, h_item);
    }

} // ns cppgui
