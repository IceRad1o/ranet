#include <sys/socket.h> //socket
#include <string.h> // memset
#include <netinet/in.h> // sockaddr_in
#include <stdio.h> //printf
#include <time.h>
#include <unistd.h> // write
#include <iostream>
#include "http_request.h"
#include "socklib.h"

int main(int argc, char **argv) {
    int listenfd, connfd;
    int port;
    struct sockaddr_storage clientaddr;
    socklen_t clientlen;

    if(argc!=2){
        fprintf(stderr, "usage: %s<port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);

    listenfd = create_and_bind(port, true);
    while(1){
        clientlen = sizeof(clientaddr);
        connfd = accept_conn(listenfd, (struct sockaddr*)&clientaddr, &clientlen);
        doit(connfd);
        close(connfd);
    }
}
