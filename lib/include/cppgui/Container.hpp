#pragma once

#include "./Container_base.hpp"
#include "./Box_model.hpp"

namespace cppgui
{
    // Internals ----------------------------------------------------

    template<class Config>
    struct _container
    {
        // TODO: put into two-tiered form

        template<class Class, bool With_layout, class Parent> struct Layouter;

        template<class Class, class Parent> 
        struct Layouter<Class, false, Parent>: Parent {};

        template<class Class, class Parent> 
        struct Layouter<Class, true, Parent>: Parent
        {
            using Container_base_t = Container_base<Config, true>;

            auto get_minimal_size() -> Extents override;
            void layout() override;

            template<class ManagerType> auto set_layout_manager() -> ManagerType *
            {
                _manager.reset( new ManagerType{} ); 
                //_manager->set_padding(this->_padding); // TODO: should padding really be a member of Container ?

                return static_cast<ManagerType*>( _manager.get() ); // as a convenience
            };
            auto layout_manager() { return _manager.get(); }

            auto compute_minimal_size() -> Extents;
            void layout_children(const Extents &);

        private:
            auto p() { return static_cast<Class*>(this); }

            std::unique_ptr<typename layouting<Config>::Manager> _manager;
        };
    };

    // Public class -------------------------------------------------

    template<class Config, bool With_layout, Box_model_definition BMDef, class Layouter>
    class Container: public 
        Layouter::template Aspect< Container<Config, With_layout, BMDef, Layouter>,
        Box_model<Config, With_layout, BMDef>::template Aspect< Container<Config, With_layout, BMDef, Layouter>,
        Container_base<Config, With_layout> > >
    {};

} // ns cppgui