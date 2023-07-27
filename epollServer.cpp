#include <stdio.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "wrap.h"

int main(int argc,char *argv[]){
    int lfd=tcp4bind(8080, NULL);
    Listen(lfd, 128);
    int epfd=epoll_create(1);
    struct epoll_event ev,evs[1024];
    ev.data.fd=lfd;
    ev.events=EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);
    while(1){
        int nready=epoll_wait(epfd,evs,1024,-1);
        printf("epoll wait____\n");
        if(nready<0){
            if (errno == EINTR) {
                // 系统调用被中断，重新尝试
                // continue;
            } else {
                // 其他错误，处理错误
                perror("");
                break;
            }
        }
        else if(nready==0){
            continue;
        }
        else{//有文件描述符变化
            for(int i=0;i<nready;i++){
                //判断lfd变化，并且市读事件变化
                if(evs[i].data.fd==lfd && evs[i].events&EPOLLIN){
                    struct sockaddr_in cliaddr;
                    char ip[16]="";
                    socklen_t len=sizeof(cliaddr);
                    int cfd=Accept(lfd,(struct sockaddr *)&cliaddr,&len);
                    printf("new client ip=%s port=%d\n",inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,ip,16),ntohs(cliaddr.sin_port));
                    //设置cfd为非阻塞
                    int flags=fcntl(cfd,F_GETFL);
                    flags |=O_NONBLOCK;
                    fcntl(cfd,F_SETFL,flags);
                    
                    ev.data.fd=cfd;
                    ev.events=EPOLLIN | EPOLLET;//读事件，边缘触发
                    // ev.events=EPOLLIN;//读事件，默认水平触发
                    epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);
                }
                else if(evs[i].events & EPOLLIN){//cfd变化，而且是读事件
                    while(1){
                        char buf[4]="";

                        int n=read(evs[i].data.fd,buf,sizeof(buf));
                        if(n<0){
                            //缓冲区读干净，跳出循环
                            if(errno==EAGAIN){
                                break;
                            }
                            perror("");
                            epoll_ctl(epfd,EPOLL_CTL_DEL,evs[i].data.fd,&evs[i]);//下树
                            break;
                        }
                        else if(n==0){
                            printf("client close\n");
                            close(evs[i].data.fd);
                            epoll_ctl(epfd,EPOLL_CTL_DEL,evs[i].data.fd,&evs[i]);
                            break;
                        }
                        else{
                            // printf("%s\n",buf);
                            write(STDOUT_FILENO,buf,sizeof(buf));
                            write(evs[i].data.fd,buf,n);
                        }
                    }
                }
            }
        }
    }
}