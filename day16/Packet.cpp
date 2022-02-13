#include "Packet.h"

#include <stdexcept>

uint64_t Packet::getVersion() const { return _version; }

uint64_t Packet::getId() const { return _id; }

uint64_t Packet::getValue() const { return calculateValue(); };

void Packet::setVersion(uint64_t version) { _version = version; }

void Packet::setId(uint64_t id) { _id = id; }

void Packet::setValue(uint64_t value) { _value = value; }

bool Packet::isLiteralValue() const { return getId() == 4; }

void Packet::addSubPacket(Packet subPacket) { _subPackets.push_back(std::move(subPacket)); }

const std::vector<Packet>& Packet::getSubPackets() const { return _subPackets; }

uint64_t Packet::getTotalVersionSum() const {
    uint64_t sum = _version;

    for (const auto& subPacket : _subPackets) sum += subPacket.getTotalVersionSum();

    return sum;
}

uint64_t Packet::calculateValue() const {
    if (isLiteralValue()) return _value;

    switch (getId()) {
        case 0:
            return calculateSum();
        case 1:
            return calculateProduct();
        case 2:
            return calculateMinimum();
        case 3:
            return calculateMaximum();
        case 5:
            return calculateGraterThan();
        case 6:
            return calculateLessThan();
        case 7:
            return calculateEqual();
        default:
            throw std::runtime_error{"unknown id: " + std::to_string(getId())};
    }
}
uint64_t Packet::calculateSum() const {
    uint64_t sum{0};

    for (const auto& subPacket : _subPackets) sum += subPacket.getValue();

    return sum;
}
uint64_t Packet::calculateProduct() const {
    uint64_t product{_subPackets.front().getValue()};

    for (auto it = _subPackets.begin() + 1; it != _subPackets.end(); it++)
        product *= it->getValue();

    return product;
}
uint64_t Packet::calculateMinimum() const {
    uint64_t minimum{_subPackets.front().getValue()};

    for (auto it = _subPackets.begin() + 1; it != _subPackets.end(); it++) {
        auto newValue = it->getValue();
        if (newValue < minimum) minimum = newValue;
    }

    return minimum;
}
uint64_t Packet::calculateMaximum() const {
    uint64_t maximum{_subPackets.front().getValue()};

    for (auto it = _subPackets.begin() + 1; it != _subPackets.end(); it++) {
        auto newValue = it->getValue();
        if (newValue > maximum) maximum = newValue;
    }

    return maximum;
}
uint64_t Packet::calculateGraterThan() const {
    auto firstValue = _subPackets.begin()->getValue();
    auto secondValue = (_subPackets.begin() + 1)->getValue();

    return firstValue > secondValue ? 1 : 0;
}
uint64_t Packet::calculateLessThan() const {
    auto firstValue = _subPackets.begin()->getValue();
    auto secondValue = (_subPackets.begin() + 1)->getValue();

    return firstValue < secondValue ? 1 : 0;
}
uint64_t Packet::calculateEqual() const {
    auto firstValue = _subPackets.begin()->getValue();
    auto secondValue = (_subPackets.begin() + 1)->getValue();

    return firstValue == secondValue ? 1 : 0;
}
