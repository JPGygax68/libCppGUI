#include "./Resource_mapper.hpp"

namespace cppgui {

    template<typename Impl, typename Backend, typename SourceType, typename MappedType>
    inline auto Resource_mapper<Impl, Backend, SourceType, MappedType, true>::get_resource(Backend * b, const SourceType &src) -> MappedType
    {
        auto it = _map.find(src);
        if (it != std::end(_map))
        {
            return it->second;
        }
        else {
            auto insert = _map.emplace(src, static_cast<Impl*>(this)->obtain(b, src));
            return insert.first->second;
        }
    }

    template<typename Impl, typename Backend, typename SourceType, typename MappedType>
    inline void Resource_mapper<Impl, Backend, SourceType, MappedType, true>::release_resource(const SourceType &src)
    {
        _laundry.emplace_back(src);
    }

    template<typename Impl, typename Backend, typename SourceType, typename MappedType>
    inline void Resource_mapper<Impl, Backend, SourceType, MappedType, true>::do_laundry(Backend * b)
    {
        for (auto &src : _laundry)
        {
            auto it = _map.find(src);
            release(b, it->second);
            _map.erase(it);
        }
        _laundry.clear();
    }

    template<typename Impl, typename Backend, typename SourceType, typename MappedType>
    void Resource_mapper<Impl, Backend, SourceType, MappedType, true>::release_all_resources(Backend * b)
    {
        for (auto &entry : _map) static_cast<Impl*>(this)->release(b, entry.second);
        _map.clear();
    }

} // ns cppgui