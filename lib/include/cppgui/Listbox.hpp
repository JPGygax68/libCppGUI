#pragma once

#include "./Boxed.hpp"
#include "./Scrolling_container.hpp"
#ifndef CPPGUI_EXCLUDE_UI_BUILDER
#include "./UI_builder.hpp"
#endif


namespace cppgui {
    
    /*
     * Specialization of Content_pane: a container intended for use as a list of items.
     */
    class Listbox_pane: public Content_pane
    {
    public:

        void add_item(Widget *);

        void render(Canvas *, const Point &offs) override;

        void set_focus_on_child(Widget *) override;

    private:
        using Super = Content_pane;

        auto separator_width() const { return 1; }
        auto separator_color(Widget_states) const -> RGBA { return { 0.6f, 0.6f, 0.6f, 1 }; }

    #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:
        auto get_minimal_bbox() -> Bbox override;
        void compute_layout(Bbox_cref b) override;
        auto get_minimal_window() -> Extents override;

    #endif // !CPPGUI_EXCLUDE_LAYOUTING

    };


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

        auto listbox_pane() const -> Listbox_pane *;

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


#ifndef CPPGUI_EXCLUDE_UI_BUILDER

    /*
    template<class ParentBuilderT>
    class UI_builder<Listbox_base, ParentBuilderT>: public UI_builder_base2<Listbox_base, ParentBuilderT>
    {
    public:
        using UI_builder_base2<Listbox_base, ParentBuilderT>::UI_builder_base2;

        template<class WidgetT>
        void _add_new_child(WidgetT *child)
        {
            this->widget().add_item(child);
        }

    private:
        int _row_index = 0;
        int _col_index = 0;
    };
    */

    template<class ParentBuilderT>
    class UI_builder<Listbox, ParentBuilderT>: public UI_builder_base2<Listbox, ParentBuilderT>
    {
    public:
        //using UI_builder_base2<Listbox, ParentBuilderT>::UI_builder_base2;

        UI_builder(Listbox& lb, Widget_bag &wb, ParentBuilderT *pb): 
            UI_builder_base2{lb, wb, pb} 
        {}

        template<class WidgetT>
        void _add_new_child(WidgetT *child)
        {
            this->widget().add_item(child);
        }

    private:
        int _row_index = 0;
        int _col_index = 0;
    };


#endif // !CPPGUI_EXCLUDE_UI_BUILDER

} // ns cppgui