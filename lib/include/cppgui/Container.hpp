#pragma once

#include "./Container_base.hpp"

namespace cppgui
{
    // Internals ----------------------------------------------------

    template<class Config>
    struct _container
    {
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

    template<class Config, bool With_layout, template<class> class BoxModel>
    class Container: public 
        _container<Config>::template Layouter<Container<Config, With_layout, BoxModel>, With_layout,
            Box<Config, With_layout,
                BoxModel<
                    Container_base<Config, With_layout> > > >
    {

    };

} // ns cppgui