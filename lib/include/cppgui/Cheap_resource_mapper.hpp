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