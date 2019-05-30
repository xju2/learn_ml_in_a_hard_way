#include "test_address_lamdba.h"
#include <iostream>

double func1(double x) {
	return  x;
}

void func2(double x) {
	return ;
}
struct EFF_CONST {
	EFF_CONST(double x): _x(x) {}
	double operator () (double xx ) {return _x; }
	double get(double xx) {return _x;}
	double _x;
};

using namespace std;
int main() {
	typedef function<double(double )> FuncStorage;
	typedef function<void(double )> VoidFuncStorage;
    FuncStorage s1(func1);
	VoidFuncStorage s2(func2);
	EFF_CONST eff(0.8);
	FuncStorage s3(eff);
	cout << "Efficiency: " << eff.get(33) << endl;
	cout << get_address(s1) << " " << get_address(s2) << endl;
	cout << "s1: " << reinterpret_cast<uintptr_t>(&s1) << endl;
	cout << "f1: " << reinterpret_cast<uintptr_t>(&func1) << endl;
	cout << "f2: " << reinterpret_cast<uintptr_t>(&func2) << endl;
	cout << "address of Eff: " << get_address(s3) << endl;
	cout << reinterpret_cast<uintptr_t>(&eff) << endl;
	return 0;
}
