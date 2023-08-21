#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stddef.h>
#include <unistd.h>
int main(int argc,char *argv[]){
    unlink("sock.c");
    int cfd=socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un myaddr;
    myaddr.sun_family=AF_UNIX;
    strcpy(myaddr.sun_path,"sock.c");
    int len=offsetof(struct sockaddr_un,sun_path)+strlen("sock.c");
    if(bind(cfd,(struct sockaddr*)&myaddr,len)<0){
        perror("");
        return 0;
    }
    struct sockaddr_un seraddr;
    seraddr.sun_family=AF_UNIX;
    strcpy(seraddr.sun_path,"sock.s");
    connect(cfd,(struct sockaddr *)&seraddr,sizeof(seraddr));
    while(1){
        char buf[1500]="";
        int n=read(STDERR_FILENO,buf,sizeof(buf));
        send(cfd,buf,n,0);
        memset(buf,0,sizeof(buf));
        n=recv(cfd,buf,sizeof(buf),0);
        if(n<0){
            printf("err or server close\n");
            break;
        }
        else{
            printf("%s\n",buf);
        }
    }
    
    close(cfd);
    return 0;
}