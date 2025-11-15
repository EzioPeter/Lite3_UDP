#include "sender.h"
#include <cstring>
#include <iostream>
#include <unistd.h> // 用于usleep

using namespace std;

int main() {
    // 配置服务器IP和端口
    const string SERVER_IP = "192.168.2.1"; // 机器狗IP
    const int SERVER_PORT = 43893;

    // 创建发送器实例
    UdpCommandSender sender(SERVER_IP, SERVER_PORT);
    if (!sender.init()) {
        cerr << "Failed to initialize sender" << endl;
        return -1;
    }

    try {
        cout << "Starting to send commands..." << endl;
        while (true) {
            // 发送ComplexCmd（速度-1.0）
            sender.send_complex_cmd(-1.0);
            usleep(5000); // 5ms

            // 发送自动模式指令
            sender.send_auto_mode();
            usleep(5000); // 5ms
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    // 停止所有指令（程序退出前）
    sender.send_stop_all();
    return 0;
}