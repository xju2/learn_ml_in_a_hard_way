#include <string>
#include <iostream>
#include <glob.h>
#include <vector>

struct MatchPathSeparator
  {
      bool operator()( char ch ) const
      {
          return ch == '/';
      }
  };

std::string basepath(const std::string& pathname){
	return std::string(
			pathname.begin(),
			std::find_if(pathname.rbegin(), pathname.rend(), MatchPathSeparator()).base()
			);
}

std::string basename(const std::string& pathname){
	return std::string(
			std::find_if(pathname.rbegin(), pathname.rend(), MatchPathSeparator()).base(),
			pathname.end()
			);
}

// Get all files from subdirectories following a certain pattern
// https://stackoverflow.com/questions/8401777/simple-glob-in-c-on-unix-system
std::vector<std::string> glob(const std::string& pat)
{
  using namespace std;
  glob_t glob_result;
  glob(pat.c_str(),GLOB_TILDE,NULL,&glob_result);
  vector<string> ret;
  for(unsigned int i=0;i<glob_result.gl_pathc;++i){
      ret.push_back(string(glob_result.gl_pathv[i]));
  }
  globfree(&glob_result);
  return ret;
}

int main(){

	std::string path("submit/000000/runPythia.cmd");
	std::cout << path << std::endl;
	std::cout << basepath(path) << std::endl;
	std::cout << basename(path) << std::endl;


	std::string indir("/global/homes/x/xju/mctuning/run/05/monojet/v3/ttbar/submit");
	std::string pfile("runPythia.cmd");
	std::string dfile("detector_config.yoda");
	for (auto f : glob(indir + "/*/" + pfile)) {
		std::cout << basepath(f)+dfile << std::endl;
	}
}
