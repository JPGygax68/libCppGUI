#pragma once

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

#include <map>

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

        auto adapt_resource  (Backend *backend, const SourceType &src) -> MappedType
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

        void release_resource(Backend *b, const SourceType &)
        {
            static_cast<Impl*>(this)->release(backend, _map.find(src).second);

            //_laundry.emplace_back(src);
            // TODO: immediate release
        }

    private:
        std::map<SourceType, MappedType>    _map;
        //std::vector<SourceType>             _laundry;
    };

} // ns cppgui
