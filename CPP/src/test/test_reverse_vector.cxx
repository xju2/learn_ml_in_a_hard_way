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

    // Specify the number of elements (N)
    const size_t N = 5;

    // Create an empty vector
    std::vector<int> myVector;

    // Resize the vector to have N elements
    myVector.resize(N);

    // Access and print the elements
    std::cout << "Vector elements: ";
    for (const auto& element : myVector) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

	return 0;
}
