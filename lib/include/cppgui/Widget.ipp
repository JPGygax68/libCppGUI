#pragma once

#include "./Widget.hpp"
#include "./Container.hpp"
#include "Root_widget.hpp"

namespace cppgui {

    template<class Config, bool With_layout>     
    template<class Next_aspects>
    inline void Default_widget_updater<Config, With_layout>::Aspect<Next_aspects>::invalidate()
    {
        auto c = static_cast<Abstract_container<Config, With_layout>*>(this->container());

        // Pass "up" to container
        c->child_invalidated(static_cast<Widget_t*>(this));
    }

    template<class Config, bool With_layout>
    template<class Next_aspects>
    void Default_root_widget_updater<Config, With_layout>::Aspect<Next_aspects>::added_to_container(Abstract_container_t *)
    {
        // DO NOTHING
    }

    template<class Config, bool With_layout>
    template<class Next_aspects>
    inline void Default_widget_updater<Config, With_layout>::Aspect<Next_aspects>::added_to_container(Abstract_container_t *cont)
    {
        _container = cont;
    }

    template <class Config, template <class = Nil_aspect> class Layouting_aspect, template <class = Nil_aspect> class Updating_aspect>
    inline void Abstract_widget<Config, Layouting_aspect, Updating_aspect>::set_position(const Position &pos)
    {
        _rect.pos = pos;
    }

    template <class Config, template <class = Nil_aspect> class Layouting_aspect, template <class = Nil_aspect> class Updating_aspect>
    inline void Abstract_widget<Config, Layouting_aspect, Updating_aspect>::set_extents(const Extents &ext)
    {
        _rect.ext = ext;
    }

    template <class Config, template <class = Nil_aspect> class Layouting_aspect, template <class = Nil_aspect> class Updating_aspect>
    inline void Abstract_widget<Config, Layouting_aspect, Updating_aspect>::cleanup_render_resources(Renderer *r)
    {
        Config::Color_mapper::release_all_resources(r);
        Config::Font_mapper ::release_all_resources(r);
    }

    template <class Config, template <class = Nil_aspect> class Layouting_aspect, template <class = Nil_aspect> class Updating_aspect>
    inline auto Abstract_widget<Config, Layouting_aspect, Updating_aspect>::rgba_to_native(Renderer *r, const Rgba_norm &color) -> Native_color
    {
        return get_resource(r, color);
    }

    template <class Config, template <class = Nil_aspect> class Layouting_aspect, template <class = Nil_aspect> class Updating_aspect>
    inline void Abstract_widget<Config, Layouting_aspect, Updating_aspect>::fill(Renderer *r, const Native_color &color)
    {
        auto b{ rectangle() };
        r->fill_rect(b.pos.x, b.pos.y, b.ext.w, b.ext.h, color);
    }

    template<class Config, bool With_layout>
    inline void Widget<Config, With_layout>::mouse_enter()
    {
        _hovered = true;
        invalidate();
    }

    template<class Config, bool With_layout>
    inline void Widget<Config, With_layout>::mouse_exit()
    {
        _hovered = false;
        invalidate();
    }

} // ns cppgui