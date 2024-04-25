#include <vector>
#include <cstdint>
#include <iostream>
#include <any>

// Enum for StatusCode, you can define it according to your needs
enum class StatusCode {
    Success,
    Failure
};

// base class
class BaseClass {
    public:
        template<typename T>
        StatusCode addInput(std::vector<T>& data);
};

// derived class
template<typename T>
class YourClass: public BaseClass{
public:
    template<typename U>
    StatusCode addInput(std::vector<U>& data);
};

// Template function definition
template<typename T>
template<typename U>
StatusCode YourClass<T>::addInput(std::vector<U>& data) {
    // Your implementation here
    // You can use 'data' vector with type U in this function
    // and return appropriate StatusCode

    // print the data type
    std::cout << "adding data type: " << typeid(U).name() << std::endl;

    return StatusCode::Success;
}

struct InputData {
    std::string name;
    std::vector<int64_t> dims;
    std::vector<std::any> data;
};

template<typename T>
std::vector<T> cast_any_vector(const std::vector<std::any>& anyVec) {
    std::vector<T> castedVec;
    for (const auto& anyVal : anyVec) {
        if (anyVal.type() == typeid(T)) { // Check if the type matches T
            castedVec.push_back(std::any_cast<T>(anyVal));
        } else {
            throw std::bad_cast(); // Or handle the type mismatch appropriately
        }
    }
    return castedVec;
}


// Example usage
int main() {
    YourClass<int64_t> yourClass;

    std::vector<int64_t> intData = {1, 2, 3};
    StatusCode intResult = yourClass.addInput(intData);

    std::vector<float> floatData = {1.0f, 2.0f, 3.0f};
    StatusCode floatResult = yourClass.addInput(floatData);

    std::vector<int64_t> inputShape = {4, 5};
    std::vector<std::any> data(inputShape[0] * inputShape[1], 1.0f);

    InputData inputData = {"input1", inputShape, data};
    InputData input2 = {"input12", inputShape, {3, 4, 5}};

    std::cout << "inputData name: " << inputData.name << std::endl;
    std::cout << "inputData dims: " << inputData.dims[0] << "x" << inputData.dims[1] << std::endl;
    // std::cout << "inputData data: " << inputData.data[0] << std::endl;
    // std::cout << "input2 data: " << input2.data[0] << std::endl;

    std::cout << "inputData data type: " << inputData.data[0].type().name() << std::endl;

    try {
        auto castedData = cast_any_vector<int>(data);
        std::cout << "castedData: " << castedData[0] << std::endl;
    } catch (const std::bad_any_cast& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    std::vector<InputData> inputDatas = {inputData, input2};

    return 0;
}
