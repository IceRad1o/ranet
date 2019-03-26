//
// Created by jialeiwang on 3/26/19.
//

#define MAXEVENTS 5
#define READ_MAX 10
#include<stdio.h>
#include<unistd.h>
#include <sys/epoll.h>
#include <string.h>

int main(int argc,char *argv[]){
    int running =1,event_count,i;
    size_t bytes_read;
    char read_buffer[READ_MAX+1];
    struct epoll_event event, events[MAXEVENTS];
    int epoll_fd = epoll_create1(0);
    if(epoll_fd == -1){
        fprintf(stderr,"Fail to create epoll file descriptor\n");
        return 1;
    }
    // should know level-triggered and edge-triggered
    event.events = EPOLLIN;
    event.data.fd = 0; // standard input

    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event)){
        fprintf(stderr,"Fail to add descriptor to epoll\n");
        close(epoll_fd);
        return 1;
    }

    while(running){
        printf("\nPolling for input\n");
        event_count = epoll_wait(epoll_fd,events, MAXEVENTS, 30000);
        for(int i=0;i<event_count;++i) {
            bytes_read = read(events[i].data.fd,read_buffer,READ_MAX);
            printf("%zd bytes read\n",bytes_read);
            read_buffer[bytes_read] = '\0';
            printf("Read '%s'\n", read_buffer);

            if(!strncmp(read_buffer, "stop\n", 5))
                running = 0;
        }
    }


    if(close(epoll_fd)){
        fprintf(stderr,"Fail to close epoll file descriptor\n");
        return 1;
    }
    return 0;
}

