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

    std::vector<float> inputData = {1.1, 2.1, 3.1};
    // Insert a vector of ints
    myMap["input0"] = std::make_pair(std::vector<int64_t>{10, 2}, std::move(inputData));
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

    inputData.push_back(9.99);
    std::cout << "original inputs: ";
    for (const auto& v : inputData) {
        std::cout << v << ' ';
    }
    std::cout << std::endl;


    // Example float array
    float data_array[] = {1.1f, 2.2f, 3.3f, 4.4f};
    // Size of the array
    size_t size = sizeof(data_array) / sizeof(data_array[0]);

    // Create an empty std::vector<float>
    std::vector<float> data_vector;

    // Assign elements to the vector from float*
    data_vector.assign(data_array, data_array + size);

    // Print elements of the vector
    for (float value : data_vector) {
        std::cout << value << " ";
    }
    std::cout << std::endl;


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
        std::cout << "key: " << key << ", ";
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

    std::vector<int64_t> dims = {10, 2};
    OutputDataMap myOutputMap;
    myOutputMap["output0"] = std::make_pair(dims, std::vector<int64_t>{}); // empty vector of floats

    auto& outputData = std::get<1>(myOutputMap["output0"].second);
    outputData.push_back(1);
    outputData.push_back(2);
    outputData.push_back(3);

    std::cout << "outputData vector: ";
    for (const auto& v : std::get<1>(myOutputMap["output0"].second)) {
        std::cout << v << ' ';
    }
    std::cout << std::endl;


    return 0;
}
