#include <iostream>
#include <regex>

#include "common.h"

struct Line {
    unsigned x1;
    unsigned y1;
    unsigned x2;
    unsigned y2;

    bool isHorizontal() const { return x1 == x2; }

    bool isVertical() const { return y1 == y2; }
};

std::vector<Line> parseLines(const std::vector<std::string> &input) {
    const std::regex pattern{R"#((\d+),(\d+) -> (\d+),(\d+))#"};
    std::vector<Line> lines;

    for (const auto &l : input) {
        std::smatch matchResult;
        if (!std::regex_match(l, matchResult, pattern))
            throw std::runtime_error{"could not parse line " + l};

        Line line{};

        line.x1 = std::stoul(matchResult[1].str());
        line.y1 = std::stoul(matchResult[2].str());
        line.x2 = std::stoul(matchResult[3].str());
        line.y2 = std::stoul(matchResult[4].str());

        lines.push_back(line);
    }

    return lines;
}

unsigned calculateNumberOfOverlaps(const std::vector<Line> &lines) {
    using Point = std::pair<unsigned, unsigned>;
    std::map<Point, unsigned> points;

    for (const auto &line : lines) {
        if (line.isHorizontal()) {
            const auto x = line.x1;
            const auto startY = std::min(line.y1, line.y2);
            const auto endY = std::max(line.y1, line.y2);

            for (unsigned y = startY; y <= endY; y++) {
                points[Point{x, y}] += 1;
            }
        } else if (line.isVertical()) {
            const auto y = line.y1;
            const auto startX = std::min(line.x1, line.x2);
            const auto endX = std::max(line.x1, line.x2);

            for (unsigned x = startX; x <= endX; x++) {
                points[Point{x, y}] += 1;
            }
        }
    }

    const auto multipleLinesOverlap = [](const std::pair<Point, unsigned> &p) {
        return p.second >= 2;
    };

    return std::count_if(points.begin(), points.end(), multipleLinesOverlap);
}

unsigned calculateNumberOfOverlapsWithDiagonalLines(const std::vector<Line> &lines) {
    using Point = std::pair<unsigned, unsigned>;
    std::map<Point, unsigned> points;

    for (const auto &line : lines) {
        if (line.isHorizontal()) {
            const auto x = line.x1;
            const auto startY = std::min(line.y1, line.y2);
            const auto endY = std::max(line.y1, line.y2);

            for (unsigned y = startY; y <= endY; y++) {
                points[Point{x, y}] += 1;
            }
        } else if (line.isVertical()) {
            const auto y = line.y1;
            const auto startX = std::min(line.x1, line.x2);
            const auto endX = std::max(line.x1, line.x2);

            for (unsigned x = startX; x <= endX; x++) {
                points[Point{x, y}] += 1;
            }
        } else {
            Point startPoint;
            Point endPoint;
            if (line.x1 < line.x2) {
                startPoint = Point{line.x1, line.y1};
                endPoint = Point{line.x2, line.y2};
            } else {
                startPoint = Point{line.x2, line.y2};
                endPoint = Point{line.x1, line.y1};
            }

            unsigned y = startPoint.second;
            for (unsigned x = startPoint.first; x <= endPoint.first; x++) {
                points[Point{x, y}] += 1;

                if (endPoint.second > startPoint.second)
                    y++;
                else
                    y--;
            }
        }
    }

    const auto multipleLinesOverlap = [](const std::pair<Point, unsigned> &p) {
        return p.second >= 2;
    };

    return std::count_if(points.begin(), points.end(), multipleLinesOverlap);
}

int main(int argc, char *argv[]) {
    try {
        const auto filename = common::getFilename(argc, argv);
        const auto input = common::readInput(filename);

        const auto lines = parseLines(input);

        std::cout << "part 1 " << calculateNumberOfOverlaps(lines) << std::endl;
        std::cout << "part 2 " << calculateNumberOfOverlapsWithDiagonalLines(lines) << std::endl;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}