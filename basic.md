Select

```c++
#include<unistd.h>
#include<sys/types.h>
int select(int n, fd_set *fdset, NULL, NULL, NULL)
    // select 返回已经准备好的fd个数，出错则返回-1
FD_ZERO(fd_set *fdset)     // clear all bits in fdset
FD_CLR(int fd, fd_set *fdset)		// clear bit fd in fdset
FD_SET(int fd, fd_set *fdset)		// turn on bit fd in fdset
FD_ISSET(int fd, fd_set *fdset)	// is bit fd in fdset turned
```





Epoll 使用

epoll接口

http://man7.org/linux/man-pages/man7/epoll.7.html

```c++
#include<sys/epoll.h>
int epoll_create(int size); int epoll_create1(int flags);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event* events,int maxevents, int timeout);
```



**Epoll_ctl :**

op代表操作，可以是

EPOLL_CTL_ADD：在epfd上注册文件描述符fd，并将事件event关联到fd

EPOLL_CTL_MOD：更改fd上关联的事件是event

EPOLL_CTL_DEL：在epoll实例上删除描述符fd

struct epoll_event的数据结构

```c++
typedef union epoll_data 
{
   void        *ptr;
   int          fd;
   uint32_t     u32;
   uint64_t     u64;
} epoll_data_t;
 
struct epoll_event 
{
   uint32_t     events;      /* Epoll events */
   epoll_data_t data;        /* User data variable */
}
```

而这里的 uint32_t events 是由(直接参考epoll.h来查看值，都是2的多次幂)

EPOLLIN

EPOLLOUT

EPOLLRDHUP

EPOLLPRI

EPOLLERR

EPOLLHUP

EPOLLET

EPOLLONESHOT



**Epoll_wait/Epoll_pwait：**

Epoll_wait 用于等待在epoll instance(epfd)上的事件，参数`events`指针指向的内存空间会包含对于caller可用的events。当然最多只能包含`maxevents`个(所以events一般就传入一个struct epoll_event events[maxevents]数组)。Timeout是epoll_wait会block的毫秒，设为-1那么接口会一直block，而设为0接口就回立即返回，即使没有events是可用的(available)

**Epoll 的工作方式 LT（level-triggered） ET（edge-triggered）**



```c++
// 简单的服务器流程
for(;;)
{
    nfds = epoll_wait(epfd,events,20,500);
    for(i=0;i<nfds;i++){
        if(events[i].data.fd == listenfd) //新的连接
        {
            connfd = accept(listenfd,(sockaddr*)&clientaddr,&clienlen);
        	ev.data.fd = connfd;
        	ev.events = EPOLLIN|EPOLLET;
        	epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev); //添加新的fd到epoll中
        }
        else if( events[i].events&EPOLLIN ) //接收到数据，读socket
        {
                n = read(sockfd, line, MAXLINE)) < 0    //读
                ev.data.ptr = md;     //md为自定义类型，添加数据
                ev.events=EPOLLOUT|EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);//修改标识符，等待下一个循环时发送数据
        }
        else if(events[i].events&EPOLLOUT) //有数据待发送，写socket
        {
                struct myepoll_data* md = (myepoll_data*)events[i].data.ptr;    //取数据
                sockfd = md->fd;
                send( sockfd, md->ptr, strlen((char*)md->ptr), 0 );        //发送数据
                ev.data.fd=sockfd;
                ev.events=EPOLLIN|EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev); //修改标识符，等待下一个循环时接收数据
        }
        else
        {
                //其他的处理
        }
    }
}

/*
     * we can know that events is 1 2 4 8....so just use & | and determine whether it's
     * the same .like even.events & (EPOLLIN | EPOLLERR) if events=IN or ERR then return true else return false
    struct epoll_event event;
    event.events = EPOLLIN;
    auto a = EPOLLPRI & EPOLLHUP;
    printf("Result of %d&%d is: %d",EPOLLPRI,EPOLLWRBAND,a);
    */
```

