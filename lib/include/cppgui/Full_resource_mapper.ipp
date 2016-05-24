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

#include "./Full_resource_mapper.hpp"

namespace cppgui {

    template<typename Impl, typename Backend, typename SourceType, typename MappedType>
    inline auto Full_resource_mapper<Impl, Backend, SourceType, MappedType>::adapt_resource(Backend * backend, const SourceType &src) -> MappedType
    {
        auto it = _map.find(src);
        if (it != std::end(_map))
        {
            return it->second;
        }
        else {
            auto insert = _map.emplace(src, static_cast<Impl*>(this)->obtain(backend, src));
            return insert.first->second;
        }
    }

    template<typename Impl, typename Backend, typename SourceType, typename MappedType>
    void Full_resource_mapper<Impl, Backend, SourceType, MappedType>::release_resource(Backend *backend, const SourceType &src)
    {
        static_cast<Impl*>(this)->release(backend, _map.find(src).second);

        //_laundry.emplace_back(src);
        // TODO: immediate release
    }

#ifdef NOT_DEFINED

    template<typename Impl, typename Backend, typename SourceType, typename MappedType>
    void Full_resource_mapper<Impl, Backend, SourceType, MappedType, true>::do_laundry(Backend * b)
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
    void Full_resource_mapper<Impl, Backend, SourceType, MappedType, true>::release_all_resources(Backend * b)
    {
        for (auto &entry : _map) static_cast<Impl*>(this)->release(b, entry.second);
        _map.clear();
    }

#endif

} // ns cppgui
