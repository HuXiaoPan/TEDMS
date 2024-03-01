#if 0

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

#else

#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#define MAX_BUFF_SIZE 1024

#pragma comment(lib,"Mswsock.lib")

// 定义重叠结构体
typedef struct _PER_IO_OPERATION_DATA
{
    WSAOVERLAPPED Overlapped;
    WSABUF DataBuf;
    CHAR Buffer[MAX_BUFF_SIZE];
    DWORD BytesSend;
    DWORD BytesRecv;
} PER_IO_OPERATION_DATA, * LPPER_IO_OPERATION_DATA;

enum class IO_OP_TYPE
{
    IO_ACCEPT,
    IO_SEND,
    IO_RECV,
    IO_CONN,
    IO_DISCONN,
};

typedef struct OverlappedPerIO
{
    OVERLAPPED overlapped;
    SOCKET socket;
    WSABUF buf;
    IO_OP_TYPE op_type;
    char buffer[MAX_BUFF_SIZE];
}*LPOverlappedPerIO;

struct SocketParam
{
    SOCKET listenSocket;
    HANDLE iocp;
};

int initServer(SocketParam &param)
{
    // 初始化Windows Socket库
    WSAData wsaData;
    int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (!ret)
    {
        param.listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
        if (param.listenSocket != INVALID_SOCKET)
        {
            sockaddr_in serverAddr;
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_addr.s_addr = INADDR_ANY;
            serverAddr.sin_port = htons(8888);

            ret = bind(param.listenSocket, (const sockaddr*)&serverAddr, sizeof(serverAddr));
            if (!ret)
            {
                ret = listen(param.listenSocket, SOMAXCONN);
                if (!ret)
                {
                    //param.iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
                    param.iocp = CreateIoCompletionPort((HANDLE)param.listenSocket, 0, 89757, 0);
                    if (param.iocp)
                    {
                        return 0;
                    }
					std::cout << "CreateIoCompletionPort listen socket failed! " << std::endl;
					CloseHandle(param.iocp);
                }
            }
            closesocket(param.listenSocket);
        }
        WSACleanup();
    }
    return -1;
}
int PostAcceptEx(SOCKET& listenSocket)
{
    SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    OverlappedPerIO* overlapped = new OverlappedPerIO;
    ZeroMemory(overlapped, sizeof(OverlappedPerIO));

    overlapped->socket = sock;
    overlapped->buf.buf = overlapped->buffer;
    overlapped->buf.len = MAX_BUFF_SIZE;
    overlapped->op_type = IO_OP_TYPE::IO_ACCEPT;
    DWORD dwByteRecv = 0;
    while (!AcceptEx(listenSocket, sock, overlapped->buf.buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwByteRecv, (LPOVERLAPPED)overlapped))
    {
        if (WSAGetLastError() == WSA_IO_PENDING)
        {
            break;
        }
    }
    return 0;
}
int main()
{
    int ret;
    SocketParam p{ 0 };
    ret = initServer(p);
    if (ret)
    {
        return 0;
    }
    PostAcceptEx(p.listenSocket);
    DWORD bytesTrans;
    ULONG_PTR completionKey;
    LPOverlappedPerIO overlapped;
    int i = 0;
    int k = 0;

    auto c = getchar();
     while (true)
    {
        i++;
        if (i > 5000000)
        {
            std::cout << "loop " << k++ << std::endl;
            i = 0;
        }
        bool result = GetQueuedCompletionStatus(p.iocp, &bytesTrans, &completionKey, (LPOVERLAPPED*)&overlapped, 0);
        if(overlapped) std::cout << "type " << (int)overlapped->op_type << std::endl;
        if (!result)
        {
            if (GetLastError() == WAIT_TIMEOUT || GetLastError() == ERROR_NETNAME_DELETED)
            {
                if(overlapped) closesocket(overlapped->socket);
                delete overlapped;
            }
            continue;
        }
        switch (overlapped->op_type)
        {
        case IO_OP_TYPE::IO_ACCEPT:
        {
            PostAcceptEx(p.listenSocket);
            setsockopt(overlapped->socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)(&p.listenSocket), sizeof(SOCKET));
            printf("new client fd %d! IP: %s\n", overlapped->socket, overlapped->buffer);
            ZeroMemory(overlapped->buffer, MAX_BUFF_SIZE);
            overlapped->buf.buf = overlapped->buffer;
            overlapped->buf.len = MAX_BUFF_SIZE;
            overlapped->op_type = IO_OP_TYPE::IO_RECV;
            if (!CreateIoCompletionPort((HANDLE)overlapped->socket, p.iocp, 901106, 0))
            {
                std::cout << "CreateIoCompletionPort failed" << std::endl;
            }
            DWORD dwRecv = 0, dwFlag = 0;
            int r = WSARecv(overlapped->socket, &overlapped->buf, 1, &dwRecv, &dwFlag, &overlapped->overlapped, 0);
            if (r == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
            {
                std::cout << "WSARecv failed : " << WSAGetLastError() << std::endl;
            }

        }
        break;
        case IO_OP_TYPE::IO_SEND:
        {
            std::cout << "Happened send : " << bytesTrans << std::endl;
            if (bytesTrans < 1)
            {
                std::cout << "closesocket : " << overlapped->socket << std::endl;
                if (overlapped) closesocket(overlapped->socket);
                delete overlapped;
                continue;
            }
            std::cout << "Send Data: " << overlapped->buffer << std::endl;

            DWORD dwRecv = 0, dwFlag = 0;

            ZeroMemory(overlapped->buffer, MAX_BUFF_SIZE);
            overlapped->buf.buf = overlapped->buffer;
            overlapped->buf.len = MAX_BUFF_SIZE;
            overlapped->op_type = IO_OP_TYPE::IO_RECV;

            int r = WSARecv(overlapped->socket, &overlapped->buf, 1, &dwRecv, &dwFlag, &overlapped->overlapped, 0);
            if (r == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
            {
                std::cout << "WSARecv failed : " << WSAGetLastError() << std::endl;
            }


        }
        break;
        case IO_OP_TYPE::IO_RECV:
        {
            std::cout << "Happened recv : " << bytesTrans << std::endl;
            if (bytesTrans < 1)
            {
                std::cout << "closesocket : " << overlapped->socket << std::endl;
                if (overlapped) closesocket(overlapped->socket);
                delete overlapped;
                continue;
            }
            std::cout << "Recv Data: " << overlapped->buffer << std::endl;

            ZeroMemory(overlapped->buffer, MAX_BUFF_SIZE);
            strcpy_s(overlapped->buffer, "Server is here");
            overlapped->buf.buf = overlapped->buffer;
            overlapped->buf.len = MAX_BUFF_SIZE;
            overlapped->op_type = IO_OP_TYPE::IO_SEND;
            DWORD dwSend = 0;
            int r = WSASend(overlapped->socket, &overlapped->buf, 1, &dwSend, 0, &overlapped->overlapped, 0);
            if (r == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
            {
                std::cout << "WSARecv failed : " << WSAGetLastError() << std::endl;
            }
        }
        break;
        default:
            break;
        }
    }
    //// 初始化Windows Socket库
    //WSAData wsaData;
    //int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    //// 创建socket监听客户端连接
    //SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //sockaddr_in serverAddr;
    //serverAddr.sin_family = AF_INET;
    //inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    //serverAddr.sin_port = htons(8888);

    //// 绑定地址和端口
    //bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    //// 开始监听
    //listen(listenSocket, SOMAXCONN);

    //// 创建IOCP句柄
    //HANDLE iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    //// 将监听socket关联到IOCP上
    //CreateIoCompletionPort((HANDLE)listenSocket, iocpHandle, 0, 0);

    //std::cout << "Server started." << std::endl;

    //while (true)
    //{
    //    sockaddr_in clientAddr;
    //    int addrLen = sizeof(clientAddr);

    //    // 接收客户端连接
    //    SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
    //    if (clientSocket == INVALID_SOCKET) continue;

    //    // 将客户端socket关联到IOCP上
    //    HANDLE clientHandle = CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle, 0, 0);

    //    // 创建重叠结构体
    //    LPPER_IO_OPERATION_DATA perIoData = new PER_IO_OPERATION_DATA();
    //    memset(&(perIoData->Overlapped), 0, sizeof(WSAOVERLAPPED));
    //    perIoData->DataBuf.len = MAX_BUFF_SIZE;
    //    perIoData->DataBuf.buf = perIoData->Buffer;

    //    DWORD bytesRecv = 0;
    //    DWORD flags = 0;

    //    // 接收客户端数据
    //    if (WSARecv(clientSocket, &(perIoData->DataBuf), 1, &bytesRecv, &flags, &(perIoData->Overlapped), NULL) == SOCKET_ERROR)
    //    {
    //        if (WSAGetLastError() != WSA_IO_PENDING)
    //        {
    //            std::cout << "WSARecv failed: " << WSAGetLastError() << std::endl;
    //            delete perIoData;
    //            closesocket(clientSocket);
    //            continue;
    //        }
    //    }
    //    printf("message from client fd %d: %s\n", clientSocket, perIoData->DataBuf.buf);
    //    char str[50];
    //    inet_ntop(AF_INET, &clientAddr.sin_addr, str, 50);
    //    printf("new client fd %d! IP: %s Port: %d\n", clientSocket, str, ntohs(clientAddr.sin_port));
    //}

    //CloseHandle(iocpHandle);
    //closesocket(listenSocket);
    //WSACleanup();

    //return 0;
}
#endif // 0
