#include <iostream>
#include "ioc_container.hpp"
class IX {
public:
    virtual void fun() = 0;
};
class X : public IX {
public:
    void fun() override {
        std::cout << "I am X fun!" << std::endl;
    }    
};
int main() {
    ioc_container ic;
    ic.register_type<X>("xx");
    auto aa = ic.resolve<X>("xx");  // return a object pointer
    aa->fun();
    delete aa;

    auto bb = ic.resolve_uniqueptr<X>("xx");    // return a unique ptr of the object
    bb->fun();

    return 0;
}