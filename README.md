ranet

c++11 network library
=======
some rep for learning:
libevent
libuv: https://github.com/libuv/libuv. It's a multi-platform support library on async I/O
libev: 
boost::asio
windows/iocp



`Tutorial` contains some example of some api used in this project

`Basic_httpserver` is a simple http server built for nerwork programming learning.

------

So when we start to learn network programming, usually we'll write a echo tcp server and client to get familiar with socket apis.

When it comes to C10K problem, we found we should expand our io module, because a simple tcp server is not effective enough.So we'll try different methods like fork, pthread(+threadpool), io multiplexing(select, poll, epoll) or reactor design pattern.

To test serval io modules, we decide to implement a http server.

web 服务器 可以用两种方式给客户端提供内容

- 取一个静态文件返回内容给客户端(static content)
- 运行一个可执行文件，把它的输出返回给客户端(dynamic conent)

#### 1.HTTP Request

It consists of 3 parts,every part is cut by CRLF(\r\n)

1. A request line 

   HTTP method(GET or POST or others) + Request URI(resource location) + HTTP Version +CRLF

2. Zero or more headers

​       \<Headers name> : \<value> CRLF

3. An empty line CRLF

4. Optionally a message-body

```http
GET /hello.htm HTTP/1.1
User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
Host: www.tutorialspoint.com
Accept-Language: en-us
Accept-Encoding: gzip, deflate
Connection: Keep-Alive
```

```http
POST /cgi-bin/process.cgi HTTP/1.1
User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
Host: www.tutorialspoint.com
Content-Type: application/x-www-form-urlencoded
Content-Length: length
Accept-Language: en-us
Accept-Encoding: gzip, deflate
Connection: Keep-Alive

licenseID=string&content=string&/paramsXML=string
```

这里解释一下，服务器没法确认URL指向的是静态还是动态内容，所以需要自己管理，比如确认一组目录，例如cgi-bin，所有的可执行文件都放在这里.

> Get的请求参数在url中传递，POST在request body中传递



#### 2.HTTP Response

It's similar to request

1. Status line = HTTP Version + Status Code +Status Message 
2. Headers
3. Body

如果在Header中指定了Transfer-Encoding: chunked则主体内容是分块编码

否则在Header中应该制定Content-Length即主体内容的长度

```http
HTTP/1.1 200 OK
Date: Fri, 31 Dec 1999 23:59:59 GMT
Content-Type: text/plain
Content-Length: 42
some-footer: some-value
another-footer: another-value

abcdefghijklmnopqrstuvwxyz1234567890abcdef
```

####  3.HTTP Keep-Alive

因为在打开一个网页时，往往需要向同一个服务器进行请求多次资源（如额外的样式、脚本、图片文件等等），因此 HTTP 协议中允许一个 TCP 连接在完成一次交互之后不立即关闭，而是继续用作下一次交互。

在 `HTTP/1.0` 中，要使用 HTTP Keep-Alive，需要在 HTTP Request Header 中加入 `Connection: Keep-Alive`。

在 `HTTP/1.1` 中，默认使用 HTTP Keep-Alive。如果服务器需要提前关闭连接，需要在 HTTP Response Header 中加入 `Connection: Close`。

HTTP Keep-Alive 无疑可以加快载入网页的速度，提升用户体验，但也增大了 HTTP Server 的负担——每个 TCP 连接的生命周期变长了。

### 4.HTTP server module

1. 最简单的tcp服务器

每次只能接受处理一个连接，另外的请求就需要等待。

```c++
for(;;){
	int connfd = accept(listenfd, NULL, NULL);
    doit(connfd);
    close(connfd)
}
```

2. 多进程

```c++

for(;;){
    int connfd = accept(listenfd, NULL, NULL);
    pid_t pid = fork();
    if(pid == 0){
        //child 
        doit(connfd);
    }
    else{
        // parent: close dup fd
        close(connfd);
    }
}
```

3.  多线程

线程数上升后线程调度的问题会影响性能

```c++
void *work(void *arg){
    int connfd = (int)arg;
    doit(connfd);
}
for(;;){
    int connfd = accept(listenfd, NULL, NULL);
    pthread_t thread;
    pthread_create(&thread, &attr, work, (void*)connfd);
}
```

4. 多线程+线程池

减去创建和销毁线程的开销。

```c++

```



5. 非阻塞I/O
6. I/O 复用 select
7. I/O 复用 poll
8. I/O 复用 epoll

分为LT（水平触发）和ET（边缘触发）

9. I/O 复用 + 多线程



#### 5.一些trick





