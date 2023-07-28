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
    myaddr.sin_port=htons(8080);
    myaddr.sin_addr.s_addr=inet_addr("127.0.0.1");//IPV4.inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);
    int ret=bind(lfd,(struct sockaddr *)&myaddr,sizeof(myaddr));
    if(ret<0){
        perror("");
        return 0;
    }
    char buf[1500]="";
    struct sockaddr_in cliaddr;
    socklen_t len=sizeof(cliaddr);
    while(1){
        memset(buf,0,sizeof(buf));
        int n=recvfrom(lfd,buf,sizeof(buf),0,(struct sockaddr *)&cliaddr,&len);
        if(n<0){
            perror("");
            break;
        }
        else{
            printf("%s\n",buf);
            sendto(lfd,buf,n,0,(struct sockaddr *)&cliaddr,len);
        }
    }
    close(lfd);
    return 0;
}