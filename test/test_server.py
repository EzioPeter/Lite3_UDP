import socket
import struct

def main():
    # 服务器配置
    SERVER_IP = "127.0.0.1"
    SERVER_PORT = 43893
    
    # 创建UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((SERVER_IP, SERVER_PORT))
    print(f"UDP服务器已启动，监听 {SERVER_IP}:{SERVER_PORT}...")
    print("等待接收数据... (按Ctrl+C退出)")
    
    try:
        while True:
            # 接收数据
            data, client_addr = sock.recvfrom(1024)  # 缓冲区大小1024字节
            print(f"\n收到来自 {client_addr} 的数据:")
            print(f"数据长度: {len(data)} 字节")
            print(f"原始十六进制: {data.hex()}")
            
            # 尝试解析数据（3个uint32_t字段）
            if len(data) == 12:  # 3个32位整数共12字节
                # 分别按大端和小端解析（因为两个客户端使用不同字节序）
                big_endian = struct.unpack(">III", data)  # 大端解析
                little_endian = struct.unpack("<III", data)  # 小端解析
                
                print("\n按大端字节序解析:")
                print(f"code: 0x{big_endian[0]:08X}")
                print(f"param_size: {big_endian[1]}")
                print(f"type: {big_endian[2]}")
                
                print("\n按小端字节序解析:")
                print(f"code: 0x{little_endian[0]:08X}")
                print(f"param_size: {little_endian[1]}")
                print(f"type: {little_endian[2]}")
            else:
                print("数据长度不符合预期(应为12字节)")
                
    except KeyboardInterrupt:
        print("\n服务器正在关闭...")
    finally:
        sock.close()
        print("服务器已关闭")

if __name__ == "__main__":
    main()