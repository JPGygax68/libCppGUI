#include "./unicode.hpp"

#include "./Stringlist.hpp"

namespace cppgui {

    template<class Config>
    template<class Class, bool With_layout>
    _stringlist<Config>::Base<Class, With_layout>::Base()
    {
        add_child(&_vert_sbar);

        _vert_sbar.on_navigation([ this ](Navigation_unit unit, const Fraction<int> &delta) {

            switch (unit)
            {
            case Navigation_unit::element:
                // TODO
                break;
            case Navigation_unit::page:
                // TODO
                break;
            case Navigation_unit::fraction:
                // TODO
                break;
            case full_range: 
                // TODO
                break;
            default: 
                // TODO
                break;
            }

            //static_cast<Pane*>(_content)->scroll(unit, delta);
        });
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::set_font(const Rasterized_font * font)
    {
        _font.assign( font );
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::init()
    {
        _font.translate( this->root_widget()->canvas() );

        Container_t::init();
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::compute_view_from_data()
    {
        compute_content_rectangle();

        auto visible_items = (_content_rect.height() + _item_separator.width) / (item_height() + _item_separator.width);

        _vert_sbar.define_values(_items.size(), visible_items);

        Container_t::compute_view_from_data();
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::add_item(const std::string & text)
    {
        add_item( utf8_to_utf32(text) );
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::add_item(const std::u32string & text)
    {
        _items.push_back( text );
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::render(Canvas_t * canvas, const Point & offset)
    {
        Point pos = offset + this->position();

        // fill(canvas, offset, Canvas_t::rgba_to_native({1, 1, 1, 1}) );

        draw_border(canvas, offset);

        // Render the separator between the scrollbar and the content rectangle
        draw_vert_separator(canvas, offset, _sbar_separator, _vert_sep_pos);

        // Render the scrollbar (delegate to the container)
        Container_t::render(canvas, offset);

        // Render the items
        // TODO: clip

        // Item rectangle
        Rectangle r_item { _content_rect };
        r_item.ext.h = item_height();

        // Separator rectangle
        Rectangle r_sep { r_item };
        r_sep.pos.y += static_cast<Position_delta>(r_item.ext.h);
        r_sep.ext.h = _item_separator.width;

        // Draw all items
        for (auto i = 0U; ; )
        {
            // Draw item background
            fill_rect(canvas, r_item, pos, Canvas_t::rgba_to_native({ 1, 1, 1, 1 }));

            // Render item text
            Point p_text = r_item.pos + Extents{ _item_padding[3], _item_padding[0] + _ascent };
            Width w_text = r_item.width() - _item_padding[3] - _item_padding[1];
            canvas->render_text( _font.get(), pos.x + p_text.x, pos.y + p_text.y, _items[i].data(), _items[i].size(), w_text);

            // Done ?
            if (++i >= _items.size()) break;

            r_item.pos.y += static_cast<Position_delta>(r_item.ext.h);
            if (r_item.pos.y >= _content_rect.bottom()) break;

            // Draw item separator
            r_sep.pos.y = r_item.pos.y;
            fill_rect(canvas, r_sep, pos, Canvas_t::rgba_to_native(_item_separator.color) );

            r_item.pos.y += _item_separator.width;
            if (r_item.pos.y >= _content_rect.bottom()) break;
        }
    }

    template<class Config>
    template<class Class, bool With_layout>
    void _stringlist<Config>::Base<Class, With_layout>::compute_content_rectangle()
    {
        Length sb_width = _vert_sbar.extents().w + _sbar_separator.width;

        _content_rect = Rectangle{ this->extents() } - 
            Margins{ this->_border.width, this->_border.width + sb_width, this->_border.width, this->_border.width };
    }

    // Layouter aspect ----------------------------------------------

    template<class Config>
    template<class Class, bool With_layout>
    auto _stringlist<Config>::Base<Class, With_layout>::item_height() const
    {
        return (Length) (_item_padding[0] + _ascent - _descent + _item_padding[2]);
    }

    template<class Config>
    template<class Parent>
    void _stringlist<Config>::Layouter<true, Parent>::init_layout()
    {
        compute_text_extents();

        Parent::init_layout();
    }

    template<class Config>
    template<class Parent>
    auto _stringlist<Config>::Layouter<true, Parent>::get_minimal_size() -> Extents
    {
        /** The get_minimal_size() method is intended to be called recursively
        by containers, or from a container's layout() method to help it
        decide how to place and size its children.
        */

        return {};
    }

    template<class Config>
    template<class Parent>
    void _stringlist<Config>::Layouter<true, Parent>::layout()
    {
        // Preparations
        auto exts = p()->extents();
        auto vertsb_minsz = p()->_vert_sbar.get_minimal_size();

        // Vertical scrollbar
        p()->_vert_sep_pos = exts.w - p()->_border.width - vertsb_minsz.w - p()->_sbar_separator.width;
        p()->_vert_sbar.set_position({ 
            exts.right() - static_cast<Position_delta>(vertsb_minsz.w + p()->_border.width), 
            static_cast<Position>(p()->_border.width) 
        });
        p()->_vert_sbar.set_extents ({ vertsb_minsz.w, exts.h - 2 * p()->_border.width });
        p()->_vert_sbar.layout();

        Parent::layout();
    }

    template<class Config>
    template<class Parent>
    void _stringlist<Config>::Layouter<true, Parent>::compute_text_extents()
    {
        if (p()->_font.source())
        {
            // TODO: support other cultures
            auto bbox = p()->_font.source()->compute_text_extents(0, U"My", 2);
            p()->_ascent  = bbox.y_max;
            p()->_descent = bbox.y_min;
            //p()->_mean_char_width = (bbox.width() + 1) / 2;
        }
    }

} // ns cppgui