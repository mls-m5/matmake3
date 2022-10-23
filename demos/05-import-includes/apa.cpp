// A workaround when you want to import iostream and string is to only import
// string, otherwise clang will throw a mysterious error

// #include <iostream>

import <iostream>;
//  import <string>;

int print() {
    std::cout << "hello" << std::endl;
    std::string str = "satnhoeu";
    return 10;
}
