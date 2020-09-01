//
// Created by abiel on 9/1/20.
//

#ifndef UDPLIB_UDP_H
#define UDPLIB_UDP_H
#include <stdint.h>
#include <netinet/in.h>
#include <stdbool.h>

typedef struct udp_conn_data_s {
    int sock;
    struct sockaddr_in sendAddr, recvAddr;
    bool enableRecv, enableSend;
    socklen_t fromLen;
} udp_conn_data;

int udp_conn_open(udp_conn_data *data, uint16_t sendPort, uint16_t recevPort);

ssize_t udp_conn_send(udp_conn_data *data, uint8_t *buf, size_t len);

ssize_t udp_conn_recv(udp_conn_data *data, uint8_t *buf, size_t bufLen);

int udp_conn_close(udp_conn_data *data);

#endif //UDPLIB_UDP_H
