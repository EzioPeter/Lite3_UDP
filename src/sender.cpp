#include "sender.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <unistd.h> // 用于usleep

using namespace std;

// 工具函数：int转uint32_t补码
uint32_t UdpCommandSender::int_to_uint32_complement(int value) {
    return static_cast<uint32_t>(value);
}

// 构造函数：初始化IP和端口
UdpCommandSender::UdpCommandSender(const string& ip, int port)
    : server_ip(ip),
      server_port(port),
      client_fd(-1),
      server_addr_len(sizeof(server_addr))
{
    memset(&server_addr, 0, sizeof(server_addr));
}


// 析构函数：关闭Socket
UdpCommandSender::~UdpCommandSender() {
    if (client_fd != -1) {
        close(client_fd);
        cout << "UDP Client closed" << endl;
    }
}

// 初始化Socket
bool UdpCommandSender::init() {
    // 创建UDP Socket
    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation failed");
        return false;
    }
    cout << "UDP Client Socket created successfully" << endl;

    // 配置服务器地址结构
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    // 转换IP为网络字节序
    if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0) {
        perror("invalid server IP address");
        close(client_fd);
        client_fd = -1;
        return false;
    }
    return true;
}

// 发送ComplexCmd指令
bool UdpCommandSender::send_complex_cmd(double velocity, uint32_t code, uint32_t type) {
    if (client_fd == -1) {
        cerr << "Socket not initialized" << endl;
        return false;
    }

    ComplexCmd cmd = {0};
    cmd.velocity = velocity;
    cmd.head.code = code;
    cmd.head.paramters_size = sizeof(cmd.velocity);
    cmd.head.type = type;

    ssize_t send_len = sendto(
        client_fd,
        &cmd,
        sizeof(cmd.head) + cmd.head.paramters_size,
        0,
        (struct sockaddr*)&server_addr,
        server_addr_len
    );

    if (send_len == -1) {
        perror("send_complex_cmd failed");
        return false;
    }
    usleep(500);
    return true;
}

// 不传参默认发送自动模式指令，传参则可认为是发送SimpleCmd指令
bool UdpCommandSender::send_auto_mode(uint32_t code, uint32_t paramters_size, uint32_t type) {
    if (client_fd == -1) {
        cerr << "Socket not initialized" << endl;
        return false;
    }

    CommandHead auto_mode = {0};
    auto_mode.code = code;
    auto_mode.paramters_size = paramters_size;
    auto_mode.type = type;

    ssize_t send_len = sendto(
        client_fd,
        &auto_mode,
        sizeof(auto_mode),
        0,
        (struct sockaddr*)&server_addr,
        server_addr_len
    );

    if (send_len == -1) {
        perror("send_auto_mode failed");
        return false;
    }
    usleep(500);
    return true;
}

// 发送心跳指令
bool UdpCommandSender::send_heartbeat() {
    return send_auto_mode(0x21040001, 0, 0); // 复用自动模式发送函数，指定心跳指令码
}

// 发送停止指令
bool UdpCommandSender::send_stop_all() {
    // 这里假设停止指令需要发送多个0速度指令（根据实际需求调整）
    bool ret1 = send_complex_cmd(0.0); // 停止velocity
    bool ret2 = send_auto_mode(0x21010130, 0, 0); // 停止前进
    return ret1 && ret2;
}