#pragma once

#include "./Widget.hpp"
#include "./Container.hpp"

namespace cppgui {


    template<class Container>
    inline void Default_widget_update_handler<Container>::set_container(Container *cont)
    {
        _container = cont;
    }

    template<class Widget>
    inline void Default_widget_update_handler<Widget>::invalidate()
    {
        assert(_container);
        _container->invalidate_child(static_cast<Widget*>(this));
    }

    template<class Widget, class Container>
    inline void Default_container_update_handler<Widget, Container>::child_added(Widget *child)
    {
        child->set_container(static_cast<Container*>(this));
    }

    template<class Widget, class Container>
    inline void cppgui::Default_container_update_handler<Widget, Container>::invalidate_child(Widget *)
    {
        // TODO
    }

    template<class Config, bool WithLayout>
    inline void Abstract_widget<Config, WithLayout>::set_position(const Position &pos)
    {
        _rect.pos = pos;
    }

    template<class Config, bool WithLayout>
    inline void Abstract_widget<Config, WithLayout>::set_extents(const Extents &ext)
    {
        _rect.ext = ext;
    }

    template<class Config, bool WithLayout>
    inline void Abstract_widget<Config, WithLayout>::cleanup_render_resources(Renderer *r)
    {
        Config::Color_mapper::release_all_resources(r);
        Config::Font_mapper ::release_all_resources(r);
    }

    template <class Config, bool WithLayout>
    inline auto Abstract_widget<Config, WithLayout>::rgba_to_native(Renderer *r, const Rgba_norm &color) -> Native_color
    {
        return get_resource(r, color);
    }

    template <class Config, bool WithLayout>
    inline void Abstract_widget<Config, WithLayout>::fill(Renderer *r, const Native_color &color)
    {
        auto b{ rectangle() };
        r->fill_rect(b.pos.x, b.pos.y, b.ext.w, b.ext.h, color);
    }

    template<class Config, bool WithLayout>
    inline void Widget<Config, WithLayout>::mouse_enter()
    {
        _hovered = true;
        invalidate();
    }

    template<class Config, bool WithLayout>
    inline void Widget<Config, WithLayout>::mouse_exit()
    {
        _hovered = false;
        invalidate();
    }

} // ns cppgui