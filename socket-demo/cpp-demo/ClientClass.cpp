/**
 * @Description: 封装socket通信类。
 * C++封装数据和函数，采用的是类。
 * --------------------------------------
 * @FileName: ClientClass.cpp
 * @Date: 2022/8/27 22:40
 * @SoftWare: CLion
 * --------------------------------------
 * @Author: lixj
 * @Contact: lixj_zj@163.com
 **/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

// TCP客户端类
class TcpClient {
public:
    int socket_file_desc;

    TcpClient();

    // 向服务器发起连接，serverIp：服务端IP，port：通信端口
    bool ConnectToServer(const char *serverIp, int port);

    // 向对端发送报文
    int Send(const void *buffer, int bufferLen);

    // 接收对端的报文
    int Recv(void *buffer, int bufferLen);

    ~TcpClient();
};

// 构造函数初始化 socket_file_desc
TcpClient::TcpClient() {
    socket_file_desc = 0;
}

bool TcpClient::ConnectToServer(const char *serverIp, const int port) {
    // 创建客户端的socket
    socket_file_desc = socket(AF_INET, SOCK_STREAM, 0);
    // ip地址信息的数据结构
    struct hostent *hostent;
    if ((hostent = gethostbyname(serverIp)) == 0) {
        close(socket_file_desc);
        socket_file_desc = 0;
        return false;
    }

    // 把服务器的地址和端口转换为数据结构
    struct sockaddr_in server_addr{};
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr, hostent->h_addr, hostent->h_length);

    // 向服务器发起连接请求
    if ((connect(socket_file_desc, (struct sockaddr *) &server_addr, sizeof(server_addr))) != 0) {
        close(socket_file_desc);
        socket_file_desc = 0;
        return false;
    }

    return true;
}

int TcpClient::Send(const void *buffer, const int bufferLen) {
    return send(socket_file_desc, buffer, bufferLen, 0);
}

int TcpClient::Recv(void *buffer, const int bufferLen) {
    return recv(socket_file_desc, buffer, bufferLen, 0);
}

// 析构函数关闭 socket_file_desc
TcpClient::~TcpClient() {
    if (socket_file_desc != 0) {
        close(socket_file_desc);
    }
}


int main() {
    TcpClient tcpClient;
    const char *addr = "192.168.12.100";
    int port = 123;
    // 向服务器发送连接请求
    if (!tcpClient.ConnectToServer(addr, port)) {
        printf("TcpClient.ConnectToServer(\"%s\",%d) failed,exit...\n", addr, port);
        return -1;
    }

    char stringBuffer[1024];
    for (int i = 0; i < 5; i++) {
        memset(stringBuffer, 0, sizeof(stringBuffer));
        sprintf(stringBuffer, "这是第%d条消息..", i++);
        if (tcpClient.Send(stringBuffer, strlen(stringBuffer)) <= 0) {
            break;
        }
        printf("发送：%s\n", stringBuffer);

        memset(stringBuffer, 0, sizeof(stringBuffer));
        if (tcpClient.Recv(stringBuffer, sizeof(stringBuffer)) <= 0) {
            break;
        }
        printf("接收：%s\n", stringBuffer);
    }

}
