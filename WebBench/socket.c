/* $Id: socket.c 1.1 1995/01/01 07:11:14 cthuang Exp $
 *
 * This module has been modified by Radim Kolar for OS/2 emx
 */

/***********************************************************************
  module:       socket.c
  program:      popclient
  SCCS ID:      @(#)socket.c    1.5  4/1/94
  programmer:   Virginia Tech Computing Center
  compiler:     DEC RISC C compiler (Ultrix 4.1)
  environment:  DEC Ultrix 4.3 
  description:  UNIX sockets code.
 ***********************************************************************/
 
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// 创建一个套接字，主动连接 host
int Socket(const char *host, int clientPort)
{
    int sock;
    unsigned long inaddr;
    // 创建 ipv4 结构体
    struct sockaddr_in ad;
    // 《UNP》ch11.3
    struct hostent *hp;
    
    // 初始化地址结构体
    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    // 将字符串表示的host转为32位整型表示
    // 《UNP》ch3.6
    inaddr = inet_addr(host);
    if (inaddr != INADDR_NONE)
        // 将转换后的数值放入结构体
        memcpy(&ad.sin_addr, &inaddr, sizeof(inaddr));
    else
    {
        // 获取host的信息 《UNP》ch11.3
        hp = gethostbyname(host);
        if (hp == NULL)
            return -1;
        memcpy(&ad.sin_addr, hp->h_addr, hp->h_length);
    }
    ad.sin_port = htons(clientPort);
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return sock;
    if (connect(sock, (struct sockaddr *)&ad, sizeof(ad)) < 0)
        return -1;
    return sock;
}

