#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024] = {0};

    // 创建一个套接字对象
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 绑定IP地址和端口号
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 开始监听
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    cout << "Server is listening on 127.0.0.1:8080..." << endl;

    // 等待客户端连接
    socklen_t client_addr_len = sizeof(client_addr);
    if ((client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    cout << "Connected by " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;
    while (true) {
        // 接收客户端发送的数据
        int n = read(client_fd, buffer, sizeof(buffer));
        if (n <= 0) {
            break;
        }
        cout << "Received from client: " << buffer << endl;

        // 发送数据给客户端
        const char *message = "Hello, client";
        write(client_fd, message, strlen(message));
    }

    close(client_fd);
    close(server_fd);
    cout << "Server is closed" << endl;

    return 0;
}