
export module derived;

// export import base;
//  import <iostream>;
import <iostream>;

#include "base.cppm"

export class Derived : public Base {
public:
    void hello() override {
        std::cout << "hello from derived\n";
    }
};
