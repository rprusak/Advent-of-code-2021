#include <iostream>

#include "common.h"

struct Octopus {
    unsigned energy;
    bool flashed;
    bool spread;
};

using Grid = std::vector<std::vector<Octopus>>;

Grid parseInput(const std::vector<std::string>& input) {
    Grid grid;

    for (const auto& line : input) {
        std::vector<Octopus> octopuses;
        for (const auto c : line) {
            unsigned energy = c - 48;
            octopuses.push_back(Octopus{energy, false, false});
        }

        grid.push_back(octopuses);
    }

    return grid;
}

unsigned markFlashed(Grid& grid) {
    unsigned flashedCount{0};

    for (unsigned row = 0; row < grid.size(); row++) {
        for (unsigned column = 0; column < grid[row].size(); column++) {
            auto& octopus = grid[row][column];

            if (octopus.energy > 9 && !octopus.flashed) {
                octopus.flashed = true;
                flashedCount++;
            }
        }
    }

    return flashedCount;
}

void increaseEnergy(Grid& grid) {
    for (auto& line : grid) {
        for (auto& octopus : line) {
            octopus.energy += 1;
        }
    }
}

void spreadEnergy(Grid& grid) {
    for (unsigned row = 0; row < grid.size(); row++) {
        for (unsigned column = 0; column < grid[row].size(); column++) {
            auto& octopus = grid[row][column];

            if (!octopus.flashed || octopus.spread) continue;

            const auto lastColumn = grid[row].size() - 1;
            const auto lastRow = grid.size() - 1;

            if (row != 0 && column != 0) grid[row - 1][column - 1].energy += 1;
            if (row != 0) grid[row - 1][column].energy += 1;
            if (row != 0 && column != lastColumn) grid[row - 1][column + 1].energy += 1;
            if (column != 0) grid[row][column - 1].energy += 1;
            if (column != lastColumn) grid[row][column + 1].energy += 1;
            if (row != lastRow && column != 0) grid[row + 1][column - 1].energy += 1;
            if (row != lastRow) grid[row + 1][column].energy += 1;
            if (row != lastRow && column != lastColumn) grid[row + 1][column + 1].energy += 1;

            octopus.spread = true;
        }
    }
}

bool flashPossible(const Grid& grid) {
    for (unsigned row = 0; row < grid.size(); row++) {
        for (unsigned column = 0; column < grid[row].size(); column++) {
            if (!grid[row][column].flashed && grid[row][column].energy > 9) return true;
        }
    }

    return false;
}

void reset(Grid& grid) {
    for (unsigned row = 0; row < grid.size(); row++) {
        for (unsigned column = 0; column < grid[row].size(); column++) {
            if (grid[row][column].flashed) {
                grid[row][column].energy = 0;
                grid[row][column].spread = false;
                grid[row][column].flashed = false;
            }
        }
    }
}

unsigned simulateSteps(Grid grid, unsigned stepCount) {
    unsigned flashedCount{0};

    for (unsigned step = 0; step < stepCount; step++) {
        increaseEnergy(grid);

        while (flashPossible(grid)) {
            flashedCount += markFlashed(grid);
            spreadEnergy(grid);
        }

        reset(grid);
    }

    return flashedCount;
}

bool allFlashed(const Grid& grid) {
    for (unsigned row = 0; row < grid.size(); row++) {
        for (unsigned column = 0; column < grid[row].size(); column++) {
            if (!grid[row][column].flashed) return false;
        }
    }

    return true;
}

unsigned findAllFlashedStep(Grid grid) {
    unsigned step{1};

    while (true) {
        increaseEnergy(grid);

        while (flashPossible(grid)) {
            markFlashed(grid);
            spreadEnergy(grid);
        }

        if (allFlashed(grid)) return step;

        step++;
        reset(grid);
    }
}

int main(int argc, char* argv[]) {
    try {
        const auto inputFilename = common::getFilename(argc, argv);
        const auto input = common::readInput(inputFilename);

        const auto grid = parseInput(input);

        std::cout << "part 1: " << simulateSteps(grid, 100) << std::endl;
        std::cout << "part 2: " << findAllFlashedStep(grid) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}