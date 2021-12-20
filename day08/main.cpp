#include <cassert>
#include <iostream>
#include <map>

#include "common.h"

struct Note {
    std::vector<std::string> signals;
    std::vector<std::string> digits;

    void sort() {
        for (auto& signal : signals) std::sort(signal.begin(), signal.end());

        std::sort(signals.begin(), signals.end(), [](const std::string& s1, const std::string& s2) {
            return s1.size() < s2.size();
        });

        for (auto& signal : digits) std::sort(signal.begin(), signal.end());

        ////        std::sort(digits.begin(), digits.end(), [](const std::string& s1, const
        ///std::string& s2) { /            return s1.size() < s2.size(); /        });
    }

    void display() {
        for (const auto& signal : signals) std::cout << signal << " ";
        std::cout << "| ";
        for (const auto& digit : digits) std::cout << digit << " ";
        std::cout << std::endl;
    }

    std::string getNumberOne() const {
        return *std::find_if(signals.begin(), signals.end(),
                             [](const std::string& s) { return s.size() == 2; });
    }

    std::string getNumberSeven() const {
        return *std::find_if(signals.begin(), signals.end(),
                             [](const std::string& s) { return s.size() == 3; });
    }

    std::string getNumberFour() const {
        return *std::find_if(signals.begin(), signals.end(),
                             [](const std::string& s) { return s.size() == 4; });
    }

    std::string getNumberEight() const {
        return *std::find_if(signals.begin(), signals.end(),
                             [](const std::string& s) { return s.size() == 7; });
    }
};

std::vector<Note> parseNotes(const std::vector<std::string>& input) {
    std::vector<Note> notes;

    for (const auto& line : input) {
        const auto split = common::split(line, "|");
        Note n{};
        n.signals = common::split(split[0], " ");
        n.digits = common::split(split[1], " ");
        notes.push_back(n);
    }

    return notes;
}

unsigned part1(const std::vector<std::string>& input) {
    const auto notes = parseNotes(input);
    unsigned simpleDigitsCount{0};

    for (const auto& note : notes) {
        simpleDigitsCount +=
            std::count_if(note.digits.begin(), note.digits.end(), [](const std::string& s) {
                return s.size() == 2 || s.size() == 7 || s.size() == 3 || s.size() == 4;
            });
    }

    return simpleDigitsCount;
}

std::string getDifference(const std::string& s1, const std::string& s2) {
    std::string difference{};

    std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(difference));

    return difference;
}

std::string merge(const std::initializer_list<std::string>& strings) {
    std::string result;

    for (const auto& string : strings) {
        std::string tmp;
        std::set_union(string.begin(), string.end(), result.begin(), result.end(),
                       std::back_inserter(tmp));
        result = std::move(tmp);
    }

    return result;
}

std::string findNine(const std::vector<std::string>& signals, const std::string& pattern) {
    const auto matchesPattern = [&pattern](const std::string& s) {
        return std::all_of(pattern.begin(), pattern.end(),
                           [s](char c) { return s.find(c) != std::string::npos; });
    };

    const auto isNine = [matchesPattern](const std::string& s) {
        return s.size() == 6 && matchesPattern(s);
    };

    return *std::find_if(signals.begin(), signals.end(), isNine);
}

std::string findSix(const std::vector<std::string>& signals, const std::string& pattern) {
    const auto matchesPattern = [&pattern](const std::string& s) {
        return std::all_of(pattern.begin(), pattern.end(),
                           [s](char c) { return s.find(c) != std::string::npos; });
    };

    const auto isNine = [matchesPattern](const std::string& s) {
        return s.size() == 6 && matchesPattern(s);
    };

    return *std::find_if(signals.begin(), signals.end(), isNine);
}

std::string findThree(const std::vector<std::string>& signals, const std::string& pattern,
                      const std::string topLeftAndMid) {
    const auto matchesPattern = [&pattern](const std::string& s) {
        return std::all_of(pattern.begin(), pattern.end(),
                           [s](char c) { return s.find(c) != std::string::npos; });
    };

    const auto hasOne = [&topLeftAndMid](const std::string& s) {
        return std::any_of(topLeftAndMid.begin(), topLeftAndMid.end(),
                           [&s](char c) { return s.find(c) != std::string::npos; });
    };

    const auto isNine = [matchesPattern, hasOne](const std::string& s) {
        return s.size() == 5 && matchesPattern(s) && hasOne(s);
    };

    return *std::find_if(signals.begin(), signals.end(), isNine);
}

std::string findTwo(const std::vector<std::string>& signals, const std::string& pattern,
                    const std::string topRightAndBottomRight) {
    const auto matchesPattern = [&pattern](const std::string& s) {
        return std::all_of(pattern.begin(), pattern.end(),
                           [s](char c) { return s.find(c) != std::string::npos; });
    };

    const auto hasOne = [&topRightAndBottomRight](const std::string& s) {
        return std::any_of(topRightAndBottomRight.begin(), topRightAndBottomRight.end(),
                           [&s](char c) { return s.find(c) != std::string::npos; });
    };

    const auto isNine = [matchesPattern, hasOne](const std::string& s) {
        return s.size() == 5 && matchesPattern(s) && hasOne(s);
    };

    return *std::find_if(signals.begin(), signals.end(), isNine);
}

