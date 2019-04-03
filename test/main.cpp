#include <sys/socket.h> //socket
#include <string.h> // memset
#include <netinet/in.h> // sockaddr_in
#include <stdio.h> //printf
#include <time.h>
#include <unistd.h> // write
#include <iostream>


int create_and_bind(int port)
{
    int listenfd;
    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        return -1;
    listen(listenfd, 1024);
    return listenfd;
}


int main() {
    time_t ticks;
    char buff[4096];
    bool stop = false;
    int listenfd = create_and_bind(13);
    int connfd;
    printf("test server is running\n");
    while(1) {
        connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        write(connfd, buff, strlen(buff));
        close(connfd);
    }
    return 0;
}

