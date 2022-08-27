//
// Created by Administrator on 2022/8/23 0023.
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
    if (argc != 2) {
        printf("Using:./server port\n Example:./server 5005\n\n");
        return -1;
    }

    // 1. 创建服务端的socket
    int listen_file_desc;
    if ((listen_file_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    // 2. 将服务端用于通信的地址和端口绑定到socket上
    struct sockaddr_in server_addr; // 存储服务端地址信息的数据结构
    memset(&server_addr, 0, sizeof(server_addr));//将server_addr变量重置为0。memset(初始化的内存地址，初始化值，要初始化块的大小)
    server_addr.sin_family = AF_INET;   // 协议族，socket编程中只能是AF_INET
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    // 任意IP地址
//    server_addr.sin_addr.s_addr = inet_addr("192.168.190.134");     // 指定IP地址
    server_addr.sin_port = htons(atoi(argv[1]));    // 指定通信端口
    /*
     * 绑定操作
     * int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
     * sockfd：用socket()创建的文件描述符；
     * my_addr：指向一个结构为sockaddr参数的指针，sockaddr中包含了地址、端口的信息；
     * addrlen：my_addr结构的长度，可以设置为sizeof(struct sockaddr)；
     * 返回值为0时表示绑定成功，-1表示绑定失败
     * */
    if (bind(listen_file_desc, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
        perror("bind");
        // 异常情况return之前，需要关闭socket
        close(listen_file_desc);
        return -1;
    }

    // 3. 把socket设置为监听模式
    /*
     * listen(int sock, int backlog)
     * sock：用socket()创建的文件描述符；
     * backlog：请求队列最大长度；
     * */
    if (listen(listen_file_desc, 5) != 0) {
        perror("listen");
        close(listen_file_desc);
        return -1;
    }

    // 4. 接受客户端的连接
    int client_file_desc;       // 客户端的socket
    int socklen = sizeof(struct sockaddr_in);   // struct sockaddr_in 的大小
    struct sockaddr_in client_addr;     // 客户端的地址信息
    /*
     * int accept(int sock, struct sockaddr *addr, socklen_t *addrlen)
     * sock：用socket()创建的文件描述符；
     * addr：sockaddr_in的结构体变量；
     * socklen：addr的长度；
     * */
    client_file_desc = accept(listen_file_desc, (struct sockaddr *) &client_addr, (socklen_t *) &socklen);
    printf("客户端（%s）已连接。\n", inet_ntoa(client_addr.sin_addr));


    // 5. 与客户端通信，接受客户端发送的报文，回复OK
    char buffer[1024];
    while (true) {
        int receive_result;
        memset(buffer, 0, sizeof(buffer));
        // 接受客户端的请求报文
        /*
         * int recv( SOCKET s, char FAR *buf, int len, int flags)
         * s：用socket()创建的文件描述符；
         * buf：缓冲区，存放recv函数接收到的数据；
         * len：buf的长度；
         * flags：一般置0；
         * */
        if ((receive_result = recv(client_file_desc, buffer, sizeof(buffer), 0)) <= 0) {
            printf("receive_result=%d\n", receive_result);
            break;
        }
        printf("接收：%s\n", buffer);

        strcpy(buffer, "ok");
        // 向客户端发送响应结果
        /*
         * ssize_t send(int sockfd, const void *buf, size_t len, int flags)
         * sockfd：用socket()创建的文件描述符；
         * buf：缓冲区，存放recv函数接收到的数据；
         * len：buf的长度；
         * flags：一般置0；
         * */
        if ((send(client_file_desc, buffer, sizeof(buffer), 0)) <= 0) {
            printf("send");
            break;
        }
        printf("发送：%s\n", buffer);
    }

    // 6. 关闭socket，释放资源
    close(listen_file_desc);
    close(client_file_desc);

}

