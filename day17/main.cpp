#include <iostream>

#include "common.h"

struct Area {
    int xStart;
    int xEnd;
    int yStart;
    int yEnd;

    bool isXInArea(int x) const { return x >= this->xStart && x <= this->xEnd; }

    bool isInArea(int x, int y) const {
        return x >= this->xStart && x <= this->xEnd && y >= this->yStart && y <= this->yEnd;
    }

    bool isAfter(int x) const { return x > this->xEnd; }

    bool isBefore(int x) const { return x < this->xStart; }

    bool isUnder(int y) const { return y < this->yStart; }
};

int nextVx(int x) {
    if (x == 0) return 0;

    return x > 0 ? x - 1 : x + 1;
}

int nextVy(int y) { return y - 1; }

bool willStayInArea(int position, int speed, const Area& area) {
    while (true) {
        if (area.isXInArea(position)) {
            if (speed == 0) return true;

            position += speed;
            speed = nextVx(speed);
        } else {
            return false;
        }
    }
}

std::vector<int> getXCandidates(const Area& area, bool bestOnly = true) {
    std::vector<int> xCandidates{};
    for (int xSpeed = area.xEnd; xSpeed >= 1; xSpeed--) {
        int position{0};
        int currentSpeed{xSpeed};
        int step = 1;

        while (true) {
            position += currentSpeed;

            if (area.isXInArea(position)) {
                if (bestOnly && willStayInArea(position, nextVx(currentSpeed), area)) {
                    xCandidates.push_back(xSpeed);
                } else {
                    xCandidates.push_back(xSpeed);
                }

                break;
            }

            if (position > area.xEnd) break;

            currentSpeed = nextVx(currentSpeed);
            if (currentSpeed == 0) break;

            step++;
        }
    }

    return xCandidates;
}

int simulate(const Area& area, int xVelocity, int yVelocity) {
    int x{0};
    int y{0};
    int yMax{0};

    while (true) {
        x += xVelocity;
        y += yVelocity;

        xVelocity = nextVx(xVelocity);
        yVelocity = nextVy(yVelocity);

        if (y > yMax) yMax = y;

        if (area.isInArea(x, y)) return yMax;

        if (area.isAfter(x)) return -1;

        if (xVelocity == 0 && area.isBefore(x)) return -1;

        if (area.isUnder(y)) return -1;
    }
}

void part1(const Area& area) {
    const auto xCandidates = getXCandidates(area);
    int best{0};

    for (auto x : xCandidates) {
        for (int y = 1; y < 200; y++) {
            auto res = simulate(area, x, y);
            if (res > best) best = res;
        }
    }

    std::cout << "part 1: " << best << std::endl;
}

void part2(const Area& area) {
    const auto xCandidates = getXCandidates(area, false);
    int counter{0};

    for (auto x : xCandidates) {
        for (int y = -400; y < 400; y++) {
            auto res = simulate(area, x, y);
            if (res != -1) counter++;
        }
    }

    std::cout << "part 2: " << counter << std::endl;
}

int main() {
    Area area{111, 161, -154, -101};

    part1(area);
    part2(area);

    return 0;
}