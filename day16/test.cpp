#include <gtest/gtest.h>

#include "Parser.h"

TEST(Parser, correctlyParsesVersion) {
    Parser parser;
    auto packet = parser.parsePacket("D2FE28");

    ASSERT_EQ(packet.getVersion(), 6u);
}

TEST(Parser, correctlyParsesId) {
    Parser parser;
    auto packet = parser.parsePacket("D2FE28");

    ASSERT_EQ(packet.getId(), 4u);
}

TEST(Parser, correctlyParsesLiteralValue) {
    Parser parser;
    auto packet = parser.parsePacket("D2FE28");
    ASSERT_TRUE(packet.isLiteralValue());
    ASSERT_EQ(packet.getValue(), 2021u);
}

TEST(Parser, correctlyParsesPacketWithTwoSubpackets) {
    Parser parser;
    auto packet = parser.parsePacket("38006F45291200");

    ASSERT_TRUE(!packet.isLiteralValue());
    ASSERT_EQ(packet.getSubPackets().size(), 2u);

    ASSERT_TRUE(packet.getSubPackets()[0].isLiteralValue());
    ASSERT_EQ(packet.getSubPackets()[0].getValue(), 10u);

    ASSERT_TRUE(packet.getSubPackets()[1].isLiteralValue());
    ASSERT_EQ(packet.getSubPackets()[1].getValue(), 20u);
}

TEST(Parser, correctlyParsesPacketWithThreeSubpackets) {
    Parser parser;
    auto packet = parser.parsePacket("EE00D40C823060");

    ASSERT_TRUE(!packet.isLiteralValue());
    ASSERT_EQ(packet.getSubPackets().size(), 3u);

    ASSERT_TRUE(packet.getSubPackets()[0].isLiteralValue());
    ASSERT_EQ(packet.getSubPackets()[0].getValue(), 1u);

    ASSERT_TRUE(packet.getSubPackets()[1].isLiteralValue());
    ASSERT_EQ(packet.getSubPackets()[1].getValue(), 2u);

    ASSERT_TRUE(packet.getSubPackets()[2].isLiteralValue());
    ASSERT_EQ(packet.getSubPackets()[2].getValue(), 3u);
}

TEST(Parser, correctlyParsesPacketsHierarchy) {
    Parser parser;
    auto packet = parser.parsePacket("8A004A801A8002F478");

    ASSERT_TRUE(!packet.isLiteralValue());
    ASSERT_EQ(packet.getVersion(), 4u);
    ASSERT_EQ(packet.getSubPackets().size(), 1u);

    ASSERT_TRUE(!packet.getSubPackets().front().isLiteralValue());
    ASSERT_EQ(packet.getSubPackets().front().getVersion(), 1u);
    ASSERT_EQ(packet.getSubPackets().front().getSubPackets().size(), 1u);

    ASSERT_TRUE(!packet.getSubPackets().front().getSubPackets().front().isLiteralValue());
    ASSERT_EQ(packet.getSubPackets().front().getSubPackets().front().getVersion(), 5u);
    ASSERT_EQ(packet.getSubPackets().front().getSubPackets().front().getSubPackets().size(), 1u);

    ASSERT_TRUE(packet.getSubPackets()
                    .front()
                    .getSubPackets()
                    .front()
                    .getSubPackets()
                    .front()
                    .isLiteralValue());
    ASSERT_EQ(
        packet.getSubPackets().front().getSubPackets().front().getSubPackets().front().getVersion(),
        6u);
    ASSERT_EQ(packet.getSubPackets()
                  .front()
                  .getSubPackets()
                  .front()
                  .getSubPackets()
                  .front()
                  .getSubPackets()
                  .size(),
              0u);
}

TEST(Packet, correctlyCalculatesTotalVersionSum) {
    Parser parser;
    ASSERT_EQ(parser.parsePacket("8A004A801A8002F478").getTotalVersionSum(), 16u);
    ASSERT_EQ(parser.parsePacket("620080001611562C8802118E34").getTotalVersionSum(), 12u);
    ASSERT_EQ(parser.parsePacket("C0015000016115A2E0802F182340").getTotalVersionSum(), 23u);
    ASSERT_EQ(parser.parsePacket("A0016C880162017C3686B18A3D4780").getTotalVersionSum(), 31u);
}

TEST(Packet, correctlyCalculatesValue) {
    Parser parser;
    ASSERT_EQ(parser.parsePacket("C200B40A82").getValue(), 3u);
    ASSERT_EQ(parser.parsePacket("04005AC33890").getValue(), 54u);
    ASSERT_EQ(parser.parsePacket("880086C3E88112").getValue(), 7u);
    ASSERT_EQ(parser.parsePacket("CE00C43D881120").getValue(), 9u);
    ASSERT_EQ(parser.parsePacket("D8005AC2A8F0").getValue(), 1u);
    ASSERT_EQ(parser.parsePacket("F600BC2D8F").getValue(), 0u);
    ASSERT_EQ(parser.parsePacket("9C005AC2F8F0").getValue(), 0u);
    ASSERT_EQ(parser.parsePacket("9C0141080250320F1802104A08").getValue(), 1u);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}