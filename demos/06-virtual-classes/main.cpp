
import derived;
import <memory>;
import <iostream>;

int main(int argc, char *argv[]) {
    auto derived = std::make_shared<Derived>();
    std::cout << "hello from main" << std::endl;
    derived->hello();

    Base &b = *derived;
    b.hello();

    return 0;
}
