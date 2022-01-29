#include <iostream>
#include <regex>
#include <set>
#include <utility>

#include "common.h"

using Point = std::pair<unsigned, unsigned>;
using FoldInstruction = std::pair<char, unsigned>;
using Paper = std::pair<std::set<Point>, std::vector<FoldInstruction>>;

Paper parseInput(const std::vector<std::string>& input) {
    const std::regex pattern{R"#(fold along (y|x)=(\d+))#"};

    std::set<Point> points;
    auto it = input.begin();

    while (!it->empty()) {
        const auto splitString = common::split(*it, ",");
        points.emplace(std::stoul(splitString[0]), std::stoul(splitString[1]));
        it++;
    }

    it++;

    std::vector<FoldInstruction> foldInstructions;
    while (it != input.end()) {
        std::smatch matchResult;

        if (!std::regex_match(*it, matchResult, pattern))
            throw std::runtime_error{"could not parse line " + *it};

        foldInstructions.emplace_back(matchResult[1].str()[0], std::stoul(matchResult[2].str()));
        it++;
    }

    return std::make_pair(points, foldInstructions);
}

std::set<Point> executeFolding(const std::set<Point>& points, const FoldInstruction& instruction) {
    std::set<Point> pointsAfterFolding;

    for (const auto& point : points) {
        if (instruction.first == 'x') {
            if (point.first < instruction.second) {
                pointsAfterFolding.insert(point);
            } else if (point.first > instruction.second) {
                auto diff = point.first - instruction.second;
                pointsAfterFolding.emplace(instruction.second - diff, point.second);
            }
        } else {
            if (point.second < instruction.second) {
                pointsAfterFolding.insert(point);
            } else if (point.second > instruction.second) {
                auto diff = point.second - instruction.second;
                pointsAfterFolding.emplace(point.first, instruction.second - diff);
            }
        }
    }

    return pointsAfterFolding;
}

unsigned countPointsAfterFirstFolding(const Paper& paper) {
    return executeFolding(paper.first, paper.second.front()).size();
}

void displayPoints(const std::set<Point>& points) {
    unsigned maxX{0};
    unsigned maxY{0};

    for (const auto& p : points) {
        if (p.first > maxX) maxX = p.first;

        if (p.second > maxY) maxY = p.second;
    }

    for (unsigned y = 0; y <= maxY; y++) {
        for (unsigned x = 0; x <= maxX; x++) {
            if (points.contains(Point{x, y}))
                std::cout << "#";
            else
                std::cout << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void part2(const Paper& paper) {
    auto points = paper.first;

    for (const auto& instruction : paper.second) {
        points = executeFolding(points, instruction);
    }

    displayPoints(points);
}

int main(int argc, char* argv[]) {
    try {
        const auto inputFilename = common::getFilename(argc, argv);
        const auto input = common::readInput(inputFilename);

        const auto paper = parseInput(input);
        std::cout << "part 1: " << countPointsAfterFirstFolding(paper) << std::endl;
        part2(paper);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}