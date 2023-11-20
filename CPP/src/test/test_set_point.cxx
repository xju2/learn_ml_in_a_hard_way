#include <set>
#include <iostream>

using namespace std;
class A{
public:
	int a;
public:
	A(){}
	virtual ~A() {}
	virtual int get_a() {return a;}
};

class B : public A {
public:
	int b;
public:
	B(){}
	~B(){}
	int get_a(){return b;}
	int get_b(){return get_a();}
};

class B2 : public A {
public:
	int b2;
public:
	B2() {}
	~B2 () {}
	int get_b2() { return b2; }
};

class C {
public:
	C() {}
	virtual ~C(){}
	void add(A& a) const {
		const A* p_a(&a);
		set<const A*>::const_iterator old = m_sets.find(p_a);
		if(old != m_sets.end()){
			cout << p_a << " is here: " << *old << endl;
		} else {
			A* p_a_nonConst = const_cast<A*>(p_a);
			m_sets.insert(p_a_nonConst);
			cout << p_a << " is added, " << p_a_nonConst << endl;
		}

	}
private:
	mutable std::set<const A*> m_sets;
};

int main()
{
	B b1 = B();
	B b2 = B();

	C c1 = C();
	c1.add(b1);
	c1.add(b2);
	for(int i = 0; i < 3; i ++) {
		B b = B();
		c1.add(b);
	}

	return 0;
}
