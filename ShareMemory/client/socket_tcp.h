#ifndef SOCKET_TCP_H
#define SOCKET_TCP_H


void run()
{
//... ...
int socket_fd;
struct sockaddr_in serv_addr ;
//... ...
socket_fd = socket ( PF_INET, SOCK_STREAM, 0 );
bzero( &serv_addr, sizeof(serv_addr) );
serv_addr.sin_family = AF_INET ;  /* 指明通信协议族 */
serv_addr.sin_port = htons( 49152 ) ;       /* 分配端口号 */
inet_pton(AF_INET, " 192.168.0.11", &serv_addr.sin_sddr) ;
/* 分配地址，把点分十进制IPv4地址转化为32位二进制Ipv4地址。 */
connect( socket_fd, (struct sockaddr*)serv_addr, sizeof( serv_addr ) ) ; /* 向服务器发起连接请求 */
//... ...                         /* 发送和接收数据 */
//... ...
}

#endif // SOCKET_TCP_H
