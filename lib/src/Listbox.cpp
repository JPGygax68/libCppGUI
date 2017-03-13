#include <cassert>
#include <layouting.hpp>
#include <Listbox.hpp>


namespace cppgui {

    // Private implementation of Content_pane contract ------------------------

    class Listbox_content_pane: public Content_pane
    {

    #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:
        auto get_minimal_bounds() -> Bounding_box override;
        void compute_layout(const Bounding_box &b) override;

    #endif // !CPPGUI_EXCLUDE_LAYOUTING

    };

    auto Listbox_content_pane::get_minimal_bounds() -> Bounding_box
    {
        auto bbmin = Bounding_box::empty();

        for (auto child: children())
        {
            bbmin.merge(child->get_minimal_bounds());
        }

        return bbmin;
    }

    void Listbox_content_pane::compute_layout(const Bounding_box &b_)
    {
        auto b{ b_ };

        for (auto child: children())
        {
            child->set_bounds(layout_element_at_top_edge(b, child->get_minimal_bounds(), horizontal_middle));
        }
    }


    // Listbox_base implementation --------------------------------------------

    Listbox_base::Listbox_base():
        Scrolling_container(new Listbox_content_pane())
    {
    }

    void Listbox_base::add_item(Widget *item) const
    {
        content_pane()->add_child(item);
    }

    void Listbox_base::navigate(Navigation_unit unit, const Fraction<int> &delta)
    {
        if (items().empty()) return;

        if (unit == element)
        {
            assert(delta.den == 1);
            scroll_by_items(delta.num);
        }
    }

    void Listbox_base::scroll_by_items(int d)
    {
        while (d != 0)
        {
            if (d < 0)
            {
                scroll_up();    // content pane moves down relative to window
                d ++;
            }
            else
            {
                scroll_down();  // content pane moves up relative to window
                d --;
            }
        }
    }

    bool Listbox_base::item_fully_visible(Index index) const
    {
        return content_window().contains_full_height_of( item_rectangle(item(index)) );
    }

    bool Listbox_base::first_item_fully_visible() const
    {
        assert(!items().empty());

        return item_fully_visible(0);
    }

    bool Listbox_base::last_item_fully_visible() const
    {
        assert(!items().empty());

        return item_fully_visible(item_count() - 1);
    }

    auto Listbox_base::first_partially_visible_item_index() const -> Index
    {
        auto r_win = content_window();

        for (Index i = 0; i < static_cast<Index>(items().size()); i ++)
        {
            auto r_item = item_rectangle(item(i));

            if (r_win.intersects_vertically_with(r_item))
                return i;
        }

        return -1;
    }

    auto Listbox_base::first_partially_visible_item() const -> Widget *
    {
        return item( first_partially_visible_item_index() );
    }

    auto Listbox_base::hidden_height_of_first_visible_item() const -> Length
    {
        auto r_item = item_rectangle(first_partially_visible_item());

        return content_window().y1() - r_item.y1();
    }

    auto Listbox_base::item_rectangle(Widget *item) const -> Rectangle
    {
        return item->rectangle() + content_pane()->position();    
    }

    void Listbox_base::bring_item_into_view(Index index)
    {
        // Find relative position of item to bring into full view
        auto r = item_rectangle( item(index) );
        // TODO: limit to try and prevent uncovering empty space at the bottom
        auto dy = r.y1() - content_window().y1();

        // Shift
        content_pane()->shift_by({0, -dy});
    }

    void Listbox_base::scroll_down()
    {
        // "scroll down" = content pane moves up relative to content window

        if (!last_item_fully_visible())
        {
            auto i = first_partially_visible_item_index();
            auto r = item_rectangle( item(i) );

            // If item fully visible, pick the successor
            if (r.y1() >= content_window().y1()) i ++;

            bring_item_into_view(i);
        }
    }

    void Listbox_base::scroll_up()  
    {
        // "scroll up" = content pane moves down relative to content window, uncovering predecessor items

        auto i = first_partially_visible_item_index();

        // If item fully visible, pick predecessor
        if (item_fully_visible(i)) i --;

        if (i >= 0)
        {
            bring_item_into_view(i);
        }
    }

} // ns cppgui