#include <ostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

int main(int argc, char** argv) {

    std::string filename = "track_candidates.csv";
    if (argc > 1) {
        filename = argv[1];
    }

    std::ifstream csvFile(filename.c_str());

    std::vector<std::vector<uint32_t>> trackCandidates;
    std::string line;
    while(std::getline(csvFile, line)){
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<uint32_t> trackCandidate;
        // std::cout << "candidates: ";
        while(std::getline(lineStream, cell, ',')){
            uint32_t cellId = 0;
            try {
                cellId = std::stoi(cell);
            } catch (const std::invalid_argument& ia) {
                std::cout << "Invalid argument: " << ia.what() << " for cell " << cell << std::endl;
                continue;
            }
            // std::cout << cellId << " ";
            if (std::find(trackCandidate.begin(), trackCandidate.end(), cellId) == trackCandidate.end()) {
                trackCandidate.push_back(cellId);
            }
        }
        // std::cout << std::endl;
        trackCandidates.push_back(trackCandidate);
    }

    // print the track candidates.
    std::for_each(trackCandidates.begin(), trackCandidates.end(),
        [](const auto& innerVector){
        std::copy(innerVector.begin(), innerVector.end(), std::ostream_iterator<uint32_t>(std::cout, ","));
        std::cout << std::endl;
    });


  return 0;
}