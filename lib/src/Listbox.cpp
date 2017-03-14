#include <cassert>
#include <layouting.hpp>
#include <Listbox.hpp>
#include <iostream>


namespace cppgui {

    // Private implementation of Content_pane contract ------------------------

    class Listbox_content_pane: public Content_pane
    {
    public:

        // TODO: optimized implementation that only draw visible items
        void render(Canvas *, const Point &offs) override;

    private:
        
        auto separator_width() const { return 1; }
        auto separator_color(Widget_states) const -> RGBA { return { 0.6f, 0.6f, 0.6f, 1 }; }

    #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:
        auto get_minimal_bounds() -> Bounding_box override;
        void compute_layout(const Bounding_box &b) override;
        auto get_minimal_window() -> Extents override;

    #endif // !CPPGUI_EXCLUDE_LAYOUTING

    };

    void Listbox_content_pane::render(Canvas *c, const Point &offs)
    {
        auto p = offs + position();

        for (auto child: children())
        {
            child->render(c, p);

            auto r = child->rectangle();
            r.pos.y += r.ext.h;
            r.ext.h = separator_width();
            c->fill_rect(r + p, separator_color(visual_states()));
        }
    }


#ifndef CPPGUI_EXCLUDE_LAYOUTING

    auto Listbox_content_pane::get_minimal_bounds() -> Bounding_box
    {
        auto bb_min = Bounding_box::empty();

        if (child_count() > 0)
        {
            for (auto child: children())
            {
                //bb_min.merge(child->get_minimal_bounds());
                bb_min.append_at_bottom(child->get_minimal_bounds(), horizontal_origin);
                // TODO: margin
            }

            bb_min.y_min -= child_count() * separator_width();
        }

        // TODO: minimal bounds if there are no children ?

        return bb_min;
    }

    void Listbox_content_pane::compute_layout(const Bounding_box &b_)
    {
        if (child_count() > 0)
        {
            auto bb_rem{ b_ };      // remaining bounding box

            // Layout vertically into the provided bounding box (can exceed at bottom)
            for (auto child: children())
            {
                auto lb = layout_element_at_top_edge(bb_rem, child->get_minimal_bounds(), horizontal_middle);
                child->set_bounds(lb);
                layout_gap_at_top_edge(bb_rem, separator_width());
            }
        }
    }

    auto Listbox_content_pane::get_minimal_window() -> Extents
    {
        Extents ext;

        for (auto child: children())
        {
            ext |= Extents{child->get_minimal_bounds()};
        }

        // TODO: support a dummy item bounding box in case the list can be initially empty

        return ext;
    }

#endif !CPPGUI_EXCLUDE_LAYOUTING


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
        else if (unit == fraction)
        {
            //std::cout << "delta (fraction): " << delta.num << "/" << delta.den << std::endl;
            auto d = delta.num * item_count() / delta.den;
            if (d != 0)
            {
                scroll_by_items(d);
            }
        }
        else if (unit == page)
        {
            assert(delta.den == 1);
            scroll_by_items(delta.num * visible_item_count());
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

    auto Listbox_base::visible_item_count() const -> Count
    {
        auto i = first_partially_visible_item_index();
        auto j = i;
        while (j < item_count() && item_fully_visible(j)) j ++;
        return j - i;
    }

    void Listbox_base::bring_item_into_view(Index index)
    {
        // Find relative position of item to bring into full view
        auto r = item_rectangle( item(index) );
        // TODO: limit to try and prevent uncovering empty space at the bottom
        auto dy = r.y1() - content_window().y1();

        // Shift
        shift_content_by({ 0, -dy });
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