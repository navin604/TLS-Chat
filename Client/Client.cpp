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

    int recvbuflen = DEFAULT_BUFLEN;
    char recvbuf[DEFAULT_BUFLEN];
    const char* msg = "This is a test message";

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
    
    // Send message to client
    iResult = send(sock, msg, strlen(msg), 0);
    if (iResult == SOCKET_ERROR) {
        printf("Failed to send message: %d\n", WSAGetLastError());
        iResult = closesocket(sock);
        if (iResult == SOCKET_ERROR)
            wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("Bytes sent: %d\n", iResult);

    //Disable send on the socket
    iResult = shutdown(sock, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("Failed to shutdown: %d\n", WSAGetLastError());
        iResult = closesocket(sock);
        if (iResult == SOCKET_ERROR)
            wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }


    do {
        iResult = recv(sock, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
        }
        else if (iResult == 0) {
            printf("Connection closed\n");
        }
        else
            printf("recv failed: %d \n", WSAGetLastError());
    } while (iResult > 0);


    //Disable recv on the socket
    iResult = shutdown(sock, SD_RECEIVE);
    if (iResult == SOCKET_ERROR) {
        printf("Failed to shutdown: %d\n", WSAGetLastError());
        iResult = closesocket(sock);
        if (iResult == SOCKET_ERROR)
            wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }



    // Output recv message
    printf("Message received: %s\n", recvbuf);

    // cleanup
    closesocket(sock);
    WSACleanup();



    return 0;
}