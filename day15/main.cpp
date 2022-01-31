#include <iostream>
#include <map>
#include <queue>
#include <utility>

#include "common.h"

using Point = std::pair<unsigned, unsigned>;
using RiskMap = std::map<Point, unsigned>;

class Cave : public std::vector<std::vector<unsigned>> {
  public:
    unsigned getRisk(const Point &p) const {
        const auto originalCaveSize = this->size();

        const unsigned xOffset = p.first / originalCaveSize;
        const unsigned yOffset = p.second / originalCaveSize;
        const unsigned x = p.first % originalCaveSize;
        const unsigned y = p.second % originalCaveSize;

        auto originalRiskLevel = (*this)[y][x];
        originalRiskLevel += xOffset + yOffset;

        return originalRiskLevel > 9 ? originalRiskLevel % 9 : originalRiskLevel;
    }
};

Cave parseInput(const std::vector<std::string> &input) {
    Cave result;

    for (const auto &line : input) {
        std::vector<unsigned> v;

        for (char c : line) {
            v.push_back(c - 48);
        }

        result.push_back(v);
    }

    return result;
}

std::vector<Point> getClosePoints(const Point &point, unsigned caveSize) {
    std::vector<Point> closePoints;

    if (point.first > 0) closePoints.emplace_back(point.first - 1, point.second);

    if (point.first < caveSize - 1) closePoints.emplace_back(point.first + 1, point.second);

    if (point.second > 0) closePoints.emplace_back(point.first, point.second - 1);

    if (point.second < caveSize - 1) closePoints.emplace_back(point.first, point.second + 1);

    return closePoints;
}

unsigned calculateLowestTotalRisk(unsigned caveSize, const Cave &cave) {
    RiskMap riskMap;

    for (unsigned y = 0; y < caveSize; y++) {
        for (unsigned x = 0; x < caveSize; x++) {
            riskMap[Point{x, y}] = std::numeric_limits<unsigned>::max();
        }
    }

    //    for (unsigned y = 0; y < caveSize; y++) {
    //        for (unsigned x = 0; x < caveSize; x++) {
    //            std::cout << cave.getRisk({x, y});
    //        }
    //        std::cout << std::endl;
    //    }

    std::cout << std::endl;

    riskMap[{0, 0}] = 0;

    auto compare = [&](const Point &p1, const Point &p2) { return riskMap[p1] > riskMap[p2]; };

    std::priority_queue<Point, std::vector<Point>, decltype(compare)> pointsQue{compare};
    pointsQue.push({0, 0});

    while (!pointsQue.empty()) {
        const auto currentPoint = pointsQue.top();
        pointsQue.pop();

        if (currentPoint.first == caveSize - 1 && currentPoint.second == caveSize - 1) continue;

        const auto closePoints = getClosePoints(currentPoint, caveSize);
        for (const auto &point : closePoints) {
            auto pointRisk = riskMap[point];
            auto newRisk = riskMap[currentPoint] + cave.getRisk(point);
            if (pointRisk > newRisk) {
                riskMap[point] = newRisk;
                pointsQue.push(point);
            }
        }
    }

    return riskMap[{caveSize - 1, caveSize - 1}];
}

int main(int argc, char *argv[]) {
    try {
        const auto inputFilename = common::getFilename(argc, argv);
        const auto input = common::readInput(inputFilename);

        const auto cave = parseInput(input);

        std::cout << "part 1: " << calculateLowestTotalRisk(cave.size(), cave) << std::endl;
        std::cout << "part 2: " << calculateLowestTotalRisk(cave.size() * 5, cave) << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}