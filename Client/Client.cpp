#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")

#define DEFAULT_PORT "1337"

int main() {
    WSADATA wsaData;
    int iResult = 0;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %ld\n", iResult);
        return 1;
    }

    // Creates a socket to connect to server
    SOCKET sock = INVALID_SOCKET;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("Failed to create socket: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set address family,
    // IP address, and port of server
    sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &server_info.sin_addr.s_addr);
    server_info.sin_port = htons(*DEFAULT_PORT);

    // Connect to server
    iResult = connect(sock, (SOCKADDR*)&server_info, sizeof(server_info));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"connect function failed with error: %ld\n", WSAGetLastError());
        iResult = closesocket(sock);
        if (iResult == SOCKET_ERROR)
            wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("Connected to server!\n");
    return 0;
}