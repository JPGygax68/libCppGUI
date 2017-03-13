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
            bbmin.merge(child->get_minimal_bounds());

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

    void Listbox_base::add_item(Widget *item)
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
        Rectangle r{ content_window() };

        return r.contains_full_height_of(items()[index]->rectangle() + content_pane()->position());
    }

    bool Listbox_base::first_item_fully_visible() const
    {
        assert(!items().empty());

        return item_fully_visible(0);
    }

    bool Listbox_base::last_item_fully_visible() const
    {
        assert(!items().empty());

        return item_fully_visible(items().size() - 1);
    }

    auto Listbox_base::first_partially_visible_item_index() const -> Index
    {
        auto r_win = content_window();

        for (Index i = 0; i < static_cast<Index>(items().size()); i ++)
        {
            auto r_item = items()[i]->rectangle() + content_pane()->position();

            if (r_win.intersects_vertically_with(r_item))
                return i;
        }

        return -1;
    }

    auto Listbox_base::first_partially_visible_item() const -> Widget *
    {
        return items()[first_partially_visible_item_index()];
    }

    auto Listbox_base::hidden_height_of_first_visible_item() const -> Length
    {
        auto r_item = first_partially_visible_item()->rectangle();

        //return Rectangle{content_pane()->bounds()}.y1() - r_item.y1();
        return content_window().y1() - r_item.y1();
    }

    void Listbox_base::scroll_down()
    {
        if (!last_item_fully_visible())
        {
            auto dy = hidden_height_of_first_visible_item();

            if (dy > 0)
            {
                // TODO: the following two should go to a shift() method, where they could be optimized
                content_pane()->move_by({0, -dy});
                invalidate();
            }
            else
            {
                // TODO: separator
                content_pane()->move_by({0, - first_partially_visible_item()->height()});
                invalidate();
            }
        }
    }

    void Listbox_base::scroll_up()  
    {
        // "scroll up" = content pane moves down relative to content window

        if (!first_item_fully_visible())
        {
            auto dy = hidden_height_of_first_visible_item();


        }
    }

} // ns cppgui