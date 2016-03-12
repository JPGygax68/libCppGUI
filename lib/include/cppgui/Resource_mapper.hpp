#pragma once

#include <map>
#include <vector>

namespace cppgui {

    template <typename Impl, typename Backend, typename SourceType, typename MappedType, bool expensive>
    struct Resource_mapper {
    };

    template <typename Impl, typename Backend, typename SourceType, typename MappedType>
    struct Resource_mapper<Impl, Backend, SourceType, MappedType, false> {

        auto map(Backend *b, const SourceType &src) -> MappedType { return Impl::map(b, src); }

        void release() {} // no-op

        void do_laundry(Backend *) {} // no-op
    };

    template <typename Impl, typename Backend, typename SourceType, typename MappedType>
    struct Resource_mapper<Impl, Backend, SourceType, MappedType, true> {

        auto map(Backend *b, const SourceType &src)->MappedType &;

        void release(const SourceType &src);

        void do_laundry(Backend *b);

        /* void release_all(Backend *b)
        {
        for (auto &entry: _map) release_resource(b, entry.second);
        _map.clear();
        } */

    private:
        std::map<SourceType, MappedType>    _map;
        std::vector<SourceType>             _laundry;
    };

} // ns cppgui