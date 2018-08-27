#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

int add(int x, int y){
	return x+y;
}

int main()
{
    std::vector<int> c = {1, 2, 3, 4, 5, 6, 7};
    int x = 5;
    c.erase(std::remove_if(c.begin(), c.end(), [x](int n) { return n < x; }), c.end());
 
    std::cout << "c: ";
    std::for_each(c.begin(), c.end(), [](int i){ std::cout << i << ' '; });
    std::cout << '\n';
 
    // the type of a closure cannot be named, but can be inferred with auto
    // since C++14, lambda could own default arguments
    auto func1 = [](int i = 6) { return i + 4; };
    std::cout << "func1: " << func1() << '\n';
 
    // like all callable objects, closures can be captured in std::function
    // (this may incur unnecessary overhead)
    std::function<int(int)> func2 = [](int i) { return i + 4; };
    std::cout << "func2: " << func2(6) << '\n';


	// nested functions, using bind
	using namespace std::placeholders;  // for _1, _2, _3...
	std::function<int(int)> add5 = std::bind(add, 5, _1);
	auto add5_auto = std::bind(add, 5, _1);
	std::cout <<  add5(5) << std::endl;
	std::cout <<  add5_auto(5) << std::endl;


	// nested function, using lambda
	auto adder = [](int x) {
		auto adderX = [x](int y) {
			return x+y;
		};
		return adderX;
	};
	auto add5_lambda = adder(5);
	std::cout <<  add5_lambda(5) << std::endl;
}
