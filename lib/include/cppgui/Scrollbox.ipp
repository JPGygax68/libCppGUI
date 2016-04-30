#include "./Scrollbox.hpp"

namespace cppgui {

    // Main class -----------------------------------------

    template<class Config, bool With_layout, class Pane>
    Scrollbox<Config, With_layout, Pane>::Scrollbox()
    {
        _vert_sbar.set_focussable(false); // TODO: should a scrollbar be focussable in the first place ?

        /* _vert_sbar.on_position_change([this](const Fraction<> &pos) {

            _content->set_position({ 0, - static_cast<Position>((_content->extents().h - extents().h) * pos.num / pos.den) });
            invalidate();
        }); */
        
        _vert_sbar.on_navigation([this](Navigation_unit unit, Position initial_pos, const Fraction<int> &delta) {

            static_cast<Pane*>(_content)->scroll(unit, initial_pos, delta);
        });

        add_child(&_vert_sbar);
    }

    template<class Config, bool With_layout, class Pane>
    void Scrollbox<Config, With_layout, Pane>::init()
    {
        // Must be done first, otherwise scrollbar will produce division by zero
        _vert_sbar.define_values(_content->extents().h, extents().h);

        Container_t::init(); // will also init scrollbar
    }

    template<class Config, bool With_layout, class Pane>
    void Scrollbox<Config, With_layout, Pane>::on_navigation(Navigation_handler handler)
    {
        _on_navigation = handler;
    }

    template<class Config, bool With_layout, class Pane>
    void Scrollbox<Config, With_layout, Pane>::set_content_pane(Scrollable_pane_t *content)
    {
        assert(!_content);

        _content = content;
        _content->set_border({1, Color{ 1, 1, 0, 1 }});

        add_child(content);
    }

    template<class Config, bool With_layout, class Pane>
    void Scrollbox<Config, With_layout, Pane>::mouse_wheel(const Vector &delta)
    {
        // TODO: better way than just redirecting input events ?
        _vert_sbar.mouse_wheel(delta);
    }

    template<class Config, bool With_layout, class Pane>
    void Scrollbox<Config, With_layout, Pane>::render(Canvas_t *canvas, const Point &offset)
    {
        Rectangle r { rectangle() };

        canvas->set_clipping_rect(r.pos.x, r.pos.y, r.ext.w, r.ext.h);

        Container_t::render(canvas, offset);

        canvas->cancel_clipping();

        draw_borders(canvas, offset, 1, {1, 0.2f, 0, 1});
    }

    // Layouter aspect ------------------------------------

    template<class Config, class Pane>
    template<class Aspect_parent>
    auto Scrollbox__Layouter<Config, true, Pane>::Aspect<Aspect_parent>::get_minimal_size() -> Extents
    {
        // Somewhat arbitrary: width: twice that of scrollbar, height: scrollbar

        auto sb_minsz = p()->_vert_sbar.get_minimal_size();

        return { 2 * sb_minsz.w, sb_minsz.h };
    }

    template<class Config, class Pane>
    template<class Aspect_parent>
    void Scrollbox__Layouter<Config, true, Pane>::Aspect<Aspect_parent>::layout()
    {
        auto exts = p()->extents();

        auto sb_minsz = p()->_vert_sbar.get_minimal_size();

        // TODO: use configurable or stylable width for the scrollbar
        p()->_vert_sbar.set_position({ exts.right() - static_cast<Position_delta>(sb_minsz.w), 0 });
        p()->_vert_sbar.set_extents ({ sb_minsz.w, exts.h });

        // Content rectangle
        p()->_content_rect = { 0, 0 , exts.w - sb_minsz.w, exts.h }; // TODO: border, padding ?

        // Content pane: adjust width, leave height untouched
        p()->_content->set_extents({ exts.w - p()->_vert_sbar.extents().w, p()->_content->extents().h });

        // TODO: the following must also be done in the main aspect when the content pane changes
        // TODO: better yet, this should be done at init() time ?
        //p()->_vert_sbar.define_values(p()->_content->extents().h, exts.h);

        p()->_vert_sbar.layout();

        p()->_content->layout();
    }

    template<class Config, class Pane>
    template<class Aspect_parent>
    auto Scrollbox__Layouter<Config, true, Pane>::Aspect<Aspect_parent>::content_rect() -> Rectangle
    {
        return { 
            0, 0, 
            p()->extents().w - p()->_vert_sbar.extents().w, p()->extents().h
        };
    }

} // ns cppgui