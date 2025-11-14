#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#pragma pack(push, 4)

// 原始的结构体
struct RobotStateRecive {
    int robot_basic_state;
    int robot_gait_state;
    double rpy[3];
    double rpy_vel[3];
    double xyz_acc[3];
    double pos_world[3];
    double vel_world[3];
    double vel_body[3];
    unsigned touch_down_and_stair_trot;
    bool is_charging;
    unsigned error_state;
    int robot_motion_state;
    double battery_level;
    int task_state;
    bool is_robot_need_move;
    bool zero_position_flag;
    double ultrasound[2];
};

#pragma pack(pop)

// 新增的外部结构体
struct RobotStateReceived {
    int code;              ///< Command code  
    int size;              ///< Command value
    int cons_code;         ///< Command type
    struct RobotStateRecive data; ///< 包含RobotStateRecive数据
};

// 打印RobotStateRecive结构体的内容
void printRobotState(const RobotStateRecive& state) {
    std::cout << "robot_basic_state: " << std::dec << state.robot_basic_state << std::endl;
    std::cout << "robot_gait_state: " << std::dec << state.robot_gait_state << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << "rpy[" << i << "]: " << state.rpy[i] << std::endl;
    }
    for (int i = 0; i < 3; ++i) {
        std::cout << "rpy_vel[" << i << "]: " << state.rpy_vel[i] << std::endl;
    }
    for (int i = 0; i < 3; ++i) {
        std::cout << "xyz_acc[" << i << "]: " << state.xyz_acc[i] << std::endl;
    }
    for (int i = 0; i < 3; ++i) {
        std::cout << "pos_world[" << i << "]: " << state.pos_world[i] << std::endl;
    }
    for (int i = 0; i < 3; ++i) {
        std::cout << "vel_world[" << i << "]: " << state.vel_world[i] << std::endl;
    }
    for (int i = 0; i < 3; ++i) {
        std::cout << "vel_body[" << i << "]: " << state.vel_body[i] << std::endl;
    }
    std::cout << "touch_down_and_stair_trot: " << state.touch_down_and_stair_trot << std::endl;
    std::cout << "is_charging: " << std::boolalpha << state.is_charging << std::endl;
    std::cout << "error_state: " << state.error_state << std::endl;
    std::cout << "robot_motion_state: " << std::dec << state.robot_motion_state << std::endl;
    std::cout << "battery_level: " << state.battery_level << std::endl;
    std::cout << "task_state: " << state.task_state << std::endl;
    std::cout << "is_robot_need_move: " << std::boolalpha << state.is_robot_need_move << std::endl;
    std::cout << "zero_position_flag: " << state.zero_position_flag << std::endl;
    for (int i = 0; i < 2; ++i) {
        std::cout << "ultrasound[" << i << "]: " << state.ultrasound[i] << std::endl;
    }
}

// 打印RobotStateReceived结构体的内容
void printRobotStateReceived(const RobotStateReceived& stateReceived) {
    std::cout << "Received Code: " << std::hex << stateReceived.code << std::endl;
    std::cout << "Received Size: " << std::dec << stateReceived.size << std::endl;
    std::cout << "Received Cons Code: " << stateReceived.cons_code << std::endl;
    printRobotState(stateReceived.data); // 调用printRobotState打印内部data的内容
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    RobotStateReceived robotStateReceived;

    // 创建UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(43897);
    server_addr.sin_addr.s_addr = inet_addr("192.168.2.1"); // 绑定到指定接口

    // 绑定socket到指定IP地址和端口
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Waiting for messages..." << std::endl;

    while (true) {
        socklen_t client_addr_len = sizeof(client_addr);
        
        // 设置接收缓冲区大小为 RobotStateReceived 结构体的大小
        char buffer[1024];

        ssize_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_len);

        // 仅在接收数据大小等于结构体大小时处理
        if (len == sizeof(RobotStateReceived)) {
            // 将接收到的原始数据复制到 RobotStateReceived 结构体中
            memcpy(&robotStateReceived, buffer, sizeof(RobotStateReceived));
            
            // 仅当 code 为 0x0901 时打印
            if (robotStateReceived.code == 0x0901) {
                printRobotStateReceived(robotStateReceived); // 打印RobotStateReceived结构体的内容
            }
        }
    }

    close(sockfd);
    return 0;
}
