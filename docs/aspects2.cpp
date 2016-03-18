#include <iostream>
#include <string>

struct Nil_aspect {};

template <template <class = Nil_aspect> class Aspect1, template <class = Nil_aspect> class Aspect2>
struct A : public Aspect1<Aspect2<>> {

    int a_val = 1000;
};

template <class NextAspect>
struct A1 : public NextAspect {
    int a1_val = 100;
    void say_hi();
};

template <class NextAspect>
struct A2 : public NextAspect {
    int a2_val = 101;
    void say_hello() { std::cout << "Hello, this is A2" << std::endl; }
};

template <template <class = Nil_aspect> class Aspect1, template <class = Nil_aspect> class Aspect2>
struct B : public A<Aspect1, Aspect2> {

    int b_val = 2000;
};

template <class NextAspect>
struct B1 : public A1<NextAspect> {
    int b1_val = 200;
    void say_hi();
};

template <class NextAspect>
struct B2 : public A2<NextAspect> {
    int b2_val = 201;
};

//-----------------------------

using _A = A<A1, A2>;

template <class NextAspect>
void A1<NextAspect>::say_hi()
{
    std::cout << "Hi, this is A1, value is " << static_cast<_A*>(this)->a_val << std::endl;
}

using _B = B<B1, B2>;

template <class NextAspect>
void B1<NextAspect>::say_hi()
{

    // Can we convert to B ?
    auto pb = static_cast<_B*>(this);
    std::cout << "b_val = " << pb->b_val << std::endl;
}

int main()
{
    A<A1, A2> a;
    a.say_hi();

    B<B1, B2> b;
    b.say_hi();

    std::cout << std::endl << "Press RETURN to terminate" << std::endl;
    char dummy; std::cin >> std::noskipws >> dummy;

    return 0;
}