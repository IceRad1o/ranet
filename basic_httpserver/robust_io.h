//
// Created by Jialei Wang on 2019/4/4.
//

#ifndef BASIC_HTTPSERVER_ROBUST_IO_H
#define BASIC_HTTPSERVER_ROBUST_IO_H

#define RIO_BUFSIZE 8192

#include <sys/types.h>
#include <string.h>
typedef struct{
    int rio_fd;
    int rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFSIZE];
}rio_t;

/* <cmu/csapp>:robust io.The function is inspired from UNP:ch3 readn/writen/readline
 * rio_readn and rio_writen are rio's io functions without buffer,
 * it's easy to transfer data between memory and file
 */
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *userbuf, size_t n);

//rio buffered input functions
//readnb can be used to for both text file and binary file while readlineb is just for text file
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);


//wrapper functions for robust io

#endif //BASIC_HTTPSERVER_ROBUST_IO_H
