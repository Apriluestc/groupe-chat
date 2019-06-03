int main(int argc, char *argv[])
{
    /* 定义一个listen指针。该结构体是自己定义的 */
    struct listen *_listen;
    /* 初始化socket，这个初始化过程跟普通的UDP初始化 socket套接字一样 */
    sockfd = init_socket(); 

    /*
     * 开始监听这个socket. 最大的连接数为10，也就是说最多只有10个客户端
     * 封装好的一个函数，功能有点类似于 TCP协议中的 listen 函数
     * */
    server_listen(&sockfd, 10);

    while(1) {
        /* 
         * 获得一个连接。类似于TCP的 accept 函数 
         * 需要注意的是，如果没有连接， server_accept 函数将进入休眠状态，直到有一个新的客户端数据
         * 客户端只有在第一次发生数据过来的时候，才会创建一个新的 listen ，并唤醒 server_accept 函数
         * 之后，这个客户端的所有数据都将发送到 这个新的 listen 的数据队列中。
         * 所以。通过这个 listen ，我们可以创建一个进程，由该进程去处理这个客户端之后的请求
         * 这里，listen 有点像 TCP 协议中的 accept 函数新建的 sockfd
         * */
        _listen = server_accept();

        /* 
         * 虽然说 server_accept 会进入休眠，但是仍然会被其它信号唤醒，所以要做个判断
         * 判断下是否为 NULL 。为 NULL 则说明没有新的连接 
         * */
        if (_listen == NULL){
            continue;
        }

        printf("new client \r\n");
        /* 
         * 启动一个 listen_phread 线程，并且，由该线程去处理这个连接
         * 类似于TCP 的fork
         * */
        listen_pthread(_listen, listen_phread);
    }
}