enum class Connection { TOP, TOP_LEFT, TOP_RIGHT, MIDDLE, BOTTOM_LEFT, BOTTOM_RIGHT, BOTTOM };

bool hasEnabled(const std::string& digit, char signal) {
    return digit.find(signal) != std::string::npos;
}

std::map<Connection, char> getConnections(const Note& n) {
    const auto numberOne = n.getNumberOne();
    const auto numberFour = n.getNumberFour();
    const auto numberSeven = n.getNumberSeven();
    const auto numberEight = n.getNumberEight();

    const auto top = getDifference(numberSeven, numberOne);
    const auto topLeftAndMid = getDifference(numberFour, numberOne);
    const auto topRightAndBottomRight = numberOne;

    const auto ninePattern = merge({top, topLeftAndMid, topRightAndBottomRight});
    const auto nine = findNine(n.signals, ninePattern);
    const auto bottom = getDifference(nine, ninePattern);

    const auto threePattern = merge({top, bottom, topRightAndBottomRight});
    const auto three = findThree(n.signals, threePattern, topLeftAndMid);
    const auto middle = getDifference(three, threePattern);
    const auto topLeft = getDifference(topLeftAndMid, middle);

    const auto eightPattern = merge({top, topLeftAndMid, topRightAndBottomRight, bottom});
    const auto bottomLeft = getDifference(numberEight, eightPattern);

    const auto twoPattern = merge({top, bottom, middle, bottomLeft});
    const auto two = findTwo(n.signals, twoPattern, topRightAndBottomRight);
    const auto topRight = getDifference(two, twoPattern);
    const auto bottomRight = getDifference(topRightAndBottomRight, topRight);

    const auto sixPattern = merge({top, bottom, middle, bottomLeft, bottomRight, topLeft});
    const auto six = findSix(n.signals, sixPattern);

    assert(n.digits.size() == 4);
    assert(n.signals.size() == 10);
    assert(top.size() == 1);
    assert(topLeft.size() == 1);
    assert(topRight.size() == 1);
    assert(middle.size() == 1);
    assert(bottomLeft.size() == 1);
    assert(bottomRight.size() == 1);
    assert(bottom.size() == 1);
    assert(topLeftAndMid.size() == 2);
    assert(topRightAndBottomRight.size() == 2);

    assert(!hasEnabled(nine, bottomLeft.front()));
    assert(!hasEnabled(three, topLeft.front()));
    assert(!hasEnabled(three, bottomLeft.front()));

    std::map<Connection, char> connections{
        {Connection::TOP, top.front()},
        {Connection::TOP_LEFT, topLeft.front()},
        {Connection::TOP_RIGHT, topRight.front()},
        {Connection::MIDDLE, middle.front()},
        {Connection::BOTTOM_LEFT, bottomLeft.front()},
        {Connection::BOTTOM_RIGHT, bottomRight.front()},
        {Connection::BOTTOM, bottom.front()},
    };

    return connections;
}

unsigned encodeDigit(const std::string& digit, const std::map<Connection, char>& connections) {
    switch (digit.size()) {
        case 2:
            return 1;
        case 3:
            return 7;
        case 4:
            return 4;
        case 5:
            if (hasEnabled(digit, connections.at(Connection::TOP_LEFT)))
                return 5;
            else if (hasEnabled(digit, connections.at(Connection::BOTTOM_LEFT)))
                return 2;
            else
                return 3;
        case 6:
            if (!hasEnabled(digit, connections.at(Connection::MIDDLE)))
                return 0;
            else if (hasEnabled(digit, connections.at(Connection::BOTTOM_LEFT)))
                return 6;
            else
                return 9;
        case 7:
            return 8;
        default:
            std::cerr << "error encoding digit" << std::endl;
            return 0;
    }
}

unsigned encodeNote(const Note& n) {
    unsigned result{0};

    const auto connections = getConnections(n);
    for (const auto& digit : n.digits) {
        auto encodedDigit = encodeDigit(digit, connections);
        result *= 10;
        result += encodedDigit;
    }

    return result;
}

unsigned part2(const std::vector<std::string>& input) {
    auto notes = parseNotes(input);
    unsigned result{0};

    for (auto& note : notes) {
        note.sort();
        // note.display();
        const auto tmp = encodeNote(note);

        std::cout << tmp << std::endl;
        result += tmp;
    }

    return result;
}

int main(int argc, char* argv[]) {
    try {
        const auto filename = common::getFilename(argc, argv);
        const auto input = common::readInput(filename);
        std::cout << "part 1: " << part1(input) << std::endl;
        std::cout << "part 2: " << part2(input) << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}