//
// Created by Administrator on 2022/8/25 0025.
// C语言只能把程序代码封装成函数。
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// TCP 客户端连接服务端函数，server-ip：服务端IP，port：通信端口
int connect_to_server(const char *server_ip, int port);

int main() {
    // 向服务器发起连接请求
    int sock_file_desc = connect_to_server("192.168.11.100", 5051);
    if (sock_file_desc <= 0) {
        printf("连接服务器失败，程序退出。\n");
        return -1;
    }

    char str_buffer[1024];

    // 与服务端通信，发送一个报文后等待回复，再发送下一个报文
    for (int i = 0; i < 10; i++) {
        memset(str_buffer, 0, sizeof(str_buffer));
        sprintf(str_buffer, "第%d个消息", i + 1);
        if (send(sock_file_desc, str_buffer, strlen(str_buffer), 0) <= 0) {
            break;
        }
        printf("接收：%s\n", str_buffer);
    }
    close(sock_file_desc);
}

// TCP 客户端连接服务端的函数，server_ip：服务端IP，port：通信端口
// 返回值：成功返回已连接socket，失败返回-1
int connect_to_server(const char *server_ip, const int port) {
    // 创建客户端socket
    int sock_file_desc = socket(AF_INET, SOCK_STREAM, 0);
    // IP地址信息的数据结构
    struct hostent *hostent;
    if ((hostent = gethostbyname(server_ip)) == 0) {
        perror("connect_to_server error!");
        // 异常时先关闭socket
        close(sock_file_desc);
        return -1;
    }

    // 把服务器地址和端口转换为数据结构
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr, hostent->h_addr, hostent->h_length);

    // 向服务器发起连接请求
    if ((connect(sock_file_desc, (struct sockaddr *) &server_addr, sizeof(server_addr))) != 0) {
        perror("connect error!");
        close(sock_file_desc);
        return -1;
    }

    return sock_file_desc;
}


