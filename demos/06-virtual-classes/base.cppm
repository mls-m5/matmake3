
export module base;

export struct Base {
    virtual void hello() = 0;
    virtual ~Base() = default;
};
