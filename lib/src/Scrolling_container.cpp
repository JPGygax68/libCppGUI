#include <layouting.hpp>
#include <Scrolling_container.hpp>


namespace cppgui {
    
    auto Content_pane::container() const -> Scrolling_container *
    {
        return dynamic_cast<Scrolling_container*>(Container_base::container());
    }

    Scrolling_container::Scrolling_container(Content_pane *p):
        _content_pane(p)
    {
        using namespace std::placeholders;

        _vscrollbar.on_navigation(bind(&Scrolling_container::navigate, this, _1, _2));

        add_child(_content_pane);
        add_child(&_vscrollbar);

        set_initial_focus(_content_pane);
    }

    void Scrolling_container::compute_view_from_data()
    {
        // TODO: use items counts (visible, total) instead ?
        _vscrollbar.define_sizes(_content_pane->height(), _window_bbox.height());

        Super::compute_view_from_data();
    }

    void Scrolling_container::render(Canvas *c, const Point &offs)
    {
        auto p = offs + position();

        // Scrollbar
        _vscrollbar.render(c, p);
        
        // Separator
        auto r = Rectangle{_window_bbox};
        r.pos.x += r.ext.w;
        r.ext.w = separator_width();
        c->fill_rect(r + p, separator_color(visual_states()));

        // Content pane (clipped)
        c->push_clipping_rect(Rectangle{_window_bbox} + p);
        _content_pane->render(c, p);
        c->pop_clipping_rect();
    }

    bool Scrolling_container::key_down(const Keycode &key)
    {
        if (is_down(key))
        {
            navigate(element, {1});
            return true;
        }
        if (is_up(key))
        {
            navigate(element, {-1});
            return true;
        }
        if (is_page_down(key))
        {
            navigate(page, {1});
            return true;
        }
        if (is_page_up(key))
        {
            navigate(page, {-1});
            return true;
        }

        return Super::key_down(key);
    }

    bool Scrolling_container::mouse_wheel(const Vector &v)
    {
        navigate(element, -v.y);
        return true;
    }

    void Scrolling_container::bring_item_into_view(Index i)
    {
        bring_item_into_view(content_pane()->child(i));
    }

    void Scrolling_container::bring_item_into_view(Widget *c)
    {
        auto r_item = c->rectangle() + content_pane()->position();
        auto r_win = Rectangle{_window_bbox};

        Vector d{0, 0};

        if      (r_item.y1() < r_win.y1()) d.y = r_win.y1() - r_item.y1();
        else if (r_item.y2() > r_win.y2()) d.y = r_win.y2() - r_item.y2();

        if (d.x != 0 || d.y != 0) shift_content_by(d);

        #ifdef NOT_DEFINED
        if (! r_win.fully_contains(r_item))
        {
            // Top-left (with standard coordinate system)
            // Define rectangle operation(s) to handle this
            Point d{ 0, 0 };
        #ifdef CPPGUI_Y_AXIS_DOWN
            if      (r_item.x2() > r_win.x2()) d.x = r_win.x2() - r_item.x2();
            else if (r_item.x1() < r_win.x1()) d.x = r_win.x1() - r_item.x1();
            if      (r_item.y2() > r_win.y2()) d.y = r_win.y2() - r_item.y2();
            else if (r_item.y1() < r_win.y1()) d.y = r_win.y1() - r_item.y1();
            shift_content_by(d);
        #else
        #error Upward Y axis not supported yet
        #endif
        }
        #endif // NOT_DEFINED
    }

    /*
    void Scrolling_container::gained_focus()
    {
        Super::gained_focus();
        set_focus_on_child(content_pane());
    }
    */

    auto Scrolling_container::item_rectangle(Widget *item) const -> Rectangle
    {
        return item->rectangle() + content_pane()->position();    
    }

    auto Scrolling_container::focused_item_index() const -> Index
    {
        auto item = content_pane()->focused_child(); // TODO: replace with focused_item() 
        return item != nullptr ? item_index(item) : -1;
    }

    void Scrolling_container::shift_content_by(const Point &d)
    {
        content_pane()->shift_by(d);
        _content_offset += d;
        update_scrollbar();
    }

    void Scrolling_container::update_scrollbar()
    {
        // We only handle the vertical scrollbar for now
        _vscrollbar.update_thumb_position(-_content_offset.y);
    }

    auto Scrolling_container::separator_color(Widget_states) const -> RGBA
    {
        return { 0.5f, 0.5f, 0.5f, 1 };
    }

#ifndef CPPGUI_EXCLUDE_LAYOUTING

    auto Scrolling_container::get_minimal_bounds() -> Bbox
    {
        auto bbox = Bbox{ _content_pane->get_minimal_window(), left, top };
        bbox.append_to_right( separator_width() );
        bbox.append_to_right(_vscrollbar.get_minimal_bounds(), top);
        return bbox;
    }

    void Scrolling_container::compute_layout(Bbox_cref b_)
    {
        auto b{b_};

        // Scrollbar at right edge
        _vscrollbar.set_bounds(layout_element_at_right_edge(b, _vscrollbar.get_minimal_bounds(), vertical_baseline));

        // Separator (gap for now - TODO)
        leave_gap_at_right_edge(b, separator_width());

        // The remainder of the bounding box becomes the window box
        _window_bbox = b;

        // Expand vertically (but not horizontall) to obtain the content pane dimensions
        // (i.e.: the content window and content pane have same width, but the content
        // pane may be higher)
        b.expand_to({0, _content_pane->get_optimal_bounds().height()}, left, top);

        // Align content pane with top left of content window
        _content_pane->set_bounds(-b.origin(), b, left, top);
        _content_offset = { 0, 0 };
    }

    auto Scrolling_container::get_optimal_bounds() -> Bbox
    {
        auto bbox = Bbox{ _content_pane->get_optimal_bounds().extents(), left, top };
        bbox.append_to_right( separator_width() );
        bbox.append_to_right(_vscrollbar.get_minimal_bounds(), top);
        return bbox;
    }

#endif // !CPPGUI_EXCLUDE_LAYOUTING

} // ns cppgui