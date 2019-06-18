#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
int main(){

	vector<int> all_seeds_container = {0, 1, 2, 3, 4};
	cout << "Elements in the container: " << endl;
	for(auto seed: all_seeds_container) {
		cout << " " << seed;
	}
	cout << endl;
	std::reverse(all_seeds_container.begin(), all_seeds_container.end());
	cout << "Elements after reverse: " << endl;
	for(auto seed: all_seeds_container) {
		cout << " " << seed;
	}
	cout << endl;

	return 0;
}
