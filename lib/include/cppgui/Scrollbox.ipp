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

        draw_border(canvas, offset); //, _border.width, _border.color);

        fill_rect(canvas, { _vert_sep_pos, (Position) _border.width, _separator.width, extents().h - 2 * _border.width }, 
            r.pos, canvas->rgba_to_native(_separator.color) );
    }

    // Layouter aspect ------------------------------------

    template<class Config, class Pane, class Parent>
    auto Scrollbox__Layouter<Config, true, Pane, Parent>::get_minimal_size() -> Extents
    {
        // Somewhat arbitrary: width: twice that of scrollbar, height: scrollbar

        auto sb_minsz = p()->_vert_sbar.get_minimal_size();

        return { 2 * sb_minsz.w, sb_minsz.h };
    }

    template<class Config, class Pane, class Parent>
    void Scrollbox__Layouter<Config, true, Pane, Parent>::layout()
    {
        /** TODO: this whole algorithm must also be accessible outside of the layouting aspect,
                because it should be triggered whenever the pane notifies a change in size.
         */

        // Preparations

        auto exts = p()->extents();
        auto vertsb_minsz = p()->_vert_sbar.get_minimal_size();
        //auto horzsb_minsz = p()->_horz_sbar.get_minimal_size();

        // Try without scrollbars first, then add scrollbars as needed

        //bool have_horz_sbar = false;    // TODO: future: may be forced on
        bool have_vert_sbar = false;    // ditto

        Extents cont_exts; // output of this algorithm

        for ( ;; )
        {
            // Compute container extents
            cont_exts = { exts.w - 2 * p()->_border.width, exts.h - 2 * p()->_border.width };
            if (have_vert_sbar) cont_exts.w -= p()->_separator.width + vertsb_minsz.w;
            //if (horz_scrollbar) cont_exts.h -= p()->_separator.width + horzsb_minsz.w;

            // Ask content pane to adjust and check its resulting extents
            p()->_content->compute_and_set_extents( cont_exts );
            auto pane_exts = p()->_content->extents();

            if      (pane_exts.h > cont_exts.h && !have_vert_sbar) have_vert_sbar = true;
            //else if (pane_exts.w > cont_exts.w && !have_horz_sbar) have_horz_sbar = true;
            else 
                break; // done
        }

        // Enable the scrollbars we need
        p()->_vert_sbar.set_visible( have_vert_sbar );
        //p()->_horz_sbar.set_visible( have_horz_sbar );

        // Position and layout the components:

        // Content pane and its containing rectangle
        p()->_content_rect = Rectangle { Point { (Position) p()->_border.width, (Position) p()->_border.width }, cont_exts };
        p()->_content->set_position({ p()->_content_rect.pos.x, p()->_content_rect.pos.y });
        p()->_content->layout(); // usually won't do much (already done by compute_and_set_extents()

        // Scrollbars and their separators
        if (have_vert_sbar)
        {
            p()->_vert_sep_pos = exts.w - p()->_border.width - vertsb_minsz.w - p()->_separator.width;
            p()->_vert_sbar.set_position({ exts.right() - (Position_delta) (vertsb_minsz.w + p()->_border.width), 0 });
            p()->_vert_sbar.set_extents ({ vertsb_minsz.w, exts.h });
            p()->_vert_sbar.layout();
        }
        /* if (have_horz_sbar)
        {
            p()->_horz_sep_pos = exts.h - p()->_border.width - sb_minsz.h - p()->_separator.width;
            p()->_horz_sbar.set_position({ exts.bottom() - (Position_delta) (vertsb_minsz.h + p()->_border.width), 0 });
            p()->_horz_sbar.set_extents ({ exts.w, horzsb_minsz.h });
            p()->_horz_sbar.layout();
        } */
    }

} // ns cppgui