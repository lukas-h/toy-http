# toy-http
lightweight embeddable http server.  
Supported requests: GET, HEAD  
You can easily embed the http server as a thread to your applications
It is only designed for unix (linux, macOSX, freebsd and other OS with posix programming environment).
# install the standalone server
You can use it as lightweight server. Only clone this repository and
type in: `./install`.
# embed it!
You can embed it into your applications, simply add `service.h` and `service.c` to your project
and use this function: ` int serve(int http_port, int max_connections, char *serve_directory);`.  
`max_connections` is the maximum number of clients that can be connected at once.  
`serve_directory` is the folder with the ressources and files that are hosted.
