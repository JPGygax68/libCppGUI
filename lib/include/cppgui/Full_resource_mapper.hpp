#pragma once

#include <map>
//#include <vector>

#include "./Resource_mapper.hpp"

namespace cppgui {

    /** The following implementation brings in the "full" functionality:
        resources are translated for the backend by calling map_resource(),
        and stored for later re-use in a std::map that uses the source resource
        type as the key (comparing by value must be supported, which is automatic
        if the source type is a pointer).
        Also, all mapped resources are reference-counted. TODO!
    */
    template <typename Impl, typename Backend, typename SourceType, typename MappedType>
    class Full_resource_mapper: public Resource_mapper_concept<Impl, Backend, SourceType, MappedType>
    {
    public:

        auto adapt_resource  (Backend *b, const SourceType &src) -> MappedType;

        void release_resource(Backend *b, const SourceType &);

    private:
        std::map<SourceType, MappedType>    _map;
        //std::vector<SourceType>             _laundry;
    };

} // ns cppgui
