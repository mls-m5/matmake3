
module;

// This is a workaround that is needed for virtual classes to work properly in
// clang
import <new>;

export module base;

export struct Base {
    virtual void hello() = 0;
    virtual ~Base() = default;
};
