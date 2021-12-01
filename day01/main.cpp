#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>

#include "common.h"

unsigned getIncreasedMeasurementCount(const std::vector<int>& numbers) {
  unsigned counter{0};
  auto start = numbers.begin();

  while (start != numbers.end()) {
    start =
      std::adjacent_find(start, numbers.end(), [](const auto& a, const auto& b) { return a < b; });
    if (start != numbers.end()) {
      counter++;
      start++;
    }
  }

  return counter;
}

unsigned getIncreasedTripleMeasurementCount(const std::vector<int>& numbers) {
  unsigned counter{0};

  auto firstWindowStart = numbers.begin();
  auto secondWindowStart = numbers.begin() + 1;

  while (std::distance(secondWindowStart, numbers.end()) >= 3) {
    auto firstSum = std::accumulate(firstWindowStart, firstWindowStart + 3, 0);
    auto secondSum = std::accumulate(secondWindowStart, secondWindowStart + 3, 0);

    if (secondSum > firstSum) {
      counter++;
    }

    firstWindowStart++;
    secondWindowStart++;
  }

  return counter;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Argument required" << std::endl;
    return 1;
  }

  const std::string inputFilename = argv[1];
  auto input = common::readInput(inputFilename);
  std::vector<int> numbers;
  std::transform(input.begin(), input.end(), std::back_inserter(numbers),
                 [](const std::string& line) { return std::stoi(line); });

  std::cout << "part 1 " << getIncreasedMeasurementCount(numbers) << std::endl;
  std::cout << "part 2 " << getIncreasedTripleMeasurementCount(numbers) << std::endl;

  return 0;
}