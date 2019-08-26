#include <iostream>
#include <map>

using namespace std;

int a1[] = {11, 12};
int a2[] = {21, 22};

void change(int* aa) {
	aa = a1;
}

void change2(int* & aa) {
	aa = a1;
}

int main()  {
	int array[5] = {0, 1, 2, 3, 4};
	int* p_array = array;
	cout  << p_array[1] << endl;

	int a = 0, b = 1;
	int* parray[2];
	parray[0] = &a;
	parray[1] = &b;

	int** pparray = parray;
	cout << *(pparray[0]) << endl;

	map<int, string> m_map[2];
	m_map[0][0] = "HELLO";
	m_map[1][0] = "GOOD";
	m_map[0][1] = "HELLO-1";
	m_map[1][1] = "GOOD-1";

	map<int, string> *p_map;
	p_map = m_map;
	cout << p_map[0][1] << endl;

	int * anull = nullptr;
	change(anull);
	cout << anull[0] << endl;

	int * anull2 = nullptr;
	change2(anull2);
	cout << anull2[0] << endl;

	return 0;
}
