#include <iostream>
#include <unistd.h>

int main() {
    pid_t pid = fork(); // 创建子进程
    if (pid < 0) {
        std::cerr << "Failed to fork!" << std::endl;
        return 1;
    } else if (pid == 0) {
        // 子进程代码
        std::cout << "Hello from child process! PID: " << getpid() << std::endl;
    } else {
        // 父进程代码
        std::cout << "Hello from parent process! Child PID: " << pid << std::endl;
    }
    return 0;
}
