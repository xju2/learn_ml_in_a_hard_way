// Write an example for using the unique_ptr to hold different objects
// and check if all objects are deleted properly.

#include <iostream>
#include <memory>
#include <vector>

class Base {
public:
    Base() {
        data = 10;
        std::cout << "Base default" << std::endl;
    }
    Base(int d) {
        data = d;
        std::cout << "Base with data " << data << std::endl;
    }
    Base(const Base& other) {
        data = other.data;
        std::cout << "Base copy constructor: " << data << std::endl;
    }
    ~Base() {
        std::cout << "Base destructor: " << data << std::endl;
    }
    int data;
};

std::unique_ptr<Base> createBase(int d) {
    return std::make_unique<Base>(d);
}

int main() {

    std::unique_ptr<Base> ptr1 = std::make_unique<Base>();
    if (true) {
        int b2 = 20;
        ptr1 = std::make_unique<Base>(b2);
    }
    if (true) {
        int b3 = 30;
        ptr1 = createBase(b3);
    }
    std::cout << "ptr1 data: " << ptr1->data << std::endl;

    std::vector<Base*> vec;
    vec.push_back(ptr1.release());

    std::cout << "vec size: " << vec.size() << ", with data: " << vec[0]->data << std::endl;
    std::cout << "ptr1 data: " << ptr1->data << std::endl;

    return 0;
}
