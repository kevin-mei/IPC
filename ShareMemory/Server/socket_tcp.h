#ifndef SOCKET_TCP_H
#define SOCKET_TCP_H

void run()
{
//... ...
int listen_fd, connect_fd;
struct sockaddr_in serv_addr, client_addr;
//... ...
listen_fd = socket ( PF_INET, SOCK_STREAM, 0 );

/* 创建网际Ipv4域的（由PF_INET指定）面向连接的（由SOCK_STREAM指定，
如果创建非面向连接的套接口则指定为SOCK_DGRAM）
的套接口。第三个参数0表示由内核确定缺省的传输协议，
对于本例，由于创建的是可靠的面向连接的基于流的套接口，
内核将选择TCP作为本套接口的传输协议） */

bzero( &serv_addr, sizeof(serv_addr) );
serv_addr.sin_family = AF_INET ;  /* 指明通信协议族 */
serv_addr.sin_port = htons( 49152 ) ;       /* 分配端口号 */
inet_pton(AF_INET, " 192.168.0.11", &serv_addr.sin_sddr) ;
/* 分配地址，把点分十进制IPv4地址转化为32位二进制Ipv4地址。 */
bind( listen_fd, (struct sockaddr*) serv_addr, sizeof ( struct sockaddr_in )) ;
/* 实现绑定操作 */
listen( listen_fd, max_num) ;
/* 套接口进入侦听状态，max_num规定了内核为此套接口排队的最大连接个数 */
for( ; ; ) {
//... ...
connect_fd = accept( listen_fd, (struct sockaddr*)client_addr, &len ) ; /* 获得连接fd. */
//... ...                 /* 发送和接收数据 */
}
#endif // SOCKET_TCP_H
