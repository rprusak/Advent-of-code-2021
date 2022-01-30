#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <utility>

#include "common.h"

using Instructions = std::map<std::string, char>;
using Formula = std::pair<std::string, Instructions>;
using Histogram = std::map<char, uint64_t>;
using CacheKey = std::tuple<char, char, uint64_t>;
using Cache = std::map<CacheKey, Histogram>;

Formula parseInput(const std::vector<std::string>& input) {
    const std::regex pattern{R"#((\w{2}) -> (\w))#"};

    Instructions instructions;
    for (auto it = input.begin() + 2; it != input.end(); it++) {
        std::smatch matchResult;
        if (!std::regex_match(*it, matchResult, pattern))
            throw std::runtime_error{"could not parse line: " + *it};

        instructions[matchResult[1].str()] = matchResult[2].str()[0];
    }

    return std::make_pair(input.front(), instructions);
}

template <typename T>
Histogram calculateHistogram(const T& polymer) {
    std::map<char, uint64_t> histogram;

    for (const auto c : polymer) {
        histogram[c]++;
    }

    return histogram;
}

uint64_t calculateDifference(const Histogram& histogram) {
    auto comparator = [](const auto& firstValue, auto& secondValue) {
        return firstValue.second < secondValue.second;
    };

    const auto maxElement = std::max_element(histogram.begin(), histogram.end(), comparator);
    const auto minElement = std::min_element(histogram.begin(), histogram.end(), comparator);

    return maxElement->second - minElement->second;
}

void addToHistogram(Histogram& first, const Histogram& second) {
    for (const auto& value : second) {
        first[value.first] += value.second;
    }
}

Histogram execute(char first, char second, const Instructions& instructions, uint64_t currentStep,
                  uint64_t maxStep, Cache& cache) {
    const auto key = std::make_tuple(first, second, currentStep);
    if (cache.contains(key)) {
        return cache.at(key);
    }

    const std::string currentString{first, second};
    const char newChar = instructions.at(currentString);

    Histogram histogram;
    histogram[newChar]++;

    if (currentStep != maxStep) {
        addToHistogram(histogram,
                       execute(first, newChar, instructions, currentStep + 1, maxStep, cache));
        addToHistogram(histogram,
                       execute(newChar, second, instructions, currentStep + 1, maxStep, cache));
    }

    cache[key] = histogram;
    return histogram;
}

uint64_t executeSteps(const Formula& formula, uint64_t steps) {
    const std::string& startingPolymer = formula.first;
    const auto& instructions = formula.second;
    auto histogram = calculateHistogram(startingPolymer);

    Cache cache;

    for (auto it = startingPolymer.begin(); it != startingPolymer.end() - 1; it++) {
        addToHistogram(histogram, execute(*it, *(it + 1), instructions, 1, steps, cache));
    }

    return calculateDifference(histogram);
}

int main(int argc, char* argv[]) {
    try {
        const auto inputFilename = common::getFilename(argc, argv);
        const auto input = common::readInput(inputFilename);

        const auto formula = parseInput(input);

        std::cout << "part 1: " << executeSteps(formula, 10) << std::endl;
        std::cout << "part 2: " << executeSteps(formula, 40) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}