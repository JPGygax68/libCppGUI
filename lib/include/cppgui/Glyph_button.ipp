/*  libCppGUI - A GUI library for C++11/14
    
    Copyright 2016 Hans-Peter Gygax

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "./Glyph_button.hpp"

namespace cppgui {

    // Main class implementation ------------------------------------

    template<class Config, bool With_layout, template<class> class BoxModel>
    void Glyph_button<Config, With_layout, BoxModel>::on_pushed(Pushed_handler handler)
    {
        _on_pushed = handler;
    }

    template<class Config, bool With_layout, template<class> class BoxModel>
    void Glyph_button<Config, With_layout, BoxModel>::set_glyph(const Icon_glyph &icgl)
    {
        _glyph_font.assign( gpc::fonts::get(icgl.data_store) );
        _glyph_cp  = icgl.code_point;
    }

    template<class Config, bool With_layout, template<class> class BoxModel>
    void Glyph_button<Config, With_layout, BoxModel>::init()
    {
        if (_label_font.source()) _label_font.translate( this->root_widget()->canvas() );

        if (_glyph_cp)
        {
            _glyph_font.translate( this->root_widget()->canvas() );
        }
    }

    template<class Config, bool With_layout, template<class> class BoxModel>
    void Glyph_button<Config, With_layout, BoxModel>::render(Canvas_t *cnv, const Point & offset)
    {
        // Background
        fill(cnv, offset, this->rgba_to_native( this->button_face_color() ));

        auto pos = offset + this->position();

        this->draw_border(cnv, pos);

        // Label
        if (!_label.empty())
        {
            cnv->render_text(_label_font.get(), pos.x + _label_pos.x, pos.y + _label_pos.y, _label.data(), _label.size());
        }

        // Glyph
        if (_glyph_cp)
        {
            cnv->render_text(_glyph_font.get(), pos.x + _glyph_pos.x, pos.y + _glyph_pos.y, &_glyph_cp, 1);
        }

        if (this->has_focus())
        {
            // TODO: draw the rectangle along the border instead of around the label ?
            auto &r = _focus_rect; // _label_rect.grow({3, 3});
            cnv->draw_stippled_rectangle_outline(pos.x + r.pos.x, pos.y + r.pos.y, r.ext.w, r.ext.h, {0, 0, 0.5f, 1});
        }
    }

    template<class Config, bool With_layout, template<class> class BoxModel>
    void Glyph_button<Config, With_layout, BoxModel>::mouse_click(const Point & /*point*/, int button, Count /*count*/)
    {
        if (_on_pushed && button == 1) _on_pushed();
    }

    // Layouter aspect ----------------------------------------------

    // TODO: 1) write a layout class that can handle a label + a glyph  2) use it here

    template <class Class, class Parent>
    void Glyph_button__Layouter<Class, true, Parent>::init_layout()
    {
        compute_sizes();
    }

    template <class Class, class Parent>
    void Glyph_button__Layouter<Class, true, Parent>::compute_sizes()
    {
        if (p()->_glyph_cp)
        {
            _glyph_bounds = p()->_glyph_font.source()->lookup_glyph(0, p()->_glyph_cp)->cbox.bounds;
            _glyph_min_edge = std::max(_glyph_bounds.width(), _glyph_bounds.height());
        }
        else {
            _glyph_bounds = {};
            _glyph_min_edge = 0;
        }

        if (!p()->_label.empty())
        {
            _label_bounds = p()->_label_font.source()->compute_text_extents(0, p()->_label.data(), p()->_label.size());
            _spacing = _glyph_min_edge > 0 ? 4 : 0; // TODO: replace with value based on EM, or stylesheet setting
        }
        else {
            _label_bounds = {};
            _spacing = 0;
        }
    }

    template <class Class, class Parent>
    auto Glyph_button__Layouter<Class, true, Parent>::get_minimal_size() -> Extents
    {
        return this->add_boxing({ 
            _label_bounds.width () + _spacing + _glyph_min_edge, 
            std::max(_label_bounds.height(), _glyph_min_edge)  
        });
    }

    template <class Class, class Parent>
    void Glyph_button__Layouter<Class, true, Parent>::layout()
    {
        auto ext = p()->extents();

        if (!p()->_label.empty())
        {
            p()->_label_pos = { 
                this->get_distance(3), 
                ((ext.h - _label_bounds.height()) / 2) + _label_bounds.y_max 
            };

            /* p()->_label_rect = {
                p()->_label_pos.x, p()->_label_pos.y - _label_bounds.y_max,
                _label_bounds.width(), _label_bounds.height()
            }; */

            p()->_glyph_pos = {
                (ext.w - this->get_distance(1) - _glyph_min_edge),
                ((ext.h - _glyph_min_edge) / 2) + _glyph_bounds.y_max
            };

        }
        else {
            p()->_glyph_pos = {
                ((ext.w - _glyph_bounds.width ()) / 2), // - _glyph_bounds.x_min,
                ((ext.h - _glyph_bounds.height()) / 2) + _glyph_bounds.y_max
            };
        }

        // Focus rectangle
        // TODO: stylesheet control
        p()->_focus_rect = p()->extents(); // add_padding( Rectangle{ p()->extents() } );
    }

} // ns cppgui
