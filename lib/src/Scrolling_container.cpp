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

    void Scrolling_container::navigate(Navigation_unit, const Fraction<int> &delta)
    {
        // TODO
    }

#ifndef CPPGUI_EXCLUDE_LAYOUTING

    auto Scrolling_container::get_minimal_bounds() -> Bounding_box
    {
        auto bbox = _content_pane->get_minimal_bounds();
        bbox.append_to_right(_vscrollbar.get_minimal_bounds());
        return bbox;
    }

    void Scrolling_container::compute_layout(const Bounding_box &b_)
    {
        auto b {b_ };

        // Scrollbar at right edge
        _vscrollbar.set_bounds(layout_element_at_right_edge(b, _vscrollbar.get_minimal_bounds(), vertical_baseline));

        // Content pane gets the rest
        _content_pane->compute_layout(b);


    #ifdef NOT_DEFINED
        {
            auto mbs = _vscrollbar.get_minimal_bounds();
            auto bs = b.cut_from_right(_vscrollbar.get_minimal_bounds().width());
        }
    #endif
    }

#endif // !CPPGUI_EXCLUDE_LAYOUTING

} // ns cppgui