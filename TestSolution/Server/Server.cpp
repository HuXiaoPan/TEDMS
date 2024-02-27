﻿#include <iostream>
#include <WS2tcpip.h>

int main()
{
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData))
    {
        return 0;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    int ra = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(8888);

    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(sockfd, SOMAXCONN);

    struct sockaddr_in clnt_addr;
    /*socklen_t*/int clnt_addr_len = sizeof(clnt_addr);
    memset(&clnt_addr, 0, sizeof(clnt_addr));

    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    char str[50];
    inet_ntop(AF_INET, &clnt_addr.sin_addr, str, 50);
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, str, ntohs(clnt_addr.sin_port));

    while (true)
    {
        char buf[1024];
        memset(&buf, 0, sizeof(buf));
        size_t read_bytes = recv(clnt_sockfd, buf, sizeof(buf), 0);
        if (read_bytes > 0)
        {
            printf("message from client fd %d: %s\n", clnt_sockfd, buf);
            send(clnt_sockfd, buf, sizeof(buf), 0);
        }
        else if (read_bytes == 0)
        {
            printf("client fd %d disconnected\n", clnt_sockfd);
            closesocket(clnt_sockfd);
            break;
        }
        else if (read_bytes == -1)
        {
            closesocket(clnt_sockfd);
        }
    }
    closesocket(sockfd);
    return 0;
}