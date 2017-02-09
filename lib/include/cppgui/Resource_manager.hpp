#pragma once

#include <type_traits>
#include <map>

#include "./contracts.hpp"
//#include "./Resource_mapper.hpp"
//#include "./Full_resource_mapper.hpp"

namespace cppgui {

    template<typename Impl, typename SourceT, typename MappedT, bool Expensive = false, typename Enable = void>
    struct Resource_manager;

    /** This is the "dummy" specialization that will be used if the backend can use the resource as-is, 
        without any mapping. This specialization will be chosen automatically if the source and mapped
        types are identical.
    */
    template<typename Impl, typename SourceT, typename MappedT>
    struct Resource_manager<Impl, SourceT, MappedT, false, std::enable_if_t<std::is_same<SourceT, MappedT>::value>> 
    {
        constexpr static auto get_resources(SourceT resource) -> MappedT { return resource; }
        constexpr static void release_resources(MappedT handle) {} // no-op
        constexpr static void release_all_resources() {} // no-op
    };

    /** This is the "full" specialization that will be picked if resource mapping is considered
        "expensive". It will keep translated resources in a map for re-use.
     */
    template<typename Impl, typename SourceT, typename MappedT>
    struct Resource_manager<Impl, SourceT, MappedT, true, std::enable_if_t<!std::is_same<SourceT, MappedT>::value>> 
    {
        auto get_resource(SourceT resource) -> MappedT
        {
            ASSERT_METHOD(Impl, map_resource, MappedT, SourceT);

            auto it = _map.find(src);

            if (it != std::end(_map))
            {
                return it->second;
            }
            else {
                auto insert = _map.emplace(src, static_cast<Impl*>(this)->map_resource(b, src));
                return insert.first->second;
            }
        }
        
        void release_resource(MappedT handle) 
        {
            ASSERT_METHOD(Impl, unmap_resource, void, MappedT);

        }

        void release_all_resources()
        {
            // TODO: this is a potentially inefficient fallback. If the backend supports a
            //  faster way to release all resources of this type, find a way to use that.

            for (auto &rec: _map)
            {
                //static_cast<Impl*>(this)->unmap_resource(rec.second);
                this->Impl::unmap_resource(rec.second);
            }

            _map.clear();
        }

    protected:
        std::map<SourceT, MappedT> _map;
    };

} // ns cppgui
