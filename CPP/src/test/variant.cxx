#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>

// Define a variant that can hold either a vector of ints or a vector of floats
using DataVariant = std::variant<std::vector<float>, std::vector<int64_t> >;
using InferenceData = std::pair<std::vector<int64_t>, DataVariant>;
using InputDataMap = std::map<std::string, InferenceData>;
using OutputDataMap = std::map<std::string, InferenceData>;

int main() {
    InputDataMap myMap;

    // Insert a vector of ints
    myMap["input0"] = std::make_pair(std::vector<int64_t>{10, 2}, std::vector<float>{1.1, 2.1, 3.1});
    myMap["input1"] = std::make_pair(std::vector<int64_t>{10, 4}, std::vector<int64_t>{1, 2, 3});


    // Accessing and using elements
    for (const auto& [key, val] : myMap) {
        std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            std::cout << typeid(T).name() << ": ";
            if constexpr (std::is_same_v<T, std::vector<int64_t>>) {
                std::cout << "vector of ints: ";
            } else if constexpr (std::is_same_v<T, std::vector<float>>) {
                std::cout << "vector of floats: ";
            }
            for (const auto& v : arg) {
                std::cout << v << ' ';
            }
            std::cout << std::endl;
        }, val.second);
    }

    for (const auto& [key, val]: myMap) {
        std::cout << "key: " << key << ",";
        std::cout << "dims: " << val.first[0] << "x" << val.first[1] << std::endl;

        using T = std::decay_t<decltype(val.second)>;
        if constexpr (std::is_same_v<T, std::vector<int64_t>>) {
            std::cout << "vector of ints: ";
        } else if constexpr (std::is_same_v<T, std::vector<float>>) {
            std::cout << "vector of floats: ";
        } else {
            std::cout << "unknown type" << std::endl;
        }

    }

    return 0;
}
