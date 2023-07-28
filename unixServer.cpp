#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>


int main(int argc,char *argv[]){
    unlink("sock.s");
    int lfd=socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un myaddr;
    myaddr.sun_family=AF_UNIX;
    strcpy(myaddr.sun_path,"sock.s");
    int len=offsetof(struct sockaddr_un,sun_path)+strlen(myaddr.sun_path);
    bind(lfd,(struct sockaddr *)&myaddr,sizeof(myaddr));
    listen(lfd,128);
    struct sockaddr_un cliaddr; 
    socklen_t len1=sizeof(cliaddr);
    int cfd=accept(lfd,(struct sockaddr *)&cliaddr,&len1);
    printf("new client file=%s\n",cliaddr.sun_path);
    char buf[1500]="";
    while(1){
        memset(buf,0,sizeof(buf));
        int n=recv(cfd,buf,sizeof(buf),0);
        if(n<0){
            printf("err or client close\n");
            break;
        }
        else{
            printf("%s\n",buf);
            send(cfd,buf,n,0);
        }
    }
    close(cfd);
    close(lfd);
    return 0;
}