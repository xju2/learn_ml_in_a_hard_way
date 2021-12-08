#include <vector>
#include <functional>
#include <iostream>
 
struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { std::cout << num_+i << '\n'; }
    int num_;
};
 
class Foo2 {
public:
    Foo2(int num) : num_(num) {}
    
    void print_add(std::vector<int> inValues) const { 
        for(auto x: inValues){
            std::cout << num_+x << ' '; 
        }
        std::cout << std::endl;
    }

    void print_add2(std::vector<std::vector<int> >& inValues) const {
        std::cout << "in print_add2\n";
        for(auto x: inValues){
            for(auto y: x)
                std::cout << num_+y << ' '; 
            std::cout << std::endl;
        }
    }

    void print_add3(std::vector<uint32_t>& first, std::vector<uint32_t>& middle, std::vector<std::vector<uint32_t> >& inValues) const {
        std::cout << "in print_add3\n";
        for(auto x: inValues){
            for(auto y: x)
                std::cout << num_+y<< ' '; 
            std::cout << std::endl;
        }
        for(auto x: middle) std::cout << x << " ";
        std::cout << "\n";
    }

private:
    int num_;
};
 
int main()
{
    const Foo foo(314159);
    // store a call to a data member accessor
    std::function<int(Foo const&)> f_num = &Foo::num_;
    std::cout << "num_: " << f_num(foo) << '\n';
 
    // store a call to a member function and object
    using std::placeholders::_1;
    std::function<void(int)> f_add_display2 = std::bind( &Foo::print_add, foo, std::placeholders::_1 );
    f_add_display2(2);
 
    Foo2 foo2(314159);
    // store a call to a member function and object ptr
    std::function<void(std::vector<int>&)> f_add_display3 = std::bind( &Foo2::print_add, &foo2, std::placeholders::_1);
    std::vector<int> results{5, 3};
    f_add_display3(results);

    std::function<void(std::vector<std::vector<int> >&)> f_add_display4 = std::bind(\
        &Foo2::print_add2, &foo2, std::placeholders::_1);

    std::vector<int> results2{5, 3};
    std::vector<std::vector<int> > allRes;
    allRes.push_back(results);
    allRes.push_back(results2);
    f_add_display4(allRes);


    std::function<void(std::vector<uint32_t>&, std::vector<uint32_t>&, std::vector<std::vector<uint32_t> >&)> f_add_display5;
    
    f_add_display5 = std::bind(
        &Foo2::print_add3, &foo2, std::placeholders::_1,  std::placeholders::_2,  std::placeholders::_3);

    std::vector<uint32_t> jj{5, 3};
    std::vector<uint32_t> jj2{5, 3};
    std::vector<std::vector<uint32_t> > allUnit{jj, jj2};
    f_add_display5(jj2, jj, allUnit);
}