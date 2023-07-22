#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};

    // 创建一个套接字对象
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 连接到服务器
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
    if (connect(client_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    while (true) {
        // 发送数据给服务器
        cout << "Enter message: ";
        cin.getline(buffer, sizeof(buffer));
        write(client_fd, buffer, strlen(buffer));

        // 接收服务器发送的数据
        int n = read(client_fd, buffer, sizeof(buffer));
        if (n <= 0) {
            break;
        }
        cout << "Received from server: " << buffer << endl;
    }

    close(client_fd);

    return 0;
}