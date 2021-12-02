#include <algorithm>
#include <iostream>
#include <regex>

#include "common.h"

struct Command {
    enum class Direction { FORWARD, DOWN, UP };

    Direction direction;
    unsigned long value;

    void execute(unsigned& horizontal, unsigned& depth) const {
        if (direction == Direction::FORWARD)
            horizontal += value;
        else if (direction == Direction::DOWN)
            depth += value;
        else  // UP
            depth -= value;
    }

    void execute(unsigned& horizontal, unsigned& depth, unsigned& aim) const {
        if (direction == Direction::FORWARD) {
            horizontal += value;
            depth += aim * value;
        } else if (direction == Direction::DOWN) {
            aim += value;
        } else {  // UP
            aim -= value;
        }
    }
};

const std::regex forwardPattern{R"#(forward (\d))#"};
const std::regex downPattern{R"#(down (\d))#"};
const std::regex upPattern{R"#(up (\d))#"};

Command parseCommand(const std::string& str) {
    std::smatch matchResult;
    if (std::regex_match(str, matchResult, forwardPattern)) {
        return Command{Command::Direction::FORWARD, std::stoul(matchResult[1].str())};
    } else if (std::regex_match(str, matchResult, downPattern)) {
        return Command{Command::Direction::DOWN, std::stoul(matchResult[1].str())};
    } else if (std::regex_match(str, matchResult, upPattern)) {
        return Command{Command::Direction::UP, std::stoul(matchResult[1].str())};
    } else {
        throw std::runtime_error{"Could not parse " + str};
    }
}

std::vector<Command> parseCommands(const std::vector<std::string>& input) {
    std::vector<Command> commands;

    std::transform(input.begin(), input.end(), std::back_inserter(commands), parseCommand);

    return commands;
}

std::pair<unsigned, unsigned> executeCommands(const std::vector<Command>& commands) {
    unsigned horizontal{0};
    unsigned depth{0};

    for (const auto& command : commands) command.execute(horizontal, depth);

    return std::make_pair(horizontal, depth);
}

std::pair<unsigned, unsigned> executeCommandsWithAim(const std::vector<Command>& commands) {
    unsigned horizontal{0};
    unsigned depth{0};
    unsigned aim{0};

    for (const auto& command : commands) command.execute(horizontal, depth, aim);

    return std::make_pair(horizontal, depth);
}

int main(int argc, char* argv[]) {
    try {
        const auto filename = common::getFilename(argc, argv);
        const auto input = common::readInput(filename);

        const auto commands = parseCommands(input);
        const auto [horizontal, depth] = executeCommands(commands);

        std::cout << "part 1: " << horizontal * depth << std::endl;

        const auto [horizontal2, depth2] = executeCommandsWithAim(commands);

        std::cout << "part 2: " << horizontal2 * depth2 << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}