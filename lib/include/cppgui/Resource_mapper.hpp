#pragma once

#include <map>
#include <vector>

namespace cppgui {

    template <typename Impl, typename Backend, typename SourceType, typename MappedType, bool expensive>
    struct Resource_mapper {
    };

    template <typename Impl, typename Backend, typename SourceType, typename MappedType>
    struct Resource_mapper<Impl, Backend, SourceType, MappedType, false> {

        auto get_resource(Backend *b, const SourceType &src) -> MappedType { return Impl::translate_resource(b, src); }

        void release_resource(const SourceType &) {} // no-op

        void do_laundry(Backend *) {} // no-op

        void release_all_resources(Backend *) {} // no-op
    };

    template <typename Impl, typename Backend, typename SourceType, typename MappedType>
    struct Resource_mapper<Impl, Backend, SourceType, MappedType, true> {

        auto get_resource(Backend *b, const SourceType &src) -> MappedType;

        void release_resource(const SourceType &);

        void do_laundry(Backend *b);

        void release_all_resources(Backend *b);

    private:
        std::map<SourceType, MappedType>    _map;
        std::vector<SourceType>             _laundry;
    };

    /** "Dummy" resource mapper that simply returns the resource as it is, unchanged.
        Can be used, for example, to "map" colors in RGBA float format for use by the
        OpenGL renderer, which uses that format natively.
     */
    template <typename Backend, typename ResourceType>
    struct Identity_mapper {
        auto get_resource(Backend *, const ResourceType &res) { return res; }
        void release_resource() {} // no-op, of course
        void release_all_resources(Backend *) {} // even more so
    };

} // ns cppgui