#include <layouting.hpp>
#include <Scrolling_container.hpp>


namespace cppgui {

    Scrolling_container::Scrolling_container(Content_pane *p):
        _content_pane(p)
    {
        using namespace std::placeholders;

        _vscrollbar.on_navigation(bind(&Scrolling_container::navigate, this, _1, _2));

        add_child(_content_pane);
        add_child(&_vscrollbar);
    }

    void Scrolling_container::compute_view_from_data()
    {
        // TODO: use items counts (visible, total) instead ?
        _vscrollbar.define_sizes(_content_pane->height(), _window_bbox.height());

        Super::compute_view_from_data();
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

#ifndef CPPGUI_EXCLUDE_LAYOUTING

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

    auto Scrolling_container::get_minimal_bounds() -> Bounding_box
    {
        // TODO: this is probably wrong, because it will try to align the baseline of the "up" button of the scrollbar
        //  with the baseline of the first list item
        auto bbox = Bounding_box{ _content_pane->get_minimal_window() };
        bbox.append_to_right(_vscrollbar.get_minimal_bounds(), top);
        return bbox;
    }

    void Scrolling_container::compute_layout(const Bounding_box &b_)
    {
        auto b{ b_ };

        // Scrollbar at right edge
        _vscrollbar.set_bounds(layout_element_at_right_edge(b, _vscrollbar.get_minimal_bounds(), vertical_baseline));

        // The remainder of the bounding box becomes the window box
        _window_bbox = b;

        // Align content pane with top left of content window
        _content_pane->set_bounds(align_top_left(_window_bbox, _content_pane->get_minimal_bounds()));
        _content_offset = { 0, 0 };
    }

#endif // !CPPGUI_EXCLUDE_LAYOUTING

} // ns cppgui