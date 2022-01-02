#include <iostream>
#include <stack>

#include "common.h"

bool isMatch(char leftChar, char rightChar) {
    return (leftChar == '(' and rightChar == ')') or (leftChar == '{' and rightChar == '}') or
           (leftChar == '[' and rightChar == ']') or (leftChar == '<' and rightChar == '>');
}

std::pair<bool, char> isCorrupted(const std::string& str) {
    std::stack<char> stack{};

    for (const char currentChar : str) {
        if (currentChar == '(' or currentChar == '{' or currentChar == '[' or currentChar == '<') {
            stack.push(currentChar);
        } else {
            char lastChar = stack.top();
            stack.pop();

            if (!isMatch(lastChar, currentChar)) return std::make_pair(true, currentChar);
        }
    }

    return std::make_pair(false, '\0');
}

std::string completeString(const std::string& str) {
    std::stack<char> stack{};
    for (const char currentChar : str) {
        if (currentChar == '(' or currentChar == '{' or currentChar == '[' or currentChar == '<') {
            stack.push(currentChar);
        } else {
            stack.pop();
        }
    }

    std::string result{};
    while (!stack.empty()) {
        char lastChar = stack.top();
        stack.pop();

        switch (lastChar) {
            case '(':
                result.push_back(')');
                break;
            case '[':
                result.push_back(']');
                break;
            case '{':
                result.push_back('}');
                break;
            case '<':
                result.push_back('>');
                break;
        }
    }

    return result;
}

unsigned long calculateCompletionScore(const std::string& completion) {
    unsigned long score{0};

    for (char c : completion) {
        score *= 5;
        switch (c) {
            case ')':
                score += 1;
                break;
            case ']':
                score += 2;
                break;
            case '}':
                score += 3;
                break;
            case '>':
                score += 4;
        }
    }

    return score;
}

unsigned long calculateSyntaxErrorScore(const std::vector<std::string>& input) {
    unsigned long score{0};

    for (const auto& line : input) {
        auto corruptedCheck = isCorrupted(line);
        if (corruptedCheck.first) {
            switch (corruptedCheck.second) {
                case ')':
                    score += 3;
                    break;
                case ']':
                    score += 57;
                    break;
                case '}':
                    score += 1197;
                    break;
                case '>':
                    score += 25137;
            }
        }
    }

    return score;
}

unsigned long calculateCompletionScore(const std::vector<std::string>& input) {
    std::vector<unsigned long> scores;

    for (const auto& line : input) {
        auto corruptedCheck = isCorrupted(line);
        if (!corruptedCheck.first) {
            auto lineComplete = completeString(line);
            auto lineScore = calculateCompletionScore(lineComplete);
            scores.push_back(lineScore);
        }
    }

    std::sort(scores.begin(), scores.end());
    return scores[scores.size() / 2];
}

int main(int argc, char* argv[]) {
    try {
        const auto filename = common::getFilename(argc, argv);
        const auto input = common::readInput(filename);

        std::cout << "part 1: " << calculateSyntaxErrorScore(input) << std::endl;
        std::cout << "part 2: " << calculateCompletionScore(input) << std::endl;

        std::cout << sizeof(unsigned long long) << std::endl;

    } catch (std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return 1;
    }

    return 0;
}