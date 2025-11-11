// udp_server.cpp
#include <iostream>
#include <cstring>
#include <arpa/inet.h>  // 网络地址转换相关函数
#include <sys/socket.h> // Socket 核心API
#include <unistd.h>     // close 函数

using namespace std;

// 配置参数
const int PORT = 8888;          // 服务器监听端口
const int BUFFER_SIZE = 1024;   // 数据缓冲区大小

int main() {
    int server_fd;              // 服务器Socket文件描述符
    struct sockaddr_in server_addr, client_addr; // 服务器/客户端地址结构
    socklen_t client_addr_len = sizeof(client_addr); // 客户端地址长度
    char buffer[BUFFER_SIZE] = {0}; // 数据接收缓冲区

    // 1. 创建UDP Socket（SOCK_DGRAM 表示UDP协议）
    // AF_INET：IPv4协议，SOCK_DGRAM：UDP类型，0：默认协议
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation failed"); // 打印错误信息
        exit(EXIT_FAILURE);               // 退出程序
    }
    cout << "UDP Server Socket created successfully" << endl;

    // 2. 配置服务器地址结构
    memset(&server_addr, 0, sizeof(server_addr)); // 初始化地址结构为0
    server_addr.sin_family = AF_INET;             // IPv4协议
    server_addr.sin_addr.s_addr = INADDR_ANY;     // 绑定所有网卡（监听所有IP）
    server_addr.sin_port = htons(PORT);           // 端口转换为网络字节序（大端序）

    // 3. 绑定Socket到指定端口（UDP必须绑定端口才能接收数据）
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        close(server_fd); // 失败时关闭Socket
        exit(EXIT_FAILURE);
    }
    cout << "UDP Server bound to port " << PORT << ", waiting for clients..." << endl;

    // 4. 循环接收客户端数据
    while (true) {
        // 清空缓冲区
        memset(buffer, 0, BUFFER_SIZE);

        // 接收客户端数据（recvfrom 用于UDP，可获取发送方地址）
        // 参数：socket_fd、接收缓冲区、缓冲区大小、标志、发送方地址、地址长度
        ssize_t recv_len = recvfrom(
            server_fd, buffer, BUFFER_SIZE - 1, 0,
            (struct sockaddr*)&client_addr, &client_addr_len
        );

        if (recv_len == -1) {
            perror("recvfrom failed");
            continue; // 接收失败不退出，继续等待下一个数据
        }

        // 打印接收信息（inet_ntoa 转换IP为字符串，ntohs 转换端口为主机字节序）
        cout << "Received from client [" 
             << inet_ntoa(client_addr.sin_addr) << ":" 
             << ntohs(client_addr.sin_port) << "]: " 
             << buffer << endl;

        // 5. 回复客户端（可选，UDP无连接，需指定客户端地址）
        const char* reply_msg = "Server received your data!";
        ssize_t send_len = sendto(
            server_fd, reply_msg, strlen(reply_msg), 0,
            (struct sockaddr*)&client_addr, client_addr_len
        );

        if (send_len == -1) {
            perror("sendto failed");
        } else {
            cout << "Replied to client: " << reply_msg << endl;
        }

        // 退出条件：客户端发送 "exit"
        if (strcmp(buffer, "exit") == 0) {
            cout << "Client requested exit, server closing..." << endl;
            break;
        }
    }

    // 6. 关闭Socket
    close(server_fd);
    cout << "UDP Server closed" << endl;
    return 0;
}
