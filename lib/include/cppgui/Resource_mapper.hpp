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
            /** Schedule the release of the "translated" representation of the specified
                "source" resource by a later call to do_laundry(). 
                This call exists so that resources can be released from code that
                does not have access to the backend.
                TODO: it is not clear yet whether that is really necessary or even a good
                    idea. It could be useful with OpenGL contexts, where there is a 
                    possibility that the context must be shared between threads.
                    Also, it is possible that a resource could be released just to be
                    re-mapped right away - in that case, immediate release would rob us
                    of the option to prevent an unnecessary re-mapping of the resource.
             */

        void do_laundry(Backend *);
            /** This is where the resources scheduled for release (by calls to release_resource())
                are actually released via the backend.
             */

        void release_all_resources(Backend *);
            /** This is called when the GUI library stops using the backend.
                TODO: decide whether this is necessary at all, or the purpose would be
                    better served by a debug-build runtime check in the destructor.
             */
               
        // To be implemented by descendant:

        auto map_resource(Backend *, const SourceType &) -> MappedType;
            /** Called by get_resource(). This must use the backend (or some associated
                API/object) to translate the specified source resource into a 
                representation that the backend can use directly.
             */
    };

    template <typename Impl, typename Backend, typename SourceType, typename MappedType, bool expensive>
    struct Resource_mapper {
    };

    /** The following partial specialization is for "inexpensive" resource translation:
        while the output type is different from the input type, output objects do not 
        require being kept after their use, as re-generating them does not consume 
        significant amounts of cycles.
        Consequently, all registry-related calls are implemented as no-ops, and 
        get_resource() is just a pass-thru to map_resource().
     */
    template <typename Impl, typename Backend, typename SourceType, typename MappedType>
    struct Resource_mapper<Impl, Backend, SourceType, MappedType, false>: 
        public Resource_mapper_concept<Impl, Backend, SourceType, MappedType> 
    {
        auto get_resource(Backend *b, const SourceType &src) -> MappedType { return Impl::map_resource(b, src); }

        void release_resource(const SourceType &) {} // no-op

        void do_laundry(Backend *) {} // no-op

        void release_all_resources(Backend *) {} // no-op
    };

    /** The following partial specialization brings in the "full" functionality:
        resources are translated for the backend by calling map_resource(),
        and stored for later re-use in a std::map that uses the source resource
        type as the key (comparing by value must be supported, which is automatic
        if the source type is a pointer).
     */
    template <typename Impl, typename Backend, typename SourceType, typename MappedType>
    struct Resource_mapper<Impl, Backend, SourceType, MappedType, true>:
        public Resource_mapper_concept<Impl, Backend, SourceType, MappedType> 
    {
        auto get_resource(Backend *b, const SourceType &src) -> MappedType;

        void release_resource(const SourceType &);

        void do_laundry(Backend *b);

        void release_all_resources(Backend *b);

    private:
        std::map<SourceType, MappedType>    _map;
        std::vector<SourceType>             _laundry;
    };

    /** "Dummy" resource mapper that simply returns the resource as it is, unchanged.
        Can be used, for example, to "map" colors in RGBA float format for use by the
        OpenGL renderer, which uses that format natively.
     */
    template <typename Backend, typename ResourceType>
    struct Identity_mapper {
        auto get_resource(Backend *, const ResourceType &res) { return res; }
        void release_resource() {} // no-op, of course
        void release_all_resources(Backend *) {} // even more so
    };

} // ns cppgui