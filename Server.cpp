#include <stdio.h>
#include <arpa/inet.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

int main(int argc,char *argv[]){
    int lfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(8080);
    addr.sin_addr.s_addr=INADDR_ANY;//绑定的通配地址
    bind(lfd,(struct sockaddr *)&addr,sizeof(addr));
    listen(lfd,128);
    struct sockaddr_in cliaddr;
    socklen_t len=sizeof(cliaddr);
    int cfd=accept(lfd,(struct sockaddr *)&cliaddr,&len);
    char ip[16]="";
    printf("新客户端ip=%s port=%d\n",inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,ip,16),ntohs(cliaddr.sin_port));
    char buf[1024]="";
    while(1){
        bzero(buf,sizeof(buf));
        // int n=read(STDIN_FILENO,buf,sizeof(buf));
        // write(cfd,buf,n);
        int n=0;
        n=read(cfd,buf,sizeof(buf));
        if(n==0){
            printf("客户端关闭！\n");
            break;
        }
        printf("%s\n",buf);
        // 去掉末尾的换行符（如果存在）
        buf[strcspn(buf, "\n")] = '\0';
        // 在读取的数据末尾添加 "OK"
        strcat(buf, " OK");
        int size = sizeof(buf);
        write(cfd,buf,size);
    }
    close(cfd);
    close(lfd);
    return 0;
}