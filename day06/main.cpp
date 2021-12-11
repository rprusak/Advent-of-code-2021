#include <array>
#include <iostream>
#include <numeric>
#include <vector>

#include "common.h"

std::vector<int> convertToNumbers(const std::vector<std::string>& input) {
    const auto splitString = common::split(input.at(0), ",");
    std::vector<int> numbers;

    std::transform(splitString.begin(), splitString.end(), std::back_inserter(numbers),
                   [](const std::string& s) { return std::stoi(s); });

    return numbers;
}

unsigned simulate(std::vector<int> timers, unsigned days) {
    for (unsigned day = 1; day <= days; day++) {
        std::vector<int> newTimers;

        for (auto& timer : timers) {
            if (timer == 0) {
                timer = 6;
                newTimers.push_back(8);
            } else {
                timer -= 1;
            };
        }

        for (auto newTimer : newTimers) timers.push_back(newTimer);
    }

    return timers.size();
}

uint64_t simulate2(std::vector<int> timers, int days) {
    std::array<uint64_t, 9> fishPerAge{0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (const auto t : timers) fishPerAge[t]++;

    int day = 0;
    while (day < days) {
        auto totalFishResetting = fishPerAge[0];
        fishPerAge[0] = fishPerAge[1];
        fishPerAge[1] = fishPerAge[2];
        fishPerAge[2] = fishPerAge[3];
        fishPerAge[3] = fishPerAge[4];
        fishPerAge[4] = fishPerAge[5];
        fishPerAge[5] = fishPerAge[6];
        fishPerAge[6] = fishPerAge[7];
        fishPerAge[7] = fishPerAge[8];
        // update
        fishPerAge[6] += totalFishResetting;
        fishPerAge[8] = totalFishResetting;
        ++day;
    }

    uint64_t res{0};
    for (int age = 0; age <= 8; age++) res += fishPerAge[age];

    return res;
}

int main(int argc, char* argv[]) {
    try {
        const auto filename = common::getFilename(argc, argv);
        const auto input = common::readInput(filename);
        const auto numbers = convertToNumbers(input);

        std::cout << "part 1 " << simulate(numbers, 80) << std::endl;
        std::cout << "part 2 " << simulate2(numbers, 256) << std::endl;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}