//
// Created by Administrator on 2022/8/24 0024.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Using:./client ip port\nExample:./client 127.0.0.1 5005\n\n");
        return -1;
    }

    // 1. 创建客户端的socket
    int sock_file_desc;
    if ((sock_file_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    // 2. 向服务器发送连接请求
    struct hostent *h;
    // 指定服务端的IP地址
    if ((h = gethostbyname(argv[1])) == 0) {
        printf("gethostbyname failed.\n");
        close(sock_file_desc);
        return -1;
    }
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);
    // 向服务端发起连接请求
    if (connect(sock_file_desc, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
        perror("connect");
        close(sock_file_desc);
        return -1;
    }

    // 3. 与服务端通信，发送一个报文后等待回复，再发送下一个报文
    char buffer[1024];
    for (int i = 0; i < 3; i++) {
        int receive_result;
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "这是第%d个消息。。。", i + 1);
        // 向服务端发送请求报文
        if ((receive_result = send(sock_file_desc, buffer, strlen(buffer), 0)) <= 0) {
            perror("send");
            break;
        }
        printf("发送：%s\n", buffer);
        // 接收服务端的回应报文
        if ((recv(sock_file_desc, buffer, sizeof(buffer), 0)) <= 0) {
            printf("receive_result=%d\n", receive_result);
            break;
        }
        printf("接收：%s\n", buffer);
    }

    // 4. 关闭socket，释放资源
    close(sock_file_desc);

}






