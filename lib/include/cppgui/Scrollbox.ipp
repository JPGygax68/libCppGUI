#include "./Scrollbox.hpp"

namespace cppgui {

    // Main class -----------------------------------------

    template<class Config, bool With_layout>
    Scrollbox<Config, With_layout>::Scrollbox()
    {
        _vert_sbar.set_focussable(false);

        _vert_sbar.on_position_change([this](const Fraction<> &pos) {

            _content->set_position({ 0, - static_cast<Position>((_content->extents().h - extents().h) * pos.num / pos.den) });
            invalidate();
        });

        add_child(&_vert_sbar);
    }

    template<class Config, bool With_layout>
    void Scrollbox<Config, With_layout>::on_navigation(Navigation_handler handler)
    {
        _on_navigation = handler;
    }

    template<class Config, bool With_layout>
    void Scrollbox<Config, With_layout>::set_content_pane(Container_t *content)
    {
        assert(!_content);

        _content = content;
        _content->set_border({1, Color{ 1, 1, 0, 1 }});

        add_child(content);
    }

    template<class Config, bool With_layout>
    void Scrollbox<Config, With_layout>::mouse_wheel(const Vector &delta)
    {
        // TODO: better way than just redirecting input events ?
        _vert_sbar.mouse_wheel(delta);
    }

    template<class Config, bool With_layout>
    void Scrollbox<Config, With_layout>::render(Canvas_t *canvas, const Point &offset)
    {
        Rectangle r { rectangle() };

        canvas->set_clipping_rect(r.pos.x, r.pos.y, r.ext.w, r.ext.h);

        Container_t::render(canvas, offset);

        canvas->cancel_clipping();

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