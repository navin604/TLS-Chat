#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>

#pragma comment(lib,"Ws2_32.lib")

#define DEFAULT_BUFLEN 512
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

    // Set sockets
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    // Create socket to listen for connections.
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket function failed with error: %u\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set address family,
    // IP address, and port of server.
    sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &server_info.sin_addr.s_addr);
    server_info.sin_port = htons(*DEFAULT_PORT);

    // Bind socket.
    iResult = bind(ListenSocket, (SOCKADDR *) &server_info, sizeof (server_info));
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    printf("Bind returned successfully\n");

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    printf("Listening on %d:%d\n", server_info.sin_addr.s_addr,server_info.sin_port );

    sockaddr client;
    int sizeclient = sizeof(client);

    // Accept a connection
    ClientSocket = accept(ListenSocket, (sockaddr *) &client, &sizeclient);
    if (ClientSocket == INVALID_SOCKET) {
        printf("Failed to accept with errorL %d\n", WSAGetLastError());
        iResult = closesocket(ClientSocket);
        if (iResult == SOCKET_ERROR)
            wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("Connected client info: %p\n", &client.sa_data);







    return 0;
}