#include "./Resource_mapper.hpp"

namespace cppgui {

    template<typename Impl, typename Backend, typename SourceType, typename MappedType>
    inline auto Resource_mapper<Impl, Backend, SourceType, MappedType, true>::map(Backend * b, const SourceType & src) -> MappedType &
    {
        auto it = _map.find(src);
        if (it == std::end(_map))
        {
            it = _map.emplace({ src, Impl::map(b, src) });
        }
        return it->second;
    }

    template<typename Impl, typename Backend, typename SourceType, typename MappedType>
    inline void Resource_mapper<Impl, Backend, SourceType, MappedType, true>::release(const SourceType & src)
    {
        _laundry.emplace_back(src);
    }

    template<typename Impl, typename Backend, typename SourceType, typename MappedType>
    inline void Resource_mapper<Impl, Backend, SourceType, MappedType, true>::do_laundry(Backend * b)
    {
        for (auto &src : _laundry) release_resource(b, _map[src]);
        _laundry.clear();
    }

} // ns cppgui