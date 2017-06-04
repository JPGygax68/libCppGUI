#include <cppgui/Grid_container.hpp>


namespace cppgui
{

#ifndef CPPGUI_EXCLUDE_LAYOUTING

    void Grid_container::add_child_cell(Widget *w)
    {
        add_child(w);
        _cell_instructions.push_back(child_cell);
    }

    void Grid_container::end_row()
    {
        _cell_instructions.push_back(end_of_row);
    }

    auto Grid_container::get_minimal_bbox() -> Bbox
    {
        uint i_r = 0, i_c = 0;
        auto item_index = 0;
        for (auto& ci: _cell_instructions)
        {
            if (i_r >= _rows.size()) _rows.push_back({});
            auto& row = _rows[i_r];

            if (ci == child_cell)
            {
                auto b = child(item_index)->get_minimal_bbox();
                // Update row
                // TODO: make the following to lines into a method of Bbox
                to_max(row.y_max, b.y_max);
                to_min(row.y_min, b.y_min);
                // Update column
                if (_columns.size() <= i_c) _columns.push_back({});
                auto& col = _columns[i_c];
                to_min(col.x_min, b.x_min);
                to_max(col.x_max, b.x_max);
                i_c ++;
                item_index ++;
            }
            else if (ci == end_of_row)
            {
                i_r ++;
                i_c = 0;
            }
            // else ... TODO
        }

        auto x_min = _columns[0].x_min, x_max = _columns[0].x_max;
        for (uint i = 1; i < _columns.size(); i ++) x_max += SPACING + _columns[i].width();
        auto y_max = _rows[0].y_max, y_min = _rows[0].y_min;
        for (uint i = 1; i < _rows.size(); i ++) y_min -= SPACING + _rows[i].height();

        return {x_min, x_max, y_min, y_max};
    }

    void Grid_container::compute_layout(Bbox_cref bbox)
    {
        uint i_r = 0, i_c = 0;
        auto item_index = 0;
        auto x = 0, y = 0;
        for (auto& ci: _cell_instructions)
        {
            auto& row = _rows[i_r];

            if (ci == child_cell)
            {
                auto& col = _columns[i_c];
                child(item_index)->set_bounds({{x, y}, {col.x_min, col.x_max, row.y_min, row.y_max}}); // TODO: alignments
                x += col.x_max;
                i_c ++;
                if (i_c < _columns.size()) x += SPACING -_columns[i_c].x_min;
                item_index ++;
            }
            else if (ci == end_of_row)
            {
                y += - row.y_min;
                x = 0;
                i_r ++;
                i_c = 0;
                if (i_r < _rows.size()) y += SPACING + _rows[i_r].y_max;
            }
            // else ... TODO
        }

    }

#endif // !CPPGUI_EXCLUDE_LAYOUTING

} // ns cppgui