#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "wrap.h"
#include <signal.h>

void free_process(int sig){
    pid_t pid;
    while(1){
        pid=waitpid(-1,NULL,WNOHANG);
        if(pid<=0){
            break;
        }
        else{
            printf("child pid=%d\n",pid);
        }
    }
}
int main(int argc,char *argv[]){
    //阻塞
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);
    sigprocmask(SIG_BLOCK,&set,NULL);

    int lfd=tcp4bind(8080, NULL);
    Listen(lfd,128);
    struct sockaddr_in cliaddr;
    socklen_t len=sizeof(cliaddr);
    while(1){
        char ip[16]="";
        int cfd=Accept(lfd,(struct sockaddr *)&cliaddr,&len);
        printf("new client ip=%s port=%d\n",inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,ip,16),ntohs(cliaddr.sin_port));
        pid_t pid;
        pid=fork();
        if(pid<0){
            perror("");
            exit(0);
        }
        else if(pid==0){
            close(lfd);
            while(1){
                char buf[1024]="";
                int n=read(cfd,buf,1024);
                if(n<0){
                    perror("");
                    close(cfd);
                    exit(0);
                }
                else if(n==0){
                    printf("client close\n");
                    close(cfd);
                    exit(0);
                }
                else{
                    printf("%s\n",buf);
                    write(cfd,buf,n);
                }
            }
            
        }
        else{
            close(cfd);
            //回收
            //注册信号回调
            struct sigaction act;
            act.sa_flags=0;
            act.sa_handler=free_process;
            sigemptyset(&act.sa_mask);
            sigaction(SIGCHLD,&act,NULL);
            sigprocmask(SIG_UNBLOCK,&set,NULL);
        }
    }
    return 0;
}