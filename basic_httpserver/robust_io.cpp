//
// Created by Jialei Wang on 2019/4/4.
//
#include <unistd.h>
#include <errno.h>
#include "robust_io.h"

ssize_t rio_readn(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    // char *bufp = usrbuf it's ok with c but not with c++
    char *bufp = (char*)usrbuf;
    while(nleft>0)
    {
        if((nread = read(fd, bufp, n)) < 0 )
        {
            if(errno == EINTR)   // Interrupted by sig handler return
                nread = 0;      // call read() again
            else
                return -1;      // errno set by read()
        }
        else if(nread == 0)
            break;              // EOF
        nleft -= nread;
        bufp += nread;
    }
    return n-nread;
}


ssize_t rio_writen(int fd, void*usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = (char*)usrbuf;
    while(nleft > 0)
    {
        if((nwritten = write(fd, bufp, n)) <= 0)
        {
            if(errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}

void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_read(rio_t *rp, void*userbuf, size_t n)
{
    int cnt;
    while(rp->rio_cnt <= 0)     //refill if buffer is empty
    {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if(rp->rio_cnt < 0){
            if(errno != EINTR) // interrupted by signal handler return
                return -1;
        }
        else if(rp->rio_cnt == 0) // EOF
            return 0;
        else
            rp->rio_bufptr = rp->rio_buf;
    }
    cnt = n;
    if(rp->rio_cnt < n)
        cnt = rp->rio_cnt;
    memcpy(userbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}


ssize_t rio_readlineb(rio_t *rp, void *usrbuf ,size_t maxlen)
{
    int n, rc;
    char c, *bufp = (char*)usrbuf;
    for(n = 1; n < maxlen; n++){
        if((rc = rio_read(rp, &c, 1)) == 1){
            *bufp++ = c;
            if(c == '\n')
                break;
        }
        else if(rc == 0){
            if(n==1)
                return 0; //EOF no data read
            else
                break;    //EOF some data was read
        }
        else
            return -1;
    }
    *bufp = 0;
    return n;
}
/*
ssize_t readnb(rio_t *rp, void *usrbuf, size_t n)
{

}
*/