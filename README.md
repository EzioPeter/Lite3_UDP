# Lite3_UDP

Lite3_UDP is a repository for the DeepRobotics Lite3 quadruped robot. It contains the code for the UDP communication between the robot motion computer and the developer computer. The code is written in C++ and follows the agreement of the Lite3 quadruped robot in the motion SDK manual. The motion SDK manual needs to contact the staff of DeepRobotics.

## Motivation

Due to the lack of the official motion SDK code and unclear expression of the motion SDK manual, I decided to write this code to help others who are using the Lite3 quadruped robot.

The code follows the agreement of the motion SDK manual and it use the UDP protocol to communicate with the robot motion computer, rather than using ROS interface. If you want to use the ROS interface, you can login the preception computer and publish your command. 

## Usage

1. Compile the code.

   ```bash
   cd Lite3_UDP
   mkdir build
   cd build
   cmake ..
   make
   ```
2. Modify the IP address in the code, and ping the IP address to make sure the IP address is correct.

   ```c++
   // wireless
   const string SERVER_IP = "192.168.2.1";
   // ethernet
   // const string SERVER_IP = "192.168.1.120";
   ```

3. Run the code.

   ```bash
   # For test, run this command first
   ./complex_cmd
   # For your designed use, run this command
   ./lite3_command
   ```

## Problems

1. When you first start the quadruped robot and there are no other operations, UDP may not work. You need to login the perception computer and publish the command for once, waiting for the rotation of the quadruped robot. Then you stop the publishing and run the code again. It may be a bug of the Lite3 quadruped robot.
    ```bash
    rostopic pub /cmd_vel geometry_msgs/Twist -r 10 "linear:
        x: 0.0
        y: 0.0
        z: 0.0
        angular:
        x: 0.0
        y: 0.0
        z: 0.5
        "
    ```