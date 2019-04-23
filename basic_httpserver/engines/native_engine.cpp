//
// Created by Jialei Wang on 2019/4/23.
//
#include "engines.h"

void native_engine(int listenfd)
{
    struct sockaddr_in clientaddr;
    socklen_t clientlen;
    int connfd;
    for(;;)
    {
        clientlen = sizeof(clientaddr);
        connfd = accept_conn(listenfd, (struct sockaddr*)&clientaddr, &clientlen);
        doit(connfd);
        close(connfd);
    }
}
