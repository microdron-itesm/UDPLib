//
// Created by abiel on 9/1/20.
//

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <malloc.h>
#include <bits/types/struct_timeval.h>
#include "UDP.h"

int udp_conn_open(udp_conn_data *data, uint16_t sendPort, uint16_t recvPort){
    memset(data, 0, sizeof(udp_conn_data));

    //If port == 0, do not enable
    data->enableRecv = recvPort;
    data->enableSend = sendPort;

    data->sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(data->sock < 0){
        perror("Socket open failed");
        return(-errno);
    }

    int ret = 0;

    if(data->enableSend){
        data->sendAddr.sin_family = AF_INET;
        data->sendAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        data->sendAddr.sin_port = htons(sendPort);
    }

    if(data->enableRecv){
        data->recvAddr.sin_family = AF_INET;
        data->recvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        data->recvAddr.sin_port = htons(recvPort);
        ret = bind(data->sock, (struct sockaddr *) &data->recvAddr, sizeof(struct sockaddr));
        if(ret < 0){
            perror("UDP bind failed");
            close(data->sock);
            return(-errno);
        }

        data->fromLen = sizeof(data->recvAddr);
    }

    ret = fcntl(data->sock, F_SETFL, O_NONBLOCK | O_ASYNC);
    if(ret < 0){
        perror("Nonblocking set failed");
        close(data->sock);
        return(-errno);
    }

    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    ret = setsockopt(data->sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));
    if(ret < 0){
        perror("Nonblocking recv failed");
        close(data->sock);
        return(-errno);
    }

    return(0);
}

int udp_conn_open_ip(udp_conn_data *data, const char * ipAddr, uint16_t sendPort, uint16_t recvPort){
    struct hostent *hp;
    hp = gethostbyname(ipAddr);

    if(!hp){
        perror("Could not obtain address of host");
        return(-errno);
    }

    int ret = udp_conn_open(data, sendPort, recvPort);

    memcpy((void *)&data->sendAddr.sin_addr, hp->h_addr_list[0], hp->h_length);

    return ret;
}


ssize_t udp_conn_send(udp_conn_data *data, uint8_t *buf, size_t len){
    if(data->enableSend){
        return sendto(data->sock, buf, len, 0, (struct sockaddr *) &data->sendAddr, sizeof(struct sockaddr_in));
    } else {
        return -1;
    }
}

ssize_t udp_conn_recv(udp_conn_data *data, uint8_t *buf, size_t bufLen){
    if(data->enableRecv){
        return recvfrom(data->sock, (void *) buf, bufLen, 0, (struct sockaddr *) &data->recvAddr, &data->fromLen);
    } else {
        return -1;
    }
}

int udp_conn_close(udp_conn_data *data){
    return(close(data->sock));
}