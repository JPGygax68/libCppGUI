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
     */

    struct Nil_struct {}; // end-of-chain for aspects

    template <class Aspect_parent>
    struct Nil_aspect : public Aspect_parent {};

    /** The following is the equivalent of std::conditional<>, but for aspects.
        std::conditional<> cannot be used for the simple reason that it works with fully
        defined types; select_aspect<> works with one-parameter templates.

        select_aspect<> makes it possible to inject completely different implementations
        of a feature
     */

    template<bool B, template <class = Nil_aspect> class Aspect1, template <class = Nil_aspect> class Aspect2>
    struct select_aspect { template <class Aspect_parent = Nil_aspect> using aspect = Aspect1<Aspect_parent>; };

    template<template <class> class Aspect1, template <class> class Aspect2>
    struct select_aspect<false, Aspect1, Aspect2> { template <class Aspect_parent = Nil_aspect> using aspect = Aspect2<Aspect_parent>; };

    /** This is a convenience macro to slightly reduce the amount of typing required to
        define an aspect, and possibly to make them stand out more clearly.
        Usage:

        CPPGUI_ASPECT(My_aspect) {
            // ... declaration / inline implementations
        };

        THIS IS EXPERIMENTAL: it is not clear how much help this is, if any.
     */

    #define CPPGUI_ASPECT(name) template <class Aspect_parent> struct name: public Aspect_parent

} // ns cppgui