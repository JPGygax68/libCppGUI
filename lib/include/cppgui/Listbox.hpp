#pragma once

#include "./Boxed.hpp"
#include "./Scrolling_container.hpp"


namespace cppgui {
    
    class Listbox_base: public Scrolling_container 
    {
    public:

        Listbox_base();
        
        void add_item(Widget *) const;

        bool key_down(const Keycode &) override;

    protected:

        void navigate(Navigation_unit, const Fraction<int> &d) override;
        void scroll_by_items(int d);

    private:
        using Super = Scrolling_container;

        bool item_fully_visible(Index) const;
        bool first_item_fully_visible() const;
        bool last_item_fully_visible() const;
        auto first_partially_visible_item_index() const -> Index;
        auto first_partially_visible_item() const -> Widget *;
        auto last_partially_visible_item_index() const -> Index;
        auto hidden_height_of_first_visible_item() const -> Length;
        auto visible_item_count() const -> Count;

        // Specific actions (TODO: move to "public" area ?)

        void scroll_up          ();
        void scroll_down        ();
        bool selection_down     ();
        bool selection_up       ();
        bool selection_page_down();
        bool selection_page_up  ();
    }; 
    

    // TODO: define and use a template called Container_boxed<> ?
    using Listbox = Boxed<Listbox_base, Simple_frame_box_styles>;
    //using Listbox = Listbox_base;

} // ns cppgui