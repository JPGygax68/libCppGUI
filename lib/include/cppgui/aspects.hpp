#pragma once

namespace cppgui {

    // Inherit_with_aspects

    template<class Class, template<class> class ...Aspects> class Inherit_with_aspects;

    template<class Class, template<class> class Aspect>
    class Inherit_with_aspects<Class, Aspect>: public Aspect< Class > {};

    template<class Class, template<class> class FirstAspect, template<class> class ...OtherAspects>
    class Inherit_with_aspects<Class, FirstAspect, OtherAspects...>: public FirstAspect< Inherit_with_aspects<Class, OtherAspects...> > {};

    /* template<template<class Config, bool With_layout, class Parent> class Aspect_, class Config, bool With_layout>
    struct Aspect_specializer {
        template<class Parent> using Aspect = Aspect_<Config, With_layout, Parent>;
    }; */

    //#define ASPECT(Name, Config, With_layout) typename Aspect_specializer<Name, Config, With_layout>::Aspect

} // ns cppgui