#include <functional>
#include <iostream>
#include <queue>
#include <string_view>
#include <vector>

template<typename T>
void pop_println(std::string_view rem, T& pq)
{
    std::cout << rem << ": ";
    for (; !pq.empty(); pq.pop())
        std::cout << pq.top() << ' ';
    std::cout << '\n';
}

template<typename T>
void pop_println_pair(std::string_view rem, T& pq)
{
    std::cout << rem << ": ";
    for (; !pq.empty(); pq.pop()){
        auto [dist, idx] = pq.top();
        std::cout << '(' << dist << ',' << idx << "), ";
    }
    std::cout << '\n';
}

template<typename T>
void println(std::string_view rem, const T& v)
{
    std::cout << rem << ": ";
    for (const auto& e : v)
        std::cout << e << ' ';
    std::cout << '\n';
}

int main()
{
    const auto data = {1, 8, 5, 6, 3, 4, 0, 9, 7, 2};
    println("data", data);

    std::priority_queue<int> max_priority_queue;
    // Fill the priority queue.
    for (int n : data)
        max_priority_queue.push(n);

    pop_println("max_priority_queue", max_priority_queue);

    std::priority_queue<std::pair<float, int64_t> > nearestNeighbors;
    nearestNeighbors.push({1.0, 1});
    nearestNeighbors.push({2.0, 20});
    nearestNeighbors.push({1.5, 3});
    nearestNeighbors.push({3.5, 10});
    nearestNeighbors.push({3.5, 30});

    pop_println_pair("nearestNeighbors", nearestNeighbors);
    return 0;
}
