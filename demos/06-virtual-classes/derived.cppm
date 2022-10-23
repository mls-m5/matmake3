
export module derived;

import <iostream>;
export import base;

export class Derived : public Base {
public:
    void hello() override {
        std::cout << "hello from derived\n";
    }
};
