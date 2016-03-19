#pragma once

namespace cppgui {

    /** An "aspect" is a "partial class" that can be "latched" onto a class.

        Implementation-wise, this is done via "template-assisted inheritance": an aspect
        must always be defined as a templatized struct taking its ancestor struct as its only
        template parameter, i.e.:

        template <class Aspect_parent = Nil_struct>
        struct My_aspect: public Aspect_parent {
            // ...
        };

        So, although it is possible to think of aspects as being latched onto the "side"
        of the class they augment, implementation-wise this is done via simple inheritance.

        Note that the macro CPPGUI() is provided to emit the above construct.

        To provide an aspect that is templatized, use a wrapping struct, e.g.:

        template <class T, bool Support_effects>
        struct My_aspect {

            CPPGUI(Aspect)
            {
                // ...
            };
        };

        It makes sense in such as case to follow a convention and always use "Aspect" as the
        name of the aspect, and to use the real name for the wrapping struct.

        To use an aspect, simply inherit from it, passing the class it augments as its 
        template parameter:

        class My_class: public Parent_aspect_A<Parent_class> {
            // ...
        };

    */

    struct Nil_struct {}; // end-of-chain for aspects

    template <class Aspect_parent>
    struct Nil_aspect : public Aspect_parent {};

    /** The following is the equivalent of std::conditional<>, but for aspects.
        (std::conditional<> cannot be used for the simple reason that it works with fully
        defined types; select_aspect<> works with one-parameter templates.)

        select_aspect<> makes it possible, for example, to inject completely different
        implementations of a feature into a class, depending on a boolean template parameter.

        Usage:
            select_aspect<Condition, Aspect1, Aspect2>::template aspect< ... >

        (Note that the keyword "template" is syntactically required to access the output of this construct.)

        Typical example:

        template <class Config, bool With_layout>
        class My_container:
            public select_aspect<With_layout, Container_layouter, Nil_layouter>::template aspect<Widget<Config, With_layout>>
        {
            //...
        };

        Here, Container_layouter and Nil_layouter are the two aspect implementations to choose between, 
        while Widget<Config, With_layout> is the base class onto which the aspect will be "latched".

    */

    template<bool B, template <class = Nil_aspect> class Aspect1, template <class = Nil_aspect> class Aspect2>
    struct select_aspect { template <class Aspect_parent = Nil_aspect> using aspect = Aspect1<Aspect_parent>; };

    template<template <class> class Aspect1, template <class> class Aspect2>
    struct select_aspect<false, Aspect1, Aspect2> { template <class Aspect_parent = Nil_aspect> using aspect = Aspect2<Aspect_parent>; };

    /** This is a convenience macro to slightly reduce the amount of typing required to
        define an aspect, and possibly to make them stand out more clearly.
        Usage:

        CPPGUI_DEFINE_ASPECT(My_aspect)
        {
            // ... declaration / inline implementations
        };

        THIS IS EXPERIMENTAL: it is not clear how much help this is, if any.
     */

    #define CPPGUI_DEFINE_ASPECT(name) template <class Aspect_parent> struct name: public Aspect_parent

    /** The following macro serves to simplify the declaration of formal template parameters
        that are aspects, e.g.

        template <CPPGUI_ASPECT(Layouting_aspect), CPPGUI_ASPECT(Rendering_aspect)>
        class My_widget { ... };
     */

    #define CPPGUI_ASPECT(name) template <class = ::cppgui::Nil_struct> class name

} // ns cppgui