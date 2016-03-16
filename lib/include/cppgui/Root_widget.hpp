#pragma once

#include "./Container.hpp"

namespace cppgui {

    extern int dummy;

    template <class Config, bool WithLayout>
    class Root_widget_basic {

    };

    /** TODO: for now, Root_widget inherits from Container. This has drawbacks, mainly
            because Container has some features that the Root_widget does not need (for
            example, Root_widget does not need a position, nor should it compute a
            minimal size or do layouting. Right now, the corresponding abstract virtual
            functions are implemented with dummies, but that is not elegant: better
            to acknowledge that the root widget is special, and give it its own
            implementations of the few methods that it could inherit from container.
     */
    template <class Config, bool WithLayout>
    class Root_widget: public Config::Root_widget_update_handler, public Container<Config, WithLayout> {
    public:
        Root_widget();

        // TODO: the following belong into the Root_widget_full class (following the usual splitting scheme)
        auto minimal_size() -> Extents override { return { 0, 0 }; };
        void layout() override {};

        // TODO: do we need non-inherited event injection methods ?
    };

} // ns cppgui