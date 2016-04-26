#include "./Scrollbox.hpp"

namespace cppgui {

    // Main class -----------------------------------------

    template<class Config, bool With_layout>
    Scrollbox<Config, With_layout>::Scrollbox()
    {
        add_child(&_vert_sbar);
    }

    template<class Config, bool With_layout>
    void Scrollbox<Config, With_layout>::set_content(Container_t *content)
    {
        assert(!_content);

        _content = content;
        add_child(content);
    }

    template<class Config, bool With_layout>
    void Scrollbox<Config, With_layout>::render(Canvas_t *canvas, const Point &offset)
    {
        Container_t::render(canvas, offset);

        draw_borders(canvas, offset, 1, {1, 0.2f, 0, 1});
    }

    // Layouter aspect ------------------------------------

    template<class Config>
    template<class Aspect_parent>
    void Scrollbox__Layouter<Config, true>::Aspect<Aspect_parent>::layout()
    {
        auto exts = p()->extents();

        p()->_content->set_extents({ exts.w, p()->_content->extents().h });

        p()->_vert_sbar.set_position({ exts.right() - 20, 0 });
        p()->_vert_sbar.set_extents ({ 20, exts.h });

        // TODO: the following must also be done in the main aspect when the content pane changes
        p()->_vert_sbar.define_range(p()->_content->extents().h, exts.h);

        p()->_vert_sbar.layout();

        p()->_content->set_extents({ exts.w - 20, p()->_content->extents().h });
        p()->_content->layout();
    }

} // ns cppgui