#include <netinet/in.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include "wrap.h"
#include <sys/time.h>

#define PORT 8080

int main(int argc,char *argv[]){
    int lfd=tcp4bind(PORT, NULL);
    Listen(lfd, 128);
    int maxfd=lfd;//最大文件描述符
    fd_set oldset,rset;
    FD_ZERO(&oldset);
    FD_ZERO(&rset);
    FD_SET(lfd,&oldset);
    while(1){
        rset=oldset;
        int n=select(maxfd+1,&rset,NULL,NULL,NULL);
        if(n<0){
            perror("");
            break;
        }
        else if(n==0){
            continue;
        }
        else{
            if(FD_ISSET(lfd,&rset)){
                struct sockaddr_in cliaddr;
                socklen_t len =sizeof(cliaddr);
                char ip[16]="";
                int cfd=Accept(lfd,(struct sockaddr *)&cliaddr,&len);
                printf("new cliaddr ip=%s port=%d\n",inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,ip,16),cliaddr.sin_port);
                FD_SET(cfd,&oldset);
                if(cfd>maxfd){
                    maxfd=cfd;
                }
                if(--n==0){
                    continue;
                }
            }
            for(int i=lfd+1;i<=maxfd;i++){
                if(FD_ISSET(i,&rset)){
                    char buf[1500]="";
                    int ret=Read(i,buf,sizeof(buf));
                    if(ret<0){
                        perror("");
                        close(i);
                        FD_CLR(i,&oldset);
                        continue;
                    }
                    else if(ret==0){
                        printf("client close\n");
                        close(i);
                        FD_CLR(i,&oldset);
                    }
                    else{
                        printf("%s\n",buf);
                        Write(i,buf,ret);
                    }
                }
            }
        }
    }
}