#include <cstring>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc,char * argv[]){
    int lfd=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in myaddr;
    myaddr.sin_family=AF_INET;
    myaddr.sin_port=htons(8000);
    myaddr.sin_addr.s_addr=inet_addr("127.0.0.1");//IPV4.inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);
    int ret=bind(lfd,(struct sockaddr *)&myaddr,sizeof(myaddr));
    if(ret<0){
        perror("");
        return 0;
    }
    char buf[1500]="";
    struct sockaddr_in cliaddr;
    socklen_t len=sizeof(cliaddr);
    struct sockaddr_in dstaddr;
    dstaddr.sin_family=AF_INET;
    dstaddr.sin_port=htons(8080);
    dstaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    int n=0;
    while(1){
        n=read(STDIN_FILENO,buf,sizeof(buf));
        sendto(lfd,buf,n,0,(struct sockaddr *)&dstaddr,sizeof(dstaddr));
        memset(buf,0,sizeof(buf));
        n=recvfrom(lfd,buf,sizeof(buf),0,NULL,NULL);
        if(n<0){
            perror("");
            break;
        }
        else{
            printf("%s\n",buf);
        }
    }
    close(lfd);
    return 0;
}