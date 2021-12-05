#include <iostream>

#include "common.h"

using BitsStats = std::vector<std::pair<unsigned, unsigned>>;

BitsStats calculateBitsStats(const std::vector<std::string>& input) {
    const auto numberSize = input.front().size();
    BitsStats bitsStats(numberSize);

    for (const auto& number : input) {
        for (size_t i = 0; i < numberSize; i++) {
            if (number[i] == '0')
                bitsStats[i].first += 1;
            else
                bitsStats[i].second += 1;
        }
    }

    return bitsStats;
}

std::pair<int, int> calculateGammaAndEpsilon(const std::vector<std::string>& input) {
    BitsStats bitsStats = calculateBitsStats(input);

    const auto numberSize = input.front().size();
    std::string mostCommonBits(numberSize, ' ');
    std::string leastCommonBits(numberSize, ' ');

    for (size_t i = 0; i < numberSize; i++) {
        const auto& stats = bitsStats[i];
        if (stats.first > stats.second) {
            mostCommonBits[i] = '0';
            leastCommonBits[i] = '1';
        } else {
            mostCommonBits[i] = '1';
            leastCommonBits[i] = '0';
        }
    }

    const auto mostCommon = std::stoi(mostCommonBits, 0, 2);
    const auto leastCommon = std::stoi(leastCommonBits, 0, 2);

    return std::make_pair(mostCommon, leastCommon);
}

int calculateOxygenGeneratorRating(const std::vector<std::string>& input) {
    const auto numberSize = input.front().size();
    std::vector<std::string> currentNumbers = input;
    size_t currentPosition{0};
    char bitCriteria = '0';

    while (currentNumbers.size() != 1 && currentPosition < numberSize) {
        const auto bitsStats = calculateBitsStats(currentNumbers);

        if (bitsStats[currentPosition].second >= bitsStats[currentPosition].first) {
            bitCriteria = '1';
        } else {
            bitCriteria = '0';
        }

        auto doNotMatchBitCriteria = [currentPosition, bitCriteria](const std::string& str) {
            return str[currentPosition] != bitCriteria;
        };

        auto it =
            std::remove_if(currentNumbers.begin(), currentNumbers.end(), doNotMatchBitCriteria);
        currentNumbers.erase(it, currentNumbers.end());
        currentPosition++;
    }

    return std::stoi(currentNumbers.front(), 0, 2);
}

int calculateCO2ScrubberRating(const std::vector<std::string>& input) {
    const auto numberSize = input.front().size();
    std::vector<std::string> currentNumbers = input;
    size_t currentPosition{0};
    char bitCriteria = '0';

    while (currentNumbers.size() != 1 && currentPosition < numberSize) {
        const auto bitsStats = calculateBitsStats(currentNumbers);

        if (bitsStats[currentPosition].first <= bitsStats[currentPosition].second) {
            bitCriteria = '0';
        } else {
            bitCriteria = '1';
        }

        auto doNotMatchBitCriteria = [currentPosition, bitCriteria](const std::string& str) {
            return str[currentPosition] != bitCriteria;
        };

        auto it =
            std::remove_if(currentNumbers.begin(), currentNumbers.end(), doNotMatchBitCriteria);
        currentNumbers.erase(it, currentNumbers.end());
        currentPosition++;
    }

    return std::stoi(currentNumbers.front(), 0, 2);
}

int main(int argc, char* argv[]) {
    try {
        const auto filename = common::getFilename(argc, argv);
        const auto input = common::readInput(filename);

        const auto [gamma, epsilon] = calculateGammaAndEpsilon(input);
        const auto power = gamma * epsilon;

        std::cout << "part 1: " << power << std::endl;

        auto oxygen = calculateOxygenGeneratorRating(input);
        auto co2 = calculateCO2ScrubberRating(input);

        std::cout << "part 2: " << co2 * oxygen << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}