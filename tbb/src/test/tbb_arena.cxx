// 
#include "tbb/global_control.h"
#include "tbb/task_group.h"
#include "tbb/task_arena.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

int main() {
    int poolSize = 3;
    int maxPoolSize = std::thread::hardware_concurrency();
    std::cout << "Maximum hardware supported threads: " << maxPoolSize;
    std::cout << "; And " << poolSize << " threads are requested" << std::endl;


    std::unique_ptr<tbb::global_control> m_tbbgc = \
        std::make_unique<tbb::global_control>( tbb::global_control::max_allowed_parallelism, poolSize + 1);

    tbb::task_arena m_arena{poolSize + 1};

    size_t active_val = tbb::global_control::active_value( tbb::global_control::max_allowed_parallelism );
    std::cout << "active value from global control: " << active_val << std::endl;

    return 0;
}