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
        _vscrollbar.define_sizes(_content_pane->height(), _content_bbox.height());
    }

#ifndef CPPGUI_EXCLUDE_LAYOUTING

    auto Scrolling_container::get_minimal_bounds() -> Bounding_box
    {
        // TODO: this is probably wrong, because it will try to align the baseline of the "up" button of the scrollbar
        //  with the baseline of the first list item
        auto bbox = _content_pane->get_minimal_bounds();
        bbox.append_to_right(_vscrollbar.get_minimal_bounds(), top);
        return bbox;
    }

    void Scrolling_container::compute_layout(const Bounding_box &b_)
    {
        auto b{ b_ };

        // Scrollbar at right edge
        _vscrollbar.set_bounds(layout_element_at_right_edge(b, _vscrollbar.get_minimal_bounds(), vertical_baseline));

        // Content pane gets the rest
        _content_pane->set_bounds({0, 0}, b);

        // We store the content window so we can check item visibility
        _content_bbox = b;
    }

#endif // !CPPGUI_EXCLUDE_LAYOUTING

} // ns cppgui