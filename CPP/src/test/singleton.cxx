#include <iostream>

class Singleton{
	int _data;
	static Singleton* _instance;
	Singleton(int data=20){
		_data = data;
	}

	public:
	static Singleton* instance(){
		if(!_instance) _instance = new Singleton();
		return _instance;
	}
	static void update(int new_data){
		delete _instance;
		_instance = new Singleton(new_data);
	}
	inline int get(){ return _data; }
};

Singleton* Singleton::_instance = 0;

int main(){
	std::cout << Singleton::instance()->get() << std::endl;
	Singleton::update(30);
	std::cout << Singleton::instance()->get() << std::endl;
	return 0;
}
