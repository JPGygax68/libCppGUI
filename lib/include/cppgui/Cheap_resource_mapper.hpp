#pragma once

#include "./Resource_mapper.hpp"

namespace cppgui {

    /** The following implementation of the Resource_mapper concept is for "inexpensive"
    resource translation: while the output type is different from the input type, output 
    objects do not require being kept after their use, as re-generating them does not consume
    significant amounts of cycles.
    Consequently, this implementation of the concept does not keep a registry of
    mapped resources.
    */
    template <typename Impl, typename Backend, typename SourceType, typename MappedType>
    struct Cheap_resource_mapper : public Resource_mapper_concept<Impl, Backend, SourceType, MappedType>
    {
        auto get_resource(Backend *b, const SourceType &src) -> MappedType { return Impl::map_resource(b, src); }

        void release_resource(const SourceType &) {} // no-op

        void do_laundry(Backend *) {} // no-op

        void release_all_resources(Backend *) {} // no-op
    };

} // ns cppgui