// Class adapted from the following sources:
// https://docs.microsoft.com/en-us/windows/win32/winsock/using-winsock
// https://stackoverflow.com/questions/34242622/windows-udp-sockets-recvfrom-fails-with-error-10054


#define WIN32_LEAN_AND_MEAN
#ifndef  _WINSOCKAPI_
#define _WINSOCKAPI_
#endif // ! _WINSOCKAPI_
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#pragma once

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define MAX_MESSAGE_LENGTH 1024

class WinsockClient {
public:
    int publishMsg(std::string msg, int port)
    {
        const char* pkt = msg.c_str();
        const char* srcIP = "127.0.0.1";
        const char* destIP = "127.0.0.1";
        sockaddr_in dest;
        sockaddr_in local;
        WSAData data;
        WSAStartup(MAKEWORD(2, 2), &data);

        local.sin_family = AF_INET;
        inet_pton(AF_INET, srcIP, &local.sin_addr.s_addr);
        local.sin_port = htons(0);

        dest.sin_family = AF_INET;
        inet_pton(AF_INET, destIP, &dest.sin_addr.s_addr);
        dest.sin_port = htons(port);

        SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        connect(s, (sockaddr*)&local, sizeof(local));

        sendto(s, pkt, (int)strlen(pkt), 0, (sockaddr*)&dest, sizeof(dest));

        closesocket(s);
        WSACleanup();

        return 0;
    }
    std::string recvMsg(int port) {
        std::string retVal;
        char msg[MAX_MESSAGE_LENGTH] = "";
        const char* srcIP = "127.0.0.1";
        const char* destIP = "127.0.0.1";
        sockaddr_in dest;
        sockaddr_in local;
        WSAData data;
        WSAStartup(MAKEWORD(2, 2), &data);

        local.sin_family = AF_INET;
        inet_pton(AF_INET, srcIP, &local.sin_addr.s_addr);
        local.sin_port = htons(port);

        dest.sin_family = AF_INET;
        inet_pton(AF_INET, destIP, &dest.sin_addr.s_addr);
        dest.sin_port = htons(0);

        SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        bind(s, (sockaddr*)&local, sizeof(local));
        int destSize = sizeof(dest);
        recvfrom(s, msg, sizeof(msg), 0, (sockaddr*)&dest, &destSize);
        for (int i = 0; i < MAX_MESSAGE_LENGTH; i++) {
            if (msg[i] >= 0 && msg[i] <= 255) {
                retVal.push_back(msg[i]);
            }
        }
        closesocket(s);
        WSACleanup();
        return retVal;
    }
};