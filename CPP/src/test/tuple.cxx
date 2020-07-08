#include <iostream>
#include <tuple>
#include <functional>
 
std::tuple<int, int> f() // this function returns multiple values
{
    int x = 5;
    return std::make_tuple(x, 7); // return {x,7}; in C++17
}
 
int main()
{
    // heterogeneous tuple construction
    int n = 1;
    auto t = std::make_tuple(10, "Test", 3.14, std::ref(n), n);
    n = 7;
    std::cout << "The value of t is "  << "("
              << std::get<0>(t) << ", " << std::get<1>(t) << ", "
              << std::get<2>(t) << ", " << std::get<3>(t) << ", "
              << std::get<4>(t) << ")\n";

    // function returning multiple values
    int a, b;
    std::tie(a, b) = f();
    std::cout << a << " " << b << "\n";

    auto t2 = std::make_tuple(1, "Foo", 3.14);
    // index-based access
    std::cout << "(" << std::get<0>(t2) << ", " << std::get<1>(t2)
              << ", " << std::get<2>(t2) << ")\n";
    // type-based access (C++14 or later)
    std::cout << "(" << std::get<int>(t2) << ", " << std::get<const char*>(t2)
              << ", " << std::get<double>(t2) << ")\n";

	return 0;
}
