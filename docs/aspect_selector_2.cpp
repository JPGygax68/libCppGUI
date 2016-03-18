#include <iostream>
#include <string>

struct Nil_aspect {};

template<bool B, template <class = Nil_aspect> class Aspect1, template <class = Nil_aspect> class Aspect2>
struct select_aspect { template <class Next_aspects = Nil_aspect> using aspect = Aspect1<Next_aspects>; };

template<template <class> class Aspect1, template <class> class Aspect2>
struct select_aspect<false, Aspect1, Aspect2> { template <class Next_aspects = Nil_aspect> using aspect = Aspect2<Next_aspects>; };

template <class Config, class Next_aspect = Nil_aspect>
struct Real_layouter {
    void layout() { std::cout << "Real_layouter::layout()" << std::endl; }
};

template <class Config, class Next_aspects = Nil_aspect>
struct Dummy_layouter {
    void layout() { std::cout << "Dummy_layouter::layout()" << std::endl; }
};

struct My_config {
};

template <class Next_aspects>
using My_real_layouter = Real_layouter<My_config, Next_aspects>;
template <class Next_aspects>
using My_dummy_layouter = Dummy_layouter<My_config, Next_aspects>;

int main()
{

    select_aspect<false, My_real_layouter, My_dummy_layouter>::aspect<> layouter1;
    select_aspect<true,  My_real_layouter, My_dummy_layouter>::aspect<> layouter2;

    layouter1.layout();
    layouter2.layout();

    std::cout << std::endl << "Press RETURN to terminate" << std::endl;
    char dummy; std::cin >> std::noskipws >> dummy;

    return 0;
}