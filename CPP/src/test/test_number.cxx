#include <string>
#include <iostream>
#include <glob.h>
#include <vector>
#include <math.h>
#include <string>

std::string dir_name(int dir_idx)
{
	int mag;
	if(dir_idx <= 1) mag = 1;
	else { 
		std::cout << log10(dir_idx);
		mag = ceil(log10(dir_idx));
		if(dir_idx/pow(10, mag) == 1) mag += 1;
	}

	std::string out("000000");
	std::cout << " is with magnitude of " << mag << " --> ";
	if(mag >= 6) {
		return std::to_string(dir_idx);
	} else {
		return out.substr(0, 6-mag)+std::to_string(dir_idx);
	}
}


int main(int argc, char** argv){
	if(argc < 2) {
		std::cout << argv[0] << " int" << std::endl;
		return 1;
	}
	int i = atoi(argv[1]);
	std::cout << i << " --> " << dir_name(i) << std::endl;	
	return 0;
}
