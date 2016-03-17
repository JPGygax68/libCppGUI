#pragma once

#include "./Widget.hpp"
#include "./Container.hpp"

namespace cppgui {

    template<class Config, bool WithLayout>
    void Default_widget_update_handler<Config, WithLayout>::invalidate()
    {
        // Pass "up" to container
        _container->child_invalidated(static_cast<WidgetT*>(this));
    }

    template<class Config, bool WithLayout>
    inline void Default_widget_update_handler<Config, WithLayout>::added_to_container(Abstract_container<Config, WithLayout> *cont)
    {
        _container = cont;
    }

    template<class Config, bool WithLayout>
    inline void Default_container_update_handler<Config, WithLayout>::child_added(Widget<Config, WithLayout> *child)
    {
        child->set_container(static_cast<ContainerT*>(this));
    }

    template<class Config, bool WithLayout>
    void Default_container_update_handler<Config, WithLayout>::child_invalidated(Widget<Config, WithLayout> *)
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

    template<class Config, bool WithLayout>
    void Widget<Config, WithLayout>::invalidate()
    {
        Config::Widget_update_handler::invalidate();
    }

} // ns cppgui