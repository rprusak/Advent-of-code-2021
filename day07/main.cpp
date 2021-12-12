#include <iostream>

#include "common.h"

int median(const std::vector<int>& numbers) {
    if (numbers.size() % 2 == 0) {
        const auto middle = numbers.size() / 2;
        return (numbers[middle - 1] + numbers[middle]) / 2;
    } else {
        return numbers[numbers.size() / 2];
    }
}

int calculateFuel(const std::vector<int>& numbers, const int position) {
    int result{0};

    for (const auto number : numbers) result += std::abs(number - position);

    return result;
}

int calculateFuel2(const std::vector<int>& numbers, const std::vector<int>& series,
                   const int position) {
    int result{0};

    for (const auto number : numbers) result += series[std::abs(number - position) - 1];

    return result;
}

int part2(const std::vector<int>& numbers) {
    std::vector<int> series(numbers.back());
    series[0] = 1;
    for (size_t i = 1; i < (size_t)numbers.back(); i++) series[i] = i + 1 + series[i - 1];

    auto lowestSum = std::numeric_limits<int>::max();

    for (int i = 0; i <= numbers.back(); i++) {
        auto sum = calculateFuel2(numbers, series, i);
        if (sum < lowestSum) lowestSum = sum;
    }

    return lowestSum;
}

int main(int argc, char* argv[]) {
    try {
        const auto filename = common::getFilename(argc, argv);
        const auto input = common::readInput(filename);
        auto numbers = common::convertToNumbers(input);

        std::sort(numbers.begin(), numbers.end());

        for (const auto n : numbers) std::cout << n << " ";

        std::cout << std::endl;
        std::cout << median(numbers) << std::endl;
        std::cout << "part 1: " << calculateFuel(numbers, median(numbers)) << std::endl;
        std::cout << "part 2: " << part2(numbers) << std::endl;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    std::cout << "hello world" << std::endl;
    return 0;
}