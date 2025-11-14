// udp_client.cpp
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#pragma pack(push, 1)
struct CommandHead{ 
    uint32_t code;
    uint32_t paramters_size;
    uint32_t type;
};
struct ComplexCmd
{
    CommandHead head;
    double velocity;
};
#pragma pack(pop)

// 配置参数
const char* SERVER_IP = "192.168.2.1"; // 服务器IP（狗用）
// const char* SERVER_IP = "127.0.0.1"; // 服务器IP（本地测试用）
const int SERVER_PORT = 43893;        // 服务器端口（需与服务器一致）

uint32_t int_to_uint32_complement(int value) {
    return static_cast<uint32_t>(value);  // 编译器自动处理补码转换
}

int main() {
    int client_fd;                      // 客户端Socket文件描述符
    struct sockaddr_in server_addr;     // 服务器地址结构
    socklen_t server_addr_len = sizeof(server_addr);

    struct ComplexCmd cmd = {0};
    cmd.velocity = -1.0;
    cmd.head.code = 0x141;
    cmd.head.paramters_size = sizeof(cmd.velocity);
    cmd.head.type = 1;

    struct CommandHead auto_mode = {0};
    auto_mode.code = 0x21010C03;
    auto_mode.paramters_size = 0;
    auto_mode.type = 0;
    
    // 1. 创建UDP Socket
    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    cout << "UDP Client Socket created successfully" << endl;

    // 2. 配置服务器地址结构
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // 将服务器IP字符串转换为网络字节序
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("invalid server IP address");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    while(true){
        // 发送数据
        ssize_t send_len = sendto(
            client_fd,
            &cmd,                  // 发送指令结构体
            sizeof(cmd.head) + cmd.head.paramters_size,           // 长度为结构体大小
            0,
            (struct sockaddr*)&server_addr,
            server_addr_len
        );
        if (send_len == -1) {
            perror("sendto failed");
            close(client_fd);
            exit(EXIT_FAILURE);
        }        
        usleep(5000);
        // 发送数据
        ssize_t send_len_auto = sendto(
            client_fd,
            &auto_mode,                  // 发送指令结构体
            sizeof(auto_mode),           // 长度为结构体大小
            0,
            (struct sockaddr*)&server_addr,
            server_addr_len
        );
        if (send_len_auto == -1) {
            perror("sendto failed");
            close(client_fd);
            exit(EXIT_FAILURE);
        }        
        usleep(5000);
    }
    // 关闭Socket
    close(client_fd);
    cout << "UDP Client closed" << endl;
    return 0;
}