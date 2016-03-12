#pragma once

#include <cassert>
#include <type_traits>
#include <map>
#include <vector>

#include <gpc/gui/renderer.hpp> // TODO: other source & namespace

namespace cppgui {

    using Rgba_norm = gpc::gui::rgba_norm;

    struct Position {
        int x, y;
        auto operator + (const Position &offs) const { return Position{ x + offs.x, y + offs.y }; }
        auto operator - (const Position &offs) const { return Position{ x - offs.x, y - offs.y }; }
    };

    struct Extents {
        unsigned int w, h;
        bool contains(const Position &pos) const { 
            return pos.x >= 0 && pos.y >= 0 
                && (unsigned int) pos.x < w && (unsigned int) pos.y < h; 
        }
    };

    struct Rectangle {
        Position pos;
        Extents  ext;
        bool contains(const Position &p) const
        { 
            return ext.contains(p - pos);
        }
    };

    class Widget_base {
    public:
        auto rectangle() const { return _rect; }
        auto position() const { return _rect.pos; }
        auto extents() const { return _rect.ext; }
        void set_position(const Position &);
        void set_extents(const Extents &);

        virtual void mouse_motion(const Position &) {};

        virtual void mouse_enter();
        virtual void mouse_exit();

        bool hovered() const { return _hovered; }

    protected:
        void trigger_redraw();

        Rectangle   _rect;

        bool        _hovered = false;
    };

    enum Resource_usage_type {
        direct,                     // renderer uses resource directly
        trivially_convertible,      // conversion from resource to native representation is trivial 
                                    // (can be repeated often without significant CPU load)
        registered,                 // conversion from resource to native representation is expensive,
                                    // representations should be kept in maps
    };

    /** Concept Widget_config
     */
    struct Widget_config {

        //using Renderer = ...;

        //static const bool color_mapping_expensive = false;
        //static const bool font_mapping_expensive = true;
    };

    template <typename Backend, typename SourceType, typename MappedType, bool expensive>
    struct Resource_mapper {
    };

    template <typename Backend, typename SourceType, typename MappedType>
    struct Resource_mapper<Backend, SourceType, MappedType, false> {

        auto map(Backend *r, const SourceType &src) -> MappedType { return map_resource(r, src); }

        void release() {} // no-op

        void do_laundry(Backend *) {} // no-op
    };

    template <typename Backend, typename SourceType, typename MappedType>
    struct Resource_mapper<Backend, SourceType, MappedType, true> {

        auto map(Backend *r, const SourceType &src) -> MappedType &
        {            
            auto it = _map.find(src);
            if (it == std::end(_map))
            {
                it = _map.emplace({ src, map_resource(r, src) });
            }
            return it->second;
        }

        void release(const SourceType &src)
        {
            _laundry.emplace_back(src);
        }

        void do_laundry(Backend *r)
        {
            for (auto &src: _laundry) release_resource(r, _map[src]);
            _laundry.clear();
        }

        /* void release_all(Backend *r)
        {
            for (auto &entry: _map) release_resource(r, entry.second);
            _map.clear();
        } */

    private:
        std::map<SourceType, MappedType>    _map;
        std::vector<SourceType>             _laundry;
    };

    template <class Config>
    class Widget: public Widget_base {
    public:
        using Renderer = typename Config::Renderer;
        using Native_color = typename Renderer::native_color;
        using Font_handle = typename Renderer::font_handle;

        virtual void update_resources(Renderer *) = 0;
        virtual void render(Renderer *, const Position &offset) = 0;

    protected:
        auto rgba_to_native(const Rgba_norm &) -> Native_color;
        void fill(Renderer *r, const Native_color &);
    };

    class Widget_layouter {
    public:
        virtual auto minimal_size() -> Extents = 0;
        virtual void layout() = 0;
    };

} // ns cppgui