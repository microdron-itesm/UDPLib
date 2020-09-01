//
// Created by abiel on 9/1/20.
//

#include "gtest/gtest.h"

extern "C" {
#include "UDP.h"
};

static const uint16_t defPort = 9000;

TEST(UDPTest, OpenCloseTest){
    udp_conn_data data;

    int ret = udp_conn_open(&data, defPort, defPort);
    ASSERT_FALSE(ret < 0);

    ASSERT_TRUE(data.sock);

    ret = udp_conn_close(&data);
    ASSERT_FALSE(ret < 0);
}

TEST(UDPTest, DisabledRecevAndSendTest){
    udp_conn_data data;

    int ret = udp_conn_open(&data, 0, 0);
    ASSERT_FALSE(data.enableSend);
    ASSERT_FALSE(data.enableRecv);
    ASSERT_FALSE(ret < 0);

    ret = udp_conn_close(&data);
    ASSERT_FALSE(ret < 0);

    ret = udp_conn_open(&data, defPort, defPort);
    ASSERT_FALSE(ret < 0);
    ASSERT_TRUE(data.enableSend);
    ASSERT_TRUE(data.enableRecv);

    ret = udp_conn_close(&data);
    ASSERT_FALSE(ret < 0);
}

TEST(UDPTest, SendRecvTest){
    udp_conn_data data;

    udp_conn_open(&data, defPort, defPort);

    uint8_t testString[] = "HelloWorld";

    ssize_t ret = udp_conn_send(&data, testString, sizeof(testString));
    ASSERT_EQ(ret, sizeof(testString));

    uint8_t testBuf[256];
    ret = udp_conn_recv(&data, testBuf, 256);
    ASSERT_EQ(ret, sizeof(testString));
    ASSERT_EQ(memcmp(testString, testBuf, sizeof(testString)), 0);

    udp_conn_close(&data);
}