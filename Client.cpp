#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc,char *argv[]){
    int sock_fd;
    sock_fd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(8080);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);
    connect(sock_fd,(struct sockaddr *)&addr,sizeof(addr));

    char buf[1024]="";
    while(1){
        int n=read(STDIN_FILENO,buf,sizeof(buf));
        write(sock_fd,buf,n);
    }

    close(sock_fd);
    return 0;
    
}