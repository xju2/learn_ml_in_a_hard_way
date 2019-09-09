#include <iostream>

using namespace std;
class A {
public:
	A() {}
	~A() {}
	void f() {
		cout << "A::f()" << endl;
	}
};

class B: public A {
public:
	B() {}
	~B() {}
	void f() {
		cout << "B::f()" << endl;
	}
};

class AV {
public:
	virtual ~AV(){}
	virtual void f() {
		cout << "AV::f()" << endl;
	}
};

class BV: public AV {
public:
	~BV(){}
	void f() {
		cout << "BV::f()" << endl;
	}
};

int main() {
	A a;
	B b;
	a.f();
	b.f();

	A* aa = new B();
	B* bb = new B();
	aa->f();
	bb->f();
	delete aa;

	AV* av = new BV();
	av->f();
	delete av;

	return 0;
}
