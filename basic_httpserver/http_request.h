//
// Created by Jialei Wang on 2019/4/4.
//
#pragma once
#ifndef BASIC_HTTPSERVER_HTTP_REQUEST_H
#define BASIC_HTTPSERVER_HTTP_REQUEST_H

#include <string>
#include <map>
#include <sys/stat.h>
#include <fcntl.h>  // open
#include <unistd.h> // close
#include <sys/mman.h> // mmap

#include "robust_io.h"
#include "util.h"
enum HTTPMETHOD {
    HTTP_METHOD_UNKNOWW,
    HTTP_MOETHOD_GET,
    HTTP_METHOD_POST
};

struct HTTPRequest {
    //constants
    static constexpr size_t BUF_SIZE = 1024;

    //buffer
    char buf[BUF_SIZE];
    size_t buf_head;
    size_t buf_tail;

    //
    HTTPMETHOD method;
    std::string uri;
    std::string request_path;
    std::string query_string;
    std::string fragment;
    unsigned short http_version_major;
    unsigned short http_version_minor;
    std::map<std::string, std::string> headers;
    std::string body;
    bool keep_alive;

    // engine data
    int fd_socket;
    union  EngineData{
        int fd_epoll;
    };
    EngineData ngdata;

    // do_request() internal variables

};





/*
 * csapp interface
 *
 */
void doit(int fd);

// read request headers
void read_requesthdrs(rio_t *rp);

int parse_uri(char *uri, char *filename, char *cgiargs);

void serve_static(int fd, char *filename, int filesize);

void get_filetype(char *filename, char *filetype);

void serve_dynamic(int fd, char *filename, char *cgiargs);

void clienterror(int fd, char *cause, const char *errnum, const char *shortmsg, const char *longmsg);

#endif //BASIC_HTTPSERVER_HTTP_REQUEST_H
