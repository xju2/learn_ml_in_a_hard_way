#include <string>
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

class ExaTrkXTrackFinding {
  public:
    struct Config {
		std::string inputMLModuleDir;
	};

	ExaTrkXTrackFinding(Config config): m_cfg(std::move(config))
	{}

	void operator()() {
		std::cout <<"configuration directory: " << m_cfg.inputMLModuleDir << std::endl;
	}

  private:
	Config m_cfg;
};


int main() {
	size_t pid  = 11;
	std::vector<std::string> my_analyses;
	my_analyses.push_back("Jack");
	my_analyses.push_back("Alice");
	my_analyses.push_back("Bob");
	
	PointConfig a = {pid, my_analyses};


	std::cout << a << std::endl;

	ExaTrkXTrackFinding::Config config;
	config.inputMLModuleDir = "/home/xju/ocean/code/Tracking-ML-Exa.TrkX/Pipelines/TrackML_Example/onnx_models";

	ExaTrkXTrackFinding exatrkx(config);
	exatrkx();

	return 0;
}
