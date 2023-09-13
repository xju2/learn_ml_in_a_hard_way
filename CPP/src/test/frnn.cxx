// forward example
#include <numeric>     // std::iota
#include <algorithm>   // std::sort
#include <iostream>     // std::cout
#include <vector>
#include <random>

int main () {
    int num_sp = 10;
    // set up random number seed
    srand(0);
    std::vector<int> idx(num_sp);
    std::vector<float> dists(num_sp);
    for (int j = 0; j < 3; j++){
        dists.clear();
        for (int i = 0; i < num_sp; i++) {
            float dist = (float) rand() / RAND_MAX;
            dists[i] = dist;
        }
        std::iota(idx.begin(), idx.end(), 0);
        std::sort(idx.begin(), idx.end(), [&dists](int i1, int i2) {return dists[i1] < dists[i2];});
        for (int i = 0; i < num_sp; i++) {
            std::cout << i << " " << dists[i] << " " << idx[i] << " " << dists[idx[i]] <<  std::endl;
        }
        std::cout << "-----" << std::endl;
    }

    return 0;
}
