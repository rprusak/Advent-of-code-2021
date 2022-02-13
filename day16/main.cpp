#include <iostream>

#include "Packet.h"
#include "Parser.h"
#include "common.h"

int main(int argc, char *argv[]) {
    try {
        const auto inputFilename = common::getFilename(argc, argv);
        const auto input = common::readInput(inputFilename);

        Parser parser;

        const auto packet = parser.parsePacket(input.front());
        std::cout << "part 1: " << packet.getTotalVersionSum() << std::endl;
        std::cout << "part 2: " << packet.getValue() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}