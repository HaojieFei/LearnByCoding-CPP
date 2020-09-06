//
// test2.cpp
//
//

#include <memory>
#include <iostream>
#include "LeakDetector.hpp"

class A;
class B;
class A {
public:
    std::shared_ptr<B> p;
};
class B {
public:
    std::shared_ptr<A> p;
};

int main() {

    auto smartA = std::make_shared<A>();
    auto smartB = std::make_shared<B>();
    smartA->p = smartB;
    smartB->p = smartA;

    return 0;

}
