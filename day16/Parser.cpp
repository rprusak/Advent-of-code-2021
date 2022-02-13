#include "Parser.h"

namespace {
std::string hexToBin(const std::string& s) {
    std::string out;

    for (auto i : s) {
        uint8_t n;
        if (i <= '9' and i >= '0')
            n = i - '0';
        else
            n = 10 + i - 'A';
        for (int8_t j = 3; j >= 0; --j) out.push_back((n & (1 << j)) ? '1' : '0');
    }

    return out;
}
}  // namespace

Packet Parser::parsePacket(const std::string& packetString) {
    const auto binaryString = hexToBin(packetString);

    _currentIterator = binaryString.begin();
    return parse();
}

Packet Parser::parse() {
    Packet packet;

    packet.setVersion(parseVersion());
    packet.setId(parseId());

    if (packet.isLiteralValue()) {
        packet.setValue(parseLiteralValue());
    } else {
        auto lengthType = *_currentIterator;
        _currentIterator++;

        if (lengthType == '0') {
            const auto length = readSubPacketsLength();
            auto startIt = _currentIterator;

            while (std::distance(startIt, _currentIterator) != (int)length) {
                packet.addSubPacket(parse());
            }
        } else {
            const auto count = readSubPacketsCount();
            for (uint64_t i = 0; i < count; i++) packet.addSubPacket(parse());
        }
    }

    return packet;
}

uint64_t Parser::parseVersion() {
    const std::string binaryVersion(_currentIterator, _currentIterator + 3);
    _currentIterator += 3;
    return std::stoul(binaryVersion, 0, 2);
}
uint64_t Parser::parseId() {
    const std::string binaryVersion(_currentIterator, _currentIterator + 3);
    _currentIterator += 3;
    return std::stoul(binaryVersion, 0, 2);
}

uint64_t Parser::parseLiteralValue() {
    std::string finalValue{};
    char firstBit{0};

    do {
        firstBit = *_currentIterator;
        _currentIterator++;

        finalValue += std::string(_currentIterator, _currentIterator + 4);
        _currentIterator += 4;
    } while (firstBit != '0');

    return std::stoul(finalValue, 0, 2);
}

uint64_t Parser::readSubPacketsLength() {
    std::string length(_currentIterator, _currentIterator + 15);
    _currentIterator += 15;
    return std::stoul(length, 0, 2);
}

uint64_t Parser::readSubPacketsCount() {
    std::string count(_currentIterator, _currentIterator + 11);
    _currentIterator += 11;
    return std::stoul(count, 0, 2);
}
