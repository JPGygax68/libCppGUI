#include <cppgui/Root_widget.hpp>

namespace cppgui {

    template<class Config, bool With_layout>
    Root_widget<Config, With_layout>::Root_widget(Renderer *r)
    {
        _renderer = r;
    }

    template<class Config, bool WithLayout>
    void Root_widget<Config, WithLayout>::add_child(Widget<Config, WithLayout> *child)
    {
        _add_child(child);
        child->added_to_container(static_cast<Abstract_container_t*>(this));
    }

    template<class Config, bool With_layout>
    auto Root_widget<Config, With_layout>::get_font_handle(const Rasterized_font *font) -> Font_handle
    {
        return _font_mapper.get_resource(_renderer, font);
    }

    template<class Config, bool WithLayout>
    void Root_widget<Config, WithLayout>::mouse_motion(const Position &pos)
    {
        handle_mouse_motion(pos); // Abstract_container
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::mouse_click(const Position &pos, int button, int count)
    {
        handle_mouse_click(pos, button, count);
    }

    template<class Config, bool With_layout>
    void Root_widget<Config, With_layout>::text_input(const char32_t *, size_t)
    {
        // TODO
        assert(false);
    }

    template<class Config, bool WithLayout>
    inline void Root_widget<Config, WithLayout>::render(Renderer *r, const Position &offs)
    {
        auto pos = offs + position();

        // TODO: the following is temporary - it must be made optional
        r->clear(r->rgba_to_native({ 0, 0.5f, 0.2f, 1 }));
        
        for (auto& child : children())
        {
            child->render(r, pos);
        }
    }

    // Layouter aspect ----------------------------------------------

    template <class Config>
    template<class Aspect_parent>
    inline void Root_widget_layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto p = static_cast<Root_widget_t*>(this);

        p->layout_children();

        /*
        for (auto child : p->children())
        {
            child->layout();
        }
        */
    }

} // ns cppgui