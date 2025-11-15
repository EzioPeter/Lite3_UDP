#ifndef UDP_COMMAND_SENDER_H
#define UDP_COMMAND_SENDER_H

#include <cstdint>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>

// 结构体定义（与原代码保持一致）
#pragma pack(push, 1)
struct CommandHead {
    uint32_t code;
    uint32_t paramters_size;
    uint32_t type;
};
struct ComplexCmd {
    CommandHead head;
    double velocity;
};
#pragma pack(pop)

class UdpCommandSender {
    private:
        std::string server_ip;       // 服务器IP
        int server_port;             // 服务器端口
        int client_fd;               // Socket文件描述符
        struct sockaddr_in server_addr;  // 服务器地址结构
        socklen_t server_addr_len;    // 地址长度

        // 转换int到uint32_t补码（工具函数）
        uint32_t int_to_uint32_complement(int value);

    public:
        // 构造函数（初始化IP和端口）
        UdpCommandSender(const std::string& ip, int port);
        // 析构函数（关闭Socket）
        ~UdpCommandSender();

        // 初始化Socket（创建并配置服务器地址）
        bool init();

        // 发送ComplexCmd指令
        bool send_complex_cmd(double velocity, uint32_t code = 0x141, uint32_t type = 1);

        // 发送自动模式指令
        bool send_auto_mode(uint32_t code = 0x21010C03, uint32_t paramters_size = 0, uint32_t type = 0);

        // 发送心跳指令
        bool send_heartbeat();

        // 发送停止指令
        bool send_stop_all();
};

#endif // UDP_COMMAND_SENDER_H