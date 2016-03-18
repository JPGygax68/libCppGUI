#include <iostream>
#include <string>

struct Nil_aspect {};

/** We declare and pass the Aspects as unspecialized templates here, so that we can
    string them together via in the inheritance specifier.
 */
template <template <class = Nil_aspect> class Aspect1, template <class = Nil_aspect> class Aspect2>
struct A: public Aspect1<Aspect2<>> {
    
};

template <class NextAspect>
struct A1: public NextAspect {
    int a1_val = 100;
    void say_hi() { std::cout << "Hi, this is A1" << std::endl; }
};

template <class NextAspect>
struct A2: public NextAspect {
    int a2_val = 101;
    void say_hello() { std::cout << "Hello, this is A2" << std::endl; }
};

template <template <class = Nil_aspect> class Aspect1, template <class = Nil_aspect> class Aspect2>
struct B: public A<Aspect1, Aspect2> {
    
};

template <class NextAspect>
struct B1: public A1<NextAspect> {
    int b1_val = 200;
    void say_hi() { 
        std::cout << "Hi, this is B1, total is " << (b1_val + this->a1_val) << std::endl; 
    }
};

template <class NextAspect>
struct B2: public A2<NextAspect> {
    int b2_val = 201;
};

int main()
{
    //A<A1, A2> a;
    //a.say_hi();

    B<B1, B2> b;
    b.say_hi();
    
    return 0;
}