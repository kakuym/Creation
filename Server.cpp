#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc,char *argv[]){
    int lfd=socket(AF_INET,SOCK_STREAM,0);
}