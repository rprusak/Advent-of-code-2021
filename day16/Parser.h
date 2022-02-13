#ifndef STARTER_PARSER_H
#define STARTER_PARSER_H

#include <string>

#include "Packet.h"

class Parser {
  public:
    Packet parsePacket(const std::string& packetString);

  private:
    Packet parse();

    uint64_t parseVersion();
    uint64_t parseId();
    uint64_t parseLiteralValue();
    uint64_t readSubPacketsLength();
    uint64_t readSubPacketsCount();

    std::string::const_iterator _currentIterator;
};

#endif  // STARTER_PARSER_H
