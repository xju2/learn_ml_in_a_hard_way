
#include <vector>
#include <iostream>

struct PointConfig
{
	size_t pid;
	std::vector<std::string> analyses;
};

inline std::ostream& operator<<(std::ostream& ost, PointConfig const& pc)
{
	  ost << pc.pid ;
	  for(auto analysis: pc.analyses) {
		  ost << " " << analysis;
	  }
	  return ost;
}


int main() {
	size_t pid  = 11;
	std::vector<std::string> my_analyses;
	my_analyses.push_back("Jack");
	my_analyses.push_back("Alice");
	my_analyses.push_back("Bob");
	
	PointConfig a = {pid, my_analyses};


	std::cout << a << std::endl;
	return 0;
}
