#include <iostream>
#include <list>
#include <set>
#include <vector>

#include "common.h"

using Point = std::pair<unsigned, unsigned>;

std::vector<std::vector<unsigned>> parseHeightmap(const std::vector<std::string>& input) {
    std::vector<std::vector<unsigned>> heightmap{};

    for (const auto& line : input) {
        std::vector<unsigned> numbersLine{};

        for (const auto c : line) numbersLine.push_back(c - 48);

        heightmap.push_back(std::move(numbersLine));
    }

    return heightmap;
}

std::vector<Point> findLowPoints(const std::vector<std::vector<unsigned>>& heightmap) {
    std::vector<Point> lowPoints;

    for (size_t row = 0; row < heightmap.size(); row++) {
        for (size_t column = 0; column < heightmap[row].size(); column++) {
            const auto value = heightmap[row][column];

            bool isLowPoint{true};

            if (row != 0) isLowPoint = (heightmap[row - 1][column] > value);

            if (isLowPoint && row != (heightmap.size() - 1))
                isLowPoint = (heightmap[row + 1][column] > value);

            if (isLowPoint && column != 0) isLowPoint = (heightmap[row][column - 1] > value);

            if (isLowPoint && column != (heightmap[row].size() - 1))
                isLowPoint = (heightmap[row][column + 1] > value);

            if (isLowPoint) lowPoints.emplace_back(row, column);
        }
    }

    return lowPoints;
}

unsigned calculateRiskLevel(const std::vector<std::vector<unsigned>>& heightmap) {
    unsigned riskLevel{0};

    const auto lowPoints = findLowPoints(heightmap);
    for (const auto& point : lowPoints) riskLevel += heightmap[point.first][point.second] + 1;

    return riskLevel;
}

unsigned calculateBasinFactor(const std::vector<std::vector<unsigned>>& heightmap) {
    const auto height = heightmap.size();
    const auto width = heightmap[0].size();

    const auto lowPoints = findLowPoints(heightmap);
    std::vector<unsigned> basinsSizes{};
    basinsSizes.reserve(lowPoints.size());

    for (const auto& lowPoint : lowPoints) {
        std::list<Point> pointsToProcess{};
        std::set<Point> processedPoints{};

        pointsToProcess.push_back(lowPoint);
        while (!pointsToProcess.empty()) {
            const auto currentPoint = pointsToProcess.front();
            pointsToProcess.pop_front();

            processedPoints.insert(currentPoint);

            const auto row = currentPoint.first;
            const auto column = currentPoint.second;

            if (row != 0) {  // up
                auto upPoint = std::make_pair(row - 1, column);
                if (!processedPoints.contains(upPoint) && heightmap[row - 1][column] != 9)
                    pointsToProcess.push_back(std::move(upPoint));
            }

            if (row != height - 1) {  // down
                auto downPoint = std::make_pair(row + 1, column);
                if (!processedPoints.contains(downPoint) && heightmap[row + 1][column] != 9)
                    pointsToProcess.push_back(std::move(downPoint));
            }

            if (column != 0) {  // left
                auto leftPoint = std::make_pair(row, column - 1);
                if (!processedPoints.contains(leftPoint) && heightmap[row][column - 1] != 9)
                    pointsToProcess.push_back(std::move(leftPoint));
            }

            if (column != width - 1) {  // right
                auto rightPoint = std::make_pair(row, column + 1);
                if (!processedPoints.contains(rightPoint) && heightmap[row][column + 1] != 9)
                    pointsToProcess.push_back(std::move(rightPoint));
            }
        }

        basinsSizes.push_back(processedPoints.size());
    }

    std::sort(basinsSizes.begin(), basinsSizes.end(), std::greater<>());
    return basinsSizes[0] * basinsSizes[1] * basinsSizes[2];
}

int main(int argc, char* argv[]) {
    try {
        const auto filename = common::getFilename(argc, argv);
        const auto input = common::readInput(filename);

        const auto heightmap = parseHeightmap(input);
        std::cout << "part 1: " << calculateRiskLevel(heightmap) << std::endl;
        std::cout << "part 2: " << calculateBasinFactor(heightmap) << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}