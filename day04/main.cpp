#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <unordered_map>

#include "common.h"

class Bingo;
std::ostream &operator<<(std::ostream &stream, const Bingo &bingo);

class Bingo {
  public:
    explicit Bingo(const std::vector<std::vector<int>> &matrix) {
        for (int i = 0; i < 5; i++) {
            _bingoMap.emplace_back(5, nullptr);
        }

        for (size_t i = 0; i < matrix.size(); i++) {
            for (size_t j = 0; j < matrix[i].size(); j++) {
                auto ptr = std::make_shared<NumberWithStatus>(matrix[i][j], false);
                _bingoMap[i][j] = ptr;
                _numberMap[matrix[i][j]] = ptr;
            }
        }
    }

    bool isWinning() {
        if (_hadAlreadyWon) return true;

        for (int i = 0; i < 5; i++) {
            if (checkRow(i) || checkColumn(i)) {
                _hadAlreadyWon = true;
                return true;
            }
        }

        return false;
    }

    void markValue(const int value) const {
        auto it = _numberMap.find(value);
        if (it != _numberMap.end()) it->second->second = true;
    }

    int getUnmarkedSum() const {
        return std::accumulate(_numberMap.begin(), _numberMap.end(), 0, [](int num, const auto &p) {
            return num + (!p.second->second ? p.first : 0);
        });
    }

    friend std::ostream &operator<<(std::ostream &stream, const Bingo &bingo);

  private:
    using NumberWithStatus = std::pair<int, bool>;
    using NumberWrapper = std::shared_ptr<NumberWithStatus>;

    std::vector<std::vector<NumberWrapper>> _bingoMap;
    std::unordered_map<int, NumberWrapper> _numberMap;
    bool _hadAlreadyWon{false};

    bool checkRow(const int rowNumber) const {
        auto &row = _bingoMap[rowNumber];

        for (const auto &wrapper : row) {
            if (!wrapper->second) return false;
        }

        return true;
    }

    bool checkColumn(const int columnNumber) const {
        for (int i = 0; i < 5; i++) {
            if (!_bingoMap[i][columnNumber]->second) return false;
        }

        return true;
    }
};

std::ostream &operator<<(std::ostream &stream, const Bingo &bingo) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) std::cout << bingo._bingoMap[i][j]->first << " ";
        std::cout << "\n";
    }

    return stream;
}

std::vector<int> getNumbers(const std::string &str, const std::string &delim) {
    const auto splitString = common::split(str, delim);
    std::vector<int> numbers;

    std::transform(splitString.begin(), splitString.end(), std::back_inserter(numbers),
                   [](const std::string &s) { return std::stoi(s); });

    return numbers;
}

std::vector<Bingo> parseBingos(const std::vector<std::string> &input) {
    std::vector<Bingo> bingos;

    auto it = input.begin() + 2;

    while (it != input.end()) {
        if (it->empty()) {
            it++;
            continue;
        }

        std::vector<std::vector<int>> numbersMatrix;
        for (int i = 0; i < 5; i++) {
            const std::string &line = *(it + i);
            numbersMatrix.push_back(getNumbers(line, " "));
        }
        it += 5;
        bingos.emplace_back(numbersMatrix);
    }

    return bingos;
}

void playBingo(const std::vector<int> &numbers, std::vector<Bingo> bingos) {
    for (const auto number : numbers) {
        for (auto &bingo : bingos) {
            bingo.markValue(number);

            if (bingo.isWinning()) {
                auto sum = bingo.getUnmarkedSum();
                std::cout << "part 1 " << sum * number << std::endl;
                return;
            }
        }
    }
}

void playUntilLastBingo(const std::vector<int> &numbers, std::vector<Bingo> bingos) {
    for (const auto number : numbers) {
        for (auto &bingo : bingos) {
            bingo.markValue(number);

            auto bingoHasWon = [](Bingo &bingo) { return bingo.isWinning(); };

            if (bingo.isWinning() && std::all_of(bingos.begin(), bingos.end(), bingoHasWon)) {
                auto sum = bingo.getUnmarkedSum();
                std::cout << "part 2 " << sum * number << std::endl;
                return;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    try {
        const auto filename = common::getFilename(argc, argv);
        const auto input = common::readInput(filename);

        const auto numbers = getNumbers(input.front(), ",");
        auto bingos = parseBingos(input);

        for (const auto &bingo : bingos) {
            std::cout << bingo << std::endl;
        }

        playBingo(numbers, bingos);
        playUntilLastBingo(numbers, bingos);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}