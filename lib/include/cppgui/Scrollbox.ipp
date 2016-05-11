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
        
        _vert_sbar.on_navigation([this](Navigation_unit unit, /* Position initial_pos, */ const Fraction<int> &delta) {

            static_cast<Pane*>(_content)->scroll(unit, /* initial_pos, */ delta);
        });

        add_child(&_vert_sbar);
    }

    template<class Config, bool With_layout, class Pane>
    void Scrollbox<Config, With_layout, Pane>::init()
    {
        Container_t::init(); // will also init scrollbar
    }

    template<class Config, bool With_layout, class Pane>
    void Scrollbox<Config, With_layout, Pane>::compute_view_from_data()
    {
        // Content pane must have its size set at this point
        _vert_sbar.define_values(_content->extents().h, _content_rect.ext.h);

        Parent_t::compute_view_from_data();
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
        //_content->set_border({1, Color{ 1, 1, 0, 1 }});

        add_child(_content);

        //_content->set_position({ (Position) _border.width, (Position) _border.width });
    }

    template<class Config, bool With_layout, class Pane>
    void Scrollbox<Config, With_layout, Pane>::mouse_wheel(const Vector &delta)
    {
        // TODO: better way than just redirecting input events ? e.g. call Impl::move_by_items() ?
        _vert_sbar.mouse_wheel(delta);
    }

    template<class Config, bool With_layout, class Pane>
    void Scrollbox<Config, With_layout, Pane>::key_down(const Keycode &code)
    {
        if      (Keyboard::is_down     (code)) pane()->scroll(Navigation_unit::element, { 1, 1});
        else if (Keyboard::is_up       (code)) pane()->scroll(Navigation_unit::element, {-1, 1});
        else if (Keyboard::is_page_down(code)) pane()->scroll(Navigation_unit::page   , { 1, 1});
        else if (Keyboard::is_page_up  (code)) pane()->scroll(Navigation_unit::page   , {-1, 1});
        else
            Parent_t::key_down(code);
    }

    template<class Config, bool With_layout, class Pane>
    void Scrollbox<Config, With_layout, Pane>::render(Canvas_t *canvas, const Point &offset)
    {
        Rectangle r { rectangle() + offset };

        canvas->set_clipping_rect(r.pos.x, r.pos.y, r.ext.w, r.ext.h);

        Container_t::render(canvas, offset);

        canvas->cancel_clipping();

        draw_borders(canvas, offset, _border.width, _border.color);

        fill_rect(canvas, { _vert_sep_pos, (Position) _border.width, _separator.width, extents().h - 2 * _border.width }, 
            r.pos, canvas->rgba_to_native(_separator.color) );
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
        p()->_vert_sbar.set_position({ exts.right() - (Position_delta) (sb_minsz.w + p()->_border.width), 0 });
        p()->_vert_sbar.set_extents ({ sb_minsz.w, exts.h });

        // Content rectangle
        p()->_content_rect = { 
            (Position) p()->_border.width, (Position) p()->_border.width, 
            exts.w - sb_minsz.w - 2 * p()->_border.width - p()->_separator.width, exts.h - 2 * p()->_border.width
        }; // TODO: padding ? separator ?

        // Vertical separator between content rectangle and vertical scrollbar
        p()->_vert_sep_pos = exts.w - p()->_border.width - sb_minsz.w - p()->_separator.width;

        // Content pane: adjust width, leave height untouched
        p()->_content->compute_and_set_extents( p()->_content_rect.ext );
        //p()->_content->set_extents({ p()->_content_rect.ext.w, p()->_content->extents().h }); // TODO: set both w and h to content rect ?

        // TODO: the following must also be done in the main aspect when the content pane changes
        // TODO: better yet, this should be done at init() time ?
        //p()->_vert_sbar.define_values(p()->_content->extents().h, exts.h);

        p()->_vert_sbar.layout();

        p()->_content->set_position({ p()->_content_rect.pos.x, p()->_content_rect.pos.y });
        p()->_content->layout(); // may modify extents set above
    }

} // ns cppgui