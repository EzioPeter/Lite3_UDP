// udp_client.cpp
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

// 配置参数
const char* SERVER_IP = "127.0.0.1"; // 服务器IP（本地测试用回环地址）
const int SERVER_PORT = 8888;        // 服务器端口（需与服务器一致）
const int BUFFER_SIZE = 1024;        // 数据缓冲区大小

int main() {
    int client_fd;                      // 客户端Socket文件描述符
    struct sockaddr_in server_addr;     // 服务器地址结构
    socklen_t server_addr_len = sizeof(server_addr);
    char send_buffer[BUFFER_SIZE] = {0};
    char recv_buffer[BUFFER_SIZE] = {0};

    // 1. 创建UDP Socket（与服务器端一致）
    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    cout << "UDP Client Socket created successfully" << endl;

    // 2. 配置服务器地址结构
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT); // 服务器端口转换为网络字节序

    // 将服务器IP字符串转换为网络字节序（inet_pton 支持IPv4/IPv6）
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("invalid server IP address");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // 3. 循环发送数据（从标准输入读取）
    while (true) {
        // 清空发送缓冲区
        memset(send_buffer, 0, BUFFER_SIZE);

        // 提示用户输入
        cout << "Please enter message to send (type 'exit' to quit): ";
        cin.getline(send_buffer, BUFFER_SIZE - 1); // 读取用户输入（支持空格）

        // 发送数据到服务器
        ssize_t send_len = sendto(
            client_fd, send_buffer, strlen(send_buffer), 0,
            (struct sockaddr*)&server_addr, server_addr_len
        );

        if (send_len == -1) {
            perror("sendto failed");
            continue;
        }
        cout << "Sent to server: " << send_buffer << endl;

        // 退出条件：用户输入 "exit"
        if (strcmp(send_buffer, "exit") == 0) {
            cout << "Client exiting..." << endl;
            break;
        }

        // 4. 接收服务器回复
        memset(recv_buffer, 0, BUFFER_SIZE);
        ssize_t recv_len = recvfrom(
            client_fd, recv_buffer, BUFFER_SIZE - 1, 0,
            nullptr, nullptr // 不需要获取服务器地址，填NULL
        );

        if (recv_len == -1) {
            perror("recvfrom failed");
        } else {
            cout << "Received from server: " << recv_buffer << endl;
        }

        cout << "------------------------" << endl;
    }

    // 5. 关闭Socket
    close(client_fd);
    cout << "UDP Client closed" << endl;
    return 0;
}
