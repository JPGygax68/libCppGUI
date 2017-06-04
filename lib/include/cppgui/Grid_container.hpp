#pragma once

#include "./Container.hpp"
#ifndef CPPGUI_EXCLUDE_UI_BUILDER
#include "./UI_builder.hpp"
#endif


namespace cppgui
{
    class Grid_container: public Container_base
    {
    #ifndef CPPGUI_EXCLUDE_LAYOUTING
    public:
        
        void add_child_cell(Widget *);
        void add_empty_cell();
        void end_row();
        void set_column_count(uint);

        auto get_minimal_bbox() -> Bbox override;
        void compute_layout(Bbox_cref) override;

    protected:

        static const auto SPACING = 2; // between columns and rows TODO: use styles

    private:

        static const auto MAX_INT = std::numeric_limits<int>::max();
        static const auto MIN_INT = std::numeric_limits<int>::min();

        enum Cell_instruction
        {
            child_cell,
            end_of_row,
            empty_cell
        };

        struct Row
        {
            int y_min = MAX_INT, y_max = MIN_INT;
            auto height() const { return y_max - y_min; }
        };

        struct Column
        {
            int x_min = MAX_INT, x_max = MIN_INT;
            auto width() const { return x_max - x_min; }
        };


        std::vector<Cell_instruction>   _cell_instructions;
        uint                            _col_count = 0;

        std::vector<Row>                _rows;
        std::vector<Column>             _columns;
    #endif 
    };


#ifndef CPPGUI_EXCLUDE_UI_BUILDER

    template<class ParentT>
    class UI_builder<Grid_container, ParentT>: public UI_builder_base2<Grid_container, ParentT>
    {
    public:
        using UI_builder_base2::UI_builder_base2;

        template<class WidgetT, typename... Args>
        auto add(Args &&... args) -> UI_builder<Grid_container, ParentT>&
        {
            auto w = new WidgetT{std::forward<Args>(args)...};
            internal_add(w);
            container().add_child_cell(w);
            _col_index ++;
            return *this;
        }

        auto& end_row()
        {
            container().end_row();
            _row_index ++;
            _col_index = 0;
            return *this;
        }

    private:
        int _row_index = 0;
        int _col_index = 0;
    };


#endif // !CPPGUI_EXCLUDE_UI_BUILDER

} // ns cppgui