#include <stdlib.h>
#include <fstream>
int main() {

	std::fstream fs("random_test.txt", std::fstream::out);
	int i = 0;
	while(i < 10000) {
		fs << rand()/static_cast<double>(RAND_MAX) << "\n";
		i++;
	}
	fs.close();
	return 0;
}
