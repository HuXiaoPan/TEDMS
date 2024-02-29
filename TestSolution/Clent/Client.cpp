#include <iostream>
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

    //bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)); 客户端不进行bind操作

    connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));

    while (true)
    {
        char buf[1024];
        memset(&buf, 0, sizeof(buf));
        //gets();
        scanf_s("%[^\n]", buf);
        auto c = getchar();
        size_t write_bytes = send(sockfd, buf, sizeof(buf), 0);
        if (write_bytes == -1)
        {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        memset(&buf, 0, sizeof(buf));
        size_t read_bytes = recv(sockfd, buf, sizeof(buf), 0);
        if (read_bytes > 0)
        {
            printf("message from server: %s\n", buf);
        }
        else if (read_bytes == 0)
        {
            printf("server socket disconnected!\n");
            break;
        }
        else if (read_bytes == -1)
        {
            closesocket(sockfd);
        }
    }
    closesocket(sockfd);
}
