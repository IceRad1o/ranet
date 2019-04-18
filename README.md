ranet

c++11 network library



`Tutorial` contains some example of some api used in this project

`Basic_httpserver` is a simple http server built for learning.

------

So when we start to learn network programming, usually we'll write a echo tcp server and client to get familiar with socket apis.



When it comes to C10K problem, we found we should expand our io module, because a simple tcp server is not effective enough.So we'll try different methods like fork, pthread(+threadpool), io multiplexing(select, poll, epoll) or reactor design pattern.

To test serval io modules, we decide to implement a http server.

web 服务器 可以用两种方式给客户端提供内容

- 取一个静态文件返回内容给客户端(static content)
- 运行一个可执行文件，把它的输出返回给客户端(dynamic conent)

#### HTTP Request

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



#### HTTP Response

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

####  HTTP Keep-Alive



### HTTP server module



