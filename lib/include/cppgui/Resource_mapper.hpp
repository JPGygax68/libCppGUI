#pragma once

#include <map>
#include <vector>
#include <type_traits>
#include <functional>

namespace cppgui {

    /** A Resource_mapper is responsible for managing resources that need to be passed to 
        a backend. Depending on the type of the resource and the way a particular backend
        works, the mapping can be anywhere between completely unnecessary (for example, the 
        OpenGL rendering backend can use color definitions in Rgba_float format exactly as 
        they are) and rather expensive (such as the same backend using images, which it
        needs to translate to textures, which must later be deleted to free video or driver
        resources).

        Resource_mapper is implemented as a specializable, curiously-recurring class template.
     */
    template <typename Impl, typename Backend, typename SourceType, typename MappedType>
    struct Resource_mapper_concept {

        // TODO: use concepts to ensure that Impl provides the required methods
        static_assert(std::is_copy_constructible<SourceType>::value, "Resource_mapper: SourceType must be copy-constructible");
        static_assert(std::is_copy_constructible<MappedType>::value, "Resource_mapper: MappedType must be copy-constructible");

        // Outside interface (implemented by specializations, see below):

        auto get_resource(Backend *b, const SourceType &src) -> MappedType;
            /** Translates the specified source resource for use by the specified
                backend. */

        void release_resource(const SourceType &);
            /** Release a resource obtained via get_resource().
             */

        // To be implemented by descendant:

        auto map_resource(Backend *, const SourceType &) -> MappedType;
            /** Called by get_resource(). This must use the backend (or some associated
                API/object) to translate the specified source resource into a 
                representation that the backend can use directly.
             */
    };

    /** "Dummy" resource mapper that simply returns the resource as it is, unchanged.
        Can be used, for example, to "map" colors in RGBA float format for use by the
        OpenGL renderer, which uses that format natively.
     */
    template <typename Backend, typename ResourceType>
    struct Identity_mapper {
        auto get_resource(Backend *, const ResourceType &res) { return res; }
        void release_resource() {} // no-op, of course
        //void release_all_resources(Backend *) {} // even more so
    };

} // ns cppgui