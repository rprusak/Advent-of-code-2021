#ifndef STARTER_PACKET_H
#define STARTER_PACKET_H

#include <cstdint>
#include <vector>

class Packet {
  public:
    void setVersion(uint64_t version);
    void setId(uint64_t id);
    void setValue(uint64_t value);
    void addSubPacket(Packet subPacket);

    uint64_t getVersion() const;
    uint64_t getId() const;
    uint64_t getValue() const;
    const std::vector<Packet>& getSubPackets() const;

    bool isLiteralValue() const;

    uint64_t getTotalVersionSum() const;

  private:
    uint64_t calculateValue() const;
    uint64_t calculateSum() const;
    uint64_t calculateProduct() const;
    uint64_t calculateMinimum() const;
    uint64_t calculateMaximum() const;
    uint64_t calculateGraterThan() const;
    uint64_t calculateLessThan() const;
    uint64_t calculateEqual() const;

    uint64_t _version;
    uint64_t _id;
    uint64_t _value;

    std::vector<Packet> _subPackets;
};

#endif  // STARTER_PACKET_H
