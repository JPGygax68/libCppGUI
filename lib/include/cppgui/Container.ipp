#include <algorithm>

#include "./Container.hpp"

namespace cppgui {

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::init()
    {
        init_children_resources();

        Widget_t::init();
    }

    template <class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_motion(const Position &pos)
    {
        container_mouse_motion(pos);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_click(const Position &pos, int button, int count)
    {
        container_mouse_click(pos - position(), button, count);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_wheel(const Position &dist)
    {
        container_mouse_wheel(dist);
    }

    template<class Config, bool With_layout>
    void Container<Config, With_layout>::mouse_exit()
    {
        container_mouse_exit();
        Widget_t::mouse_exit();
    }

    template <class Config, bool With_layout>
    void Container<Config, With_layout>::render(Canvas_t *r, const Position &offs)
    {
        // TODO: paint background, borders

        auto pos = offs + position();

        for (auto& child : children())
        {
            child->render(r, pos);
        }
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    void Abstract_container_Layouter<Config, true>::init_children_layout()
    {
        for (auto child : p()->children())
        {
            child->init_layout();
        }
    }

    template<class Config>
    inline void Abstract_container_Layouter<Config, true>::layout_children()
    {
        for (auto child : p()->children())
        {
            // EXPERIMENTAL: obtain minimum size and extend accordingly
            auto min_ext = child->minimal_size(), cur_ext = child->extents();
            if (child->extents().w == 0 && child->extents().h == 0)
            {
                child->set_extents({ std::max(min_ext.w, cur_ext.w), std::max(min_ext.h, cur_ext.h) });
            }
            child->layout();
        }
    }

    // Container Layouter aspect ------------------------------------

    template <class Config>
    template<class Aspect_parent>
    inline void Container_Layouter<Config, true>::Aspect<Aspect_parent>::init_layout()
    {
        for (auto child: p()->children())
        {
            child->init_layout();
        }
    }

    template <class Config>
    template<class Aspect_parent>
    inline void Container_Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        p()->layout_children();
    }

    // Container_updater aspect -------------------------------------

    template<class Config, bool With_layout>
    template<class Aspect_parent>
    inline void Default_Container_Container_updater<Config, With_layout>::Aspect<Aspect_parent>::child_invalidated(Widget_t *)
    {
        p()->container()->child_invalidated(p());
    }

} // ns cppgui