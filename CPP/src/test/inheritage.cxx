#include <iostream>

using namespace std;
class A {
public:
	A():m_id(3) { 
		cout <<"In A() constructor" << endl;
	}
	~A() {}
	int id() { return m_id;}
	void set_id(int id) { m_id = id; }

private:
	int m_id;
};

class B: public A {
public:
	B():m_bb(4) { }
	B(const A& a, int bb):A(a),m_bb(bb) {
	}

public:
	int m_bb;
};

int main() {
	A a;
	B b;
	cout << b.m_bb << " " << b.id() << endl;
	a.set_id(44);
	B c(a, 21);
	cout << c.m_bb << " " << c.id() << endl;

	return 0;
}
