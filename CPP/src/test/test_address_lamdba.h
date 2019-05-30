#include <functional>
#include <iostream>

using namespace std;

template<typename T, typename... U>
inline uintptr_t get_address(std::function<T(U...)> f) {
	typedef T(fnType)(U...);
	fnType ** fnPointer = f.template target<fnType*>();
	return (fnPointer != nullptr) ? reinterpret_cast<uintptr_t>(*fnPointer) : 0;
}
