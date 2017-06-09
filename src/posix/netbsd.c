/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-04-03  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>>>> CrHack 网络通讯函数库 for BSD <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"
#include "memlib.h"
#include "morder.h"
#include "strlib.h"

#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <unistd.h>

/* 方便代码的书写 */
typedef sint_t              SOCKET;
typedef struct ifreq        sIFREQ;
typedef struct timeval      TIMEVAL;
typedef struct hostent      HOSTENT;
typedef struct ip_mreq      sIP_MREQ;
typedef struct sockaddr     SOCKADDR;
typedef struct sockaddr_in  SOCKADDR_IN;

/* 不是所有系统都有这个的 */
#ifndef MSG_NOSIGNAL
    #define MSG_NOSIGNAL    0
#endif

/* 套接字内部结构 */
typedef struct
{
        /* 套接字句柄 */
        SOCKET      socket;

        /* 本地网络连接信息 */
        bool_t      local_have;
        SOCKADDR_IN local_addr;

        /* 远端网络连接信息 */
        bool_t      remote_have;
        SOCKADDR_IN remote_addr;

        /* 组播网络连接信息 */
        bool_t      multic_have;
        sIP_MREQ    multic_parm;

        /* 两个超时参数 (发/收) */
        TIMEVAL wr, *timeout_wr;
        TIMEVAL rd, *timeout_rd;

} sSOCKET;

/* 关闭连接收发模式 */
static const int s_shut_mode[] =
{
    SHUT_RD, SHUT_WR, SHUT_RDWR,
};

/*
---------------------------------------
    设置 SOCKADDR_IN 结构
---------------------------------------
*/
static bool_t
socket_set_addr (
  __CR_OT__ SOCKADDR_IN*    dest,
  __CR_IN__ const ansi_t*   addr,
  __CR_IN__ int16u          port
    )
{
    HOSTENT*    host;

    if (addr == NULL)
    {
        /* 可以连接所有地址 */
        dest->sin_addr.s_addr = DWORD_BE(INADDR_ANY);
    }
    else
    {
        /* 判断地址是否为域名 */
        if (is_alphaA(addr[0])) {
            host = gethostbyname(addr);
            if (host == NULL)
                return (FALSE);
            dest->sin_addr.s_addr = *(in_addr_t*)host->h_addr_list[0];
        }
        else {
            /* 直接的 IP 地址 */
            dest->sin_addr.s_addr = inet_addr(addr);
            if (dest->sin_addr.s_addr == INADDR_NONE)
                return (FALSE);
        }
    }
    dest->sin_family = AF_INET;
    dest->sin_port = WORD_BE(port);
    return (TRUE);
}

/*
=======================================
    初始化网络库
=======================================
*/
CR_API bool_t
socket_init (void_t)
{
    return (TRUE);
}

/*
=======================================
    释放网络库
=======================================
*/
CR_API void_t
socket_free (void_t)
{
}

/*
=======================================
    关闭套接字连接
=======================================
*/
CR_API void_t
socket_close (
  __CR_IN__ socket_t    netw
    )
{
    /* 关闭套接字 */
    close(((sSOCKET*)netw)->socket);
    mem_free((sSOCKET*)netw);
}

/*
=======================================
    关闭套接字传输
=======================================
*/
CR_API bool_t
socket_shutdown (
  __CR_IN__ socket_t    netw,
  __CR_IN__ uint_t      flags
    )
{
    /* 过滤参数 */
    if (flags > CR_NET_SHT_RT)
        return (FALSE);

    /* 关闭连接的传输 */
    flags = s_shut_mode[flags];
    if (shutdown(((sSOCKET*)netw)->socket, flags) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    获取接收缓冲的数据长度
=======================================
*/
CR_API bool_t
socket_input_size (
  __CR_IN__ socket_t    netw,
  __CR_OT__ uint_t*     size
    )
{
    sSOCKET*    real;

    real = (sSOCKET*)netw;
    if (ioctl(real->socket, FIONREAD, size) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    获取接收缓冲的数据长度 (阻塞)
=======================================
*/
CR_API bool_t
socket_input_size2 (
  __CR_IN__ socket_t    netw,
  __CR_OT__ uint_t*     size
    )
{
    sint_t      rett;
    fd_set      rset;
    TIMEVAL     tout;
    sSOCKET*    real;

    FD_ZERO(&rset);
    real = (sSOCKET*)netw;
    FD_SET(real->socket, &rset);
    mem_cpy(&tout, real->timeout_rd, sizeof(tout));
    rett = select(real->socket + 1, &rset, NULL, NULL, &tout);
    if (rett < 0)
        return (FALSE);

    /* 超时发生 */
    if (rett == 0)
        return (FALSE);

    /* 套接字有状态 */
    if (FD_ISSET(real->socket, &rset)) {
        if (ioctl(real->socket, FIONREAD, size) != 0)
            return (FALSE);
    }
    else {
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    服务端接收客户端 TCP 连接
=======================================
*/
CR_API socket_t
server_tcp_accept (
  __CR_IN__ socket_t    netw
    )
{
    sint_t      errs;
    fd_set      rset;
    TIMEVAL     tout;
    sSOCKET     temp;
    sSOCKET*    real;
    sSOCKET*    rett;
    socklen_t   size;

    real = (sSOCKET*)netw;
    size = sizeof(temp.remote_addr);
    struct_zero(&temp, sSOCKET);
    if (real->timeout_rd == NULL)
    {
        /* 阻塞模式 */
        temp.socket = accept(real->socket,
            (SOCKADDR*)(&temp.remote_addr), &size);
        if (temp.socket < 0)
            return (NULL);
    }
    else
    {
        /* 超时模式 */
        FD_ZERO(&rset);
        FD_SET(real->socket, &rset);
        mem_cpy(&tout, real->timeout_rd, sizeof(tout));
        errs = select(real->socket + 1, &rset, NULL, NULL, &tout);
        if (errs < 0)
            return (NULL);

        /* 超时发生 */
        if (errs == 0)
            return (NULL);

        /* 套接字有状态 */
        if (FD_ISSET(real->socket, &rset)) {
            temp.socket = accept(real->socket,
                (SOCKADDR*)(&temp.remote_addr), &size);
            if (temp.socket < 0)
                return (NULL);
        }
        else {
            return (NULL);
        }
    }

    /* 返回客户端对象 */
    temp.remote_have = TRUE;
    rett = struct_dup(&temp, sSOCKET);
    if (rett == NULL)
        close(temp.socket);
    return ((socket_t)rett);
}

/*
=======================================
    创建服务端 TCP 套接字
=======================================
*/
CR_API socket_t
server_tcp_open (
  __CR_IN__ const ansi_t*   addr,
  __CR_IN__ int16u          port
    )
{
    sint_t      opts;
    sSOCKET     temp;
    sSOCKET*    rett;

    /* 填充本地连接结构 */
    struct_zero(&temp, sSOCKET);
    if (!socket_set_addr(&temp.local_addr, addr, port))
        return (NULL);

    /* 生成 TCP 套接字 */
    temp.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (temp.socket < 0)
        return (NULL);
    opts = TRUE;
    if (setsockopt(temp.socket, SOL_SOCKET, SO_REUSEADDR,
                        &opts, sizeof(opts)) != 0)
        goto _failure;

    /* 绑定套接字并监听端口 */
    if (bind(temp.socket, (SOCKADDR*)(&temp.local_addr),
             sizeof(temp.local_addr)) != 0)
        goto _failure;
    if (listen(temp.socket, SOMAXCONN) != 0)
        goto _failure;

    /* 返回生成的对象 */
    temp.local_have = TRUE;
    rett = struct_dup(&temp, sSOCKET);
    if (rett == NULL)
        goto _failure;
    return ((socket_t)rett);

_failure:
    close(temp.socket);
    return (NULL);
}

/*
=======================================
    创建客户端 TCP 套接字
=======================================
*/
CR_API socket_t
client_tcp_open (
  __CR_IN__ const ansi_t*   addr,
  __CR_IN__ int16u          port,
  __CR_IN__ int32s          time
    )
{
    sint_t      slct;
    sint_t      opts;
    fd_set      wset;
    TIMEVAL     tout;
    sSOCKET     temp;
    sSOCKET*    rett;
    socklen_t   size;

    /* 填充远程连接结构 */
    struct_zero(&temp, sSOCKET);
    if (!socket_set_addr(&temp.remote_addr, addr, port))
        return (NULL);

    /* 生成 TCP 套接字 */
    temp.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (temp.socket < 0)
        return (NULL);

    /* 连接到远程服务端 */
    if (time < 0)
    {
        /* 阻塞模式 */
        if (connect(temp.socket, (SOCKADDR*)(&temp.remote_addr),
                    sizeof(temp.remote_addr)) != 0)
            goto _failure;
    }
    else
    {
        /* 超时模式 */
        opts = TRUE;
        if (ioctl(temp.socket, FIONBIO, &opts) != 0)
            goto _failure;
        FD_ZERO(&wset);
        FD_SET(temp.socket, &wset);
        tout.tv_sec  = (time / 1000);
        tout.tv_usec = (time % 1000) * 1000;
        connect(temp.socket, (SOCKADDR*)(&temp.remote_addr),
                sizeof(temp.remote_addr));
        slct = select(temp.socket + 1, NULL, &wset, NULL, &tout);
        if (slct < 0)
            goto _failure;

        /* 超时发生 */
        if (slct == 0)
            goto _failure;

        /* 判断是否是出错 */
        if (FD_ISSET(temp.socket, &wset)) {
            slct = 0;
            size = sizeof(slct);
            if (getsockopt(temp.socket, SOL_SOCKET, SO_ERROR,
                                &slct, &size) != 0)
                goto _failure;
            if (slct != 0)
                goto _failure;
        }
        else {
            goto _failure;
        }

        /* 恢复阻塞 */
        opts = FALSE;
        if (ioctl(temp.socket, FIONBIO, &opts) != 0)
            goto _failure;
    }

    /* 返回生成的对象 */
    temp.remote_have = TRUE;
    rett = struct_dup(&temp, sSOCKET);
    if (rett == NULL)
        goto _failure;
    return ((socket_t)rett);

_failure:
    close(temp.socket);
    return (NULL);
}

/*
=======================================
    创建客户端 TCP 套接字 (绑本地地址)
=======================================
*/
CR_API socket_t
client_tcp_open2 (
  __CR_IN__ const ansi_t*   addr,
  __CR_IN__ int16u          port,
  __CR_IN__ int32s          time,
  __CR_IN__ const ansi_t*   laddr,
  __CR_IN__ int16u          lport
    )
{
    sint_t      slct;
    sint_t      opts;
    fd_set      wset;
    TIMEVAL     tout;
    sSOCKET     temp;
    sSOCKET*    rett;
    socklen_t   size;

    /* 填充远程 & 本地连接结构 */
    struct_zero(&temp, sSOCKET);
    if (!socket_set_addr(&temp.remote_addr, addr, port))
        return (NULL);
    if (!socket_set_addr(&temp.local_addr, laddr, lport))
        return (NULL);

    /* 生成 TCP 套接字 */
    temp.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (temp.socket < 0)
        return (NULL);
    opts = TRUE;
    if (setsockopt(temp.socket, SOL_SOCKET, SO_REUSEADDR,
                        &opts, sizeof(opts)) != 0)
        goto _failure;

    /* 绑定本地地址和端口 */
    if (bind(temp.socket, (SOCKADDR*)(&temp.local_addr),
             sizeof(temp.local_addr)) != 0)
        goto _failure;

    /* 连接到远程服务端 */
    if (time < 0)
    {
        /* 阻塞模式 */
        if (connect(temp.socket, (SOCKADDR*)(&temp.remote_addr),
                    sizeof(temp.remote_addr)) != 0)
            goto _failure;
    }
    else
    {
        /* 超时模式 */
        opts = TRUE;
        if (ioctl(temp.socket, FIONBIO, &opts) != 0)
            goto _failure;
        FD_ZERO(&wset);
        FD_SET(temp.socket, &wset);
        tout.tv_sec  = (time / 1000);
        tout.tv_usec = (time % 1000) * 1000;
        connect(temp.socket, (SOCKADDR*)(&temp.remote_addr),
                sizeof(temp.remote_addr));
        slct = select(temp.socket + 1, NULL, &wset, NULL, &tout);
        if (slct < 0)
            goto _failure;

        /* 超时发生 */
        if (slct == 0)
            goto _failure;

        /* 判断是否是出错 */
        if (FD_ISSET(temp.socket, &wset)) {
            slct = 0;
            size = sizeof(slct);
            if (getsockopt(temp.socket, SOL_SOCKET, SO_ERROR,
                                &slct, &size) != 0)
                goto _failure;
            if (slct != 0)
                goto _failure;
        }
        else {
            goto _failure;
        }

        /* 恢复阻塞 */
        opts = FALSE;
        if (ioctl(temp.socket, FIONBIO, &opts) != 0)
            goto _failure;
    }

    /* 返回生成的对象 */
    temp.local_have  = TRUE;
    temp.remote_have = TRUE;
    rett = struct_dup(&temp, sSOCKET);
    if (rett == NULL)
        goto _failure;
    return ((socket_t)rett);

_failure:
    close(temp.socket);
    return (NULL);
}

/*
=======================================
    创建服务端 UDP 套接字
=======================================
*/
CR_API socket_t
server_udp_open (
  __CR_IN__ const ansi_t*   addr,
  __CR_IN__ int16u          port
    )
{
    sint_t      opts;
    sSOCKET     temp;
    sSOCKET*    rett;

    /* 填充本地连接结构 */
    struct_zero(&temp, sSOCKET);
    if (!socket_set_addr(&temp.local_addr, addr, port))
        return (NULL);

    /* 生成 UDP 套接字 */
    temp.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (temp.socket < 0)
        return (NULL);
    opts = TRUE;
    if (setsockopt(temp.socket, SOL_SOCKET, SO_REUSEADDR,
                        &opts, sizeof(opts)) != 0)
        goto _failure;

    /* 绑定套接字到本地地址 */
    if (bind(temp.socket, (SOCKADDR*)(&temp.local_addr),
             sizeof(temp.local_addr)) != 0)
        goto _failure;

    /* 返回生成的对象 */
    temp.local_have = TRUE;
    rett = struct_dup(&temp, sSOCKET);
    if (rett == NULL)
        goto _failure;
    return ((socket_t)rett);

_failure:
    close(temp.socket);
    return (NULL);
}

/*
=======================================
    创建客户端 UDP 套接字
=======================================
*/
CR_API socket_t
client_udp_open (
  __CR_IN__ const ansi_t*   addr,
  __CR_IN__ int16u          port
    )
{
    sSOCKET     temp;
    sSOCKET*    rett;

    /* 填充远程连接结构 */
    struct_zero(&temp, sSOCKET);
    if (!socket_set_addr(&temp.remote_addr, addr, port))
        return (NULL);

    /* 生成 UDP 套接字 */
    temp.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (temp.socket < 0)
        return (NULL);

    /* 返回生成的对象 */
    temp.remote_have = TRUE;
    rett = struct_dup(&temp, sSOCKET);
    if (rett == NULL)
        close(temp.socket);
    return ((socket_t)rett);
}

/*
=======================================
    创建客户端 UDP 套接字 (绑本地地址)
=======================================
*/
CR_API socket_t
client_udp_open2 (
  __CR_IN__ const ansi_t*   addr,
  __CR_IN__ int16u          port,
  __CR_IN__ const ansi_t*   laddr,
  __CR_IN__ int16u          lport
    )
{
    sint_t      opts;
    sSOCKET     temp;
    sSOCKET*    rett;

    /* 填充远程 & 本地连接结构 */
    struct_zero(&temp, sSOCKET);
    if (!socket_set_addr(&temp.remote_addr, addr, port))
        return (NULL);
    if (!socket_set_addr(&temp.local_addr, laddr, lport))
        return (NULL);

    /* 生成 UDP 套接字 */
    temp.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (temp.socket < 0)
        return (NULL);
    opts = TRUE;
    if (setsockopt(temp.socket, SOL_SOCKET, SO_REUSEADDR,
                        &opts, sizeof(opts)) != 0)
        goto _failure;

    /* 绑定本地地址和端口 */
    if (bind(temp.socket, (SOCKADDR*)(&temp.local_addr),
             sizeof(temp.local_addr)) != 0)
        goto _failure;

    /* 返回生成的对象 */
    temp.local_have  = TRUE;
    temp.remote_have = TRUE;
    rett = struct_dup(&temp, sSOCKET);
    if (rett == NULL)
        goto _failure;
    return ((socket_t)rett);

_failure:
    close(temp.socket);
    return (NULL);
}

/*
=======================================
    在 TCP 套接字上发送数据
=======================================
*/
CR_API uint_t
socket_tcp_send (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    fd_set      wset;
    SOCKET      sock;
    sint_t      rett;
    sint_t      rest;
    ansi_t*     pntr;
    TIMEVAL     tout;
    sSOCKET*    real;

    /* 过滤参数 */
    if (size == 0)
        return (0);
    if ((sint_t)size < 0)
        return (CR_U_ERROR);
    real = (sSOCKET*)netw;
    sock =  real->socket;
    rest = (sint_t )size;
    pntr = (ansi_t*)data;
    if (real->timeout_wr == NULL)
    {
        /* 阻塞模式 */
        do
        {
            /* 可能会分块发送数据 */
            rett = (sint_t)send(sock, pntr, rest, MSG_NOSIGNAL);
            if (rett > 0) {
                pntr += rett;
                rest -= rett;
            }
        } while (rett > 0 && rest > 0);
    }
    else
    {
        /* 超时模式 */
_next_send:
        FD_ZERO(&wset);
        FD_SET(sock, &wset);
        mem_cpy(&tout, real->timeout_wr, sizeof(tout));
        rett = select(sock + 1, NULL, &wset, NULL, &tout);
        if (rett < 0)
            return (CR_U_ERROR);

        /* 超时发生 */
        if (rett == 0)
            return (CR_SOCKET_TIMEOUT);

        /* 套接字有状态 */
        if (FD_ISSET(sock, &wset)) {
            rett = (sint_t)send(sock, pntr, rest, MSG_NOSIGNAL);
            if (rett > 0) {
                pntr += rett;
                rest -= rett;
                if (rest > 0)
                    goto _next_send;
            }
        }
        else {
            return (CR_U_ERROR);
        }
    }

    /* 错误判别 */
    if (rett < 0)
        return (CR_U_ERROR);
    return (size - rest);
}

/*
=======================================
    在 UDP 套接字上发送数据
=======================================
*/
CR_API uint_t
socket_udp_send (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   addr,
  __CR_IN__ int16u          port,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    fd_set      wset;
    SOCKET      sock;
    sint_t      tlen;
    sint_t      rett;
    TIMEVAL     tout;
    sSOCKET*    real;
    SOCKADDR*   sdto;

    /* 过滤参数 */
    if ((sint_t)size < 0)
        return (CR_U_ERROR);

    /* 生成远端地址 */
    real = (sSOCKET*)netw;
    if (addr == NULL) {
        if (!real->remote_have)
            return (CR_U_ERROR);
    }
    else {
        /* 填充远程连接结构 */
        if (!socket_set_addr(&real->remote_addr, addr, port))
            return (CR_U_ERROR);
        real->remote_have = TRUE;
    }

    if (size == 0)
        return (0);
    sock = real->socket;
    tlen = sizeof(SOCKADDR_IN);
    sdto = (SOCKADDR*)(&real->remote_addr);
    if (real->timeout_wr == NULL)
    {
        /* 阻塞模式 */
        rett = (sint_t)sendto(sock, (char*)data, size,
                        MSG_NOSIGNAL, sdto, tlen);
    }
    else
    {
        /* 超时模式 */
        FD_ZERO(&wset);
        FD_SET(sock, &wset);
        mem_cpy(&tout, real->timeout_wr, sizeof(tout));
        rett = select(sock + 1, NULL, &wset, NULL, &tout);
        if (rett < 0)
            return (CR_U_ERROR);

        /* 超时发生 */
        if (rett == 0)
            return (CR_SOCKET_TIMEOUT);

        /* 套接字有状态 */
        if (FD_ISSET(sock, &wset)) {
            rett = (sint_t)sendto(sock, (char*)data, size,
                            MSG_NOSIGNAL, sdto, tlen);
        }
        else {
            return (CR_U_ERROR);
        }
    }

    /* 错误判别 */
    if (rett < 0)
        return (CR_U_ERROR);
    return ((uint_t)rett);
}

/*
=======================================
    在 TCP 套接字上接收数据
=======================================
*/
CR_API uint_t
socket_tcp_recv (
  __CR_IN__ socket_t    netw,
  __CR_OT__ void_t*     data,
  __CR_IN__ uint_t      size
    )
{
    fd_set      rset;
    SOCKET      sock;
    sint_t      rett;
    sint_t      rest;
    ansi_t*     pntr;
    TIMEVAL     tout;
    sSOCKET*    real;

    /* 过滤参数 */
    if (size == 0)
        return (0);
    if ((sint_t)size < 0)
        return (CR_U_ERROR);
    real = (sSOCKET*)netw;
    sock =  real->socket;
    rest = (sint_t )size;
    pntr = (ansi_t*)data;
    if (real->timeout_rd == NULL)
    {
        /* 阻塞模式 */
        do
        {
            /* 可能会分块接收数据 */
            rett = (sint_t)recv(sock, pntr, rest, 0);
            if (rett > 0) {
                pntr += rett;
                rest -= rett;
            }
        } while (rett > 0 && rest > 0);
    }
    else
    {
        /* 超时模式 */
_next_recv:
        FD_ZERO(&rset);
        FD_SET(sock, &rset);
        mem_cpy(&tout, real->timeout_rd, sizeof(tout));
        rett = select(sock + 1, &rset, NULL, NULL, &tout);
        if (rett < 0)
            return (CR_U_ERROR);

        /* 超时发生 */
        if (rett == 0)
            return (CR_SOCKET_TIMEOUT);

        /* 套接字有状态 */
        if (FD_ISSET(sock, &rset)) {
            rett = (sint_t)recv(sock, pntr, rest, 0);
            if (rett > 0) {
                pntr += rett;
                rest -= rett;
                if (rest > 0)
                    goto _next_recv;
            }
        }
        else {
            return (CR_U_ERROR);
        }
    }

    /* 错误判别 */
    if (rett < 0)
        return (CR_U_ERROR);
    if (rett == 0)
        return (CR_SOCKET_CLOSED);
    return (size - rest);
}

/*
=======================================
    在 TCP 套接字上预读数据
=======================================
*/
CR_API uint_t
socket_tcp_peek (
  __CR_IN__ socket_t    netw,
  __CR_OT__ void_t*     data,
  __CR_IN__ uint_t      size
    )
{
    sint_t      rett;
    fd_set      rset;
    SOCKET      sock;
    TIMEVAL     tout;
    sSOCKET*    real;

    /* 过滤参数 */
    if (size == 0)
        return (0);
    if ((sint_t)size < 0)
        return (CR_U_ERROR);
    real = (sSOCKET*)netw;
    sock =  real->socket;
    if (real->timeout_rd == NULL)
    {
        /* 阻塞模式 */
        rett = (sint_t)recv(sock, (char*)data, size, MSG_PEEK);
    }
    else
    {
        /* 超时模式 */
        FD_ZERO(&rset);
        FD_SET(sock, &rset);
        mem_cpy(&tout, real->timeout_rd, sizeof(tout));
        rett = select(sock + 1, &rset, NULL, NULL, &tout);
        if (rett < 0)
            return (CR_U_ERROR);

        /* 超时发生 */
        if (rett == 0)
            return (CR_SOCKET_TIMEOUT);

        /* 套接字有状态 */
        if (FD_ISSET(sock, &rset))
            rett = (sint_t)recv(sock, (char*)data, size, MSG_PEEK);
        else
            return (CR_U_ERROR);
    }

    /* 错误判别 */
    if (rett < 0)
        return (CR_U_ERROR);
    if (rett == 0)
        return (CR_SOCKET_CLOSED);
    return ((uint_t)rett);
}

/*
=======================================
    在 UDP 套接字上接收数据
=======================================
*/
CR_API uint_t
socket_udp_recv (
  __CR_IN__ socket_t    netw,
  __CR_OT__ void_t*     data,
  __CR_IN__ uint_t      size
    )
{
    sint_t      rett;
    fd_set      rset;
    SOCKET      sock;
    TIMEVAL     tout;
    sSOCKET*    real;
    SOCKADDR*   from;
    socklen_t   flen;

    /* 过滤参数 */
    if (size == 0)
        return (0);
    if ((sint_t)size < 0)
        return (CR_U_ERROR);
    real = (sSOCKET*)netw;
    sock = real->socket;
    flen = sizeof(SOCKADDR_IN);
    from = (SOCKADDR*)(&real->remote_addr);
    if (real->timeout_rd == NULL)
    {
        /* 阻塞模式 */
        rett = (sint_t)recvfrom(sock, (char*)data, size,
                                0, from, &flen);
    }
    else
    {
        /* 超时模式 */
        FD_ZERO(&rset);
        FD_SET(sock, &rset);
        mem_cpy(&tout, real->timeout_rd, sizeof(tout));
        rett = select(sock + 1, &rset, NULL, NULL, &tout);
        if (rett < 0)
            return (CR_U_ERROR);

        /* 超时发生 */
        if (rett == 0)
            return (CR_SOCKET_TIMEOUT);

        /* 套接字有状态 */
        if (FD_ISSET(sock, &rset)) {
            rett = (sint_t)recvfrom(sock, (char*)data, size,
                                    0, from, &flen);
        }
        else {
            return (CR_U_ERROR);
        }
    }

    /* 错误判别 */
    if (rett < 0)
        return (CR_U_ERROR);
    if (rett == 0)
        return (CR_SOCKET_CLOSED);
    real->remote_have = TRUE;
    return ((uint_t)rett);
}

/*
=======================================
    在 UDP 套接字上预读数据
=======================================
*/
CR_API uint_t
socket_udp_peek (
  __CR_IN__ socket_t    netw,
  __CR_OT__ void_t*     data,
  __CR_IN__ uint_t      size
    )
{
    sint_t      rett;
    fd_set      rset;
    SOCKET      sock;
    TIMEVAL     tout;
    sSOCKET*    real;
    SOCKADDR*   from;
    socklen_t   flen;

    /* 过滤参数 */
    if (size == 0)
        return (0);
    if ((sint_t)size < 0)
        return (CR_U_ERROR);
    real = (sSOCKET*)netw;
    sock = real->socket;
    flen = sizeof(SOCKADDR_IN);
    from = (SOCKADDR*)(&real->remote_addr);
    if (real->timeout_rd == NULL)
    {
        /* 阻塞模式 */
        rett = (sint_t)recvfrom(sock, (char*)data, size,
                                MSG_PEEK, from, &flen);
    }
    else
    {
        /* 超时模式 */
        FD_ZERO(&rset);
        FD_SET(sock, &rset);
        mem_cpy(&tout, real->timeout_rd, sizeof(tout));
        rett = select(sock + 1, &rset, NULL, NULL, &tout);
        if (rett < 0)
            return (CR_U_ERROR);

        /* 超时发生 */
        if (rett == 0)
            return (CR_SOCKET_TIMEOUT);

        /* 套接字有状态 */
        if (FD_ISSET(sock, &rset)) {
            rett = (sint_t)recvfrom(sock, (char*)data, size,
                                    MSG_PEEK, from, &flen);
        }
        else {
            return (CR_U_ERROR);
        }
    }

    /* 错误判别 */
    if (rett < 0)
        return (CR_U_ERROR);
    if (rett == 0)
        return (CR_SOCKET_CLOSED);
    real->remote_have = TRUE;
    return ((uint_t)rett);
}

/*
=======================================
    设置套接字超时参数
=======================================
*/
CR_API void_t
socket_set_timeout (
  __CR_IN__ socket_t    netw,
  __CR_IN__ int32s      wr_time,
  __CR_IN__ int32s      rd_time
    )
{
    sSOCKET*    real = (sSOCKET*)netw;

    /* send()/sendto() 超时 */
    if (wr_time < 0) {
        real->timeout_wr = NULL;
    }
    else {
        real->timeout_wr = &real->wr;
        real->wr.tv_sec  = (wr_time / 1000);
        real->wr.tv_usec = (wr_time % 1000) * 1000;
    }

    /* accept()/recv()/recvfrom() 超时 */
    if (rd_time < 0) {
        real->timeout_rd = NULL;
    }
    else {
        real->timeout_rd = &real->rd;
        real->rd.tv_sec  = (rd_time / 1000);
        real->rd.tv_usec = (rd_time % 1000) * 1000;
    }
}

/*
=======================================
    获取远端主机 IP 地址端口
=======================================
*/
CR_API int16u
socket_remote_ip (
  __CR_IN__ socket_t    netw,
  __CR_OT__ int32u*     ip
    )
{
    sSOCKET*    real;

    real = (sSOCKET*)netw;
    if (!real->remote_have)
        return (0x0000);
    *ip = DWORD_BE(real->remote_addr.sin_addr.s_addr);
    return (WORD_BE(real->remote_addr.sin_port));
}

/*
=======================================
    获取远端主机 IP 地址端口A
=======================================
*/
CR_API int16u
socket_remote_ipA (
  __CR_IN__ socket_t    netw,
  __CR_OT__ ansi_t      ip[16]
    )
{
    sSOCKET*    real;

    real = (sSOCKET*)netw;
    if (!real->remote_have)
        return (0x0000);
    str_cpyNA(ip, inet_ntoa(real->remote_addr.sin_addr), 16);
    return (WORD_BE(real->remote_addr.sin_port));
}

/*
=======================================
    设置 TCP 断线检测参数
=======================================
*/
CR_API bool_t
socket_tcp_set_alive (
  __CR_IN__ socket_t    netw,
  __CR_IN__ uint_t      idle,
  __CR_IN__ uint_t      interval,
  __CR_IN__ uint_t      count
    )
{
#if !defined(SOL_TCP)
    CR_NOUSE(netw);
    CR_NOUSE(idle);
    CR_NOUSE(interval);
    CR_NOUSE(count);
    return (FALSE);
#else
    sint_t  value, opts = TRUE;
    SOCKET  sock = ((sSOCKET*)netw)->socket;

    if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE,
                    &opts, sizeof(opts)) != 0)
        return (FALSE);
    value = idle / 1000;
    setsockopt(sock, SOL_TCP, TCP_KEEPIDLE, &value, sizeof(value));
    value = interval / 1000;
    setsockopt(sock, SOL_TCP, TCP_KEEPINTVL, &value, sizeof(value));
    setsockopt(sock, SOL_TCP, TCP_KEEPCNT, &count, sizeof(count));
    return (TRUE);
#endif
}

/*
=======================================
    加入指定组播网络
=======================================
*/
CR_API bool_t
socket_mcast_enter (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   maddr,
  __CR_IN__ const ansi_t*   laddr,
  __CR_IN__ byte_t          ttl,
  __CR_IN__ byte_t          loop
    )
{
#if !defined(SOL_IP)
    CR_NOUSE(netw);
    CR_NOUSE(maddr);
    CR_NOUSE(laddr);
    CR_NOUSE(ttl);
    CR_NOUSE(loop);
    return (FALSE);
#else
    sSOCKET*    real;
    SOCKADDR_IN netm;
    SOCKADDR_IN netl;

    /* 先退出当前组播网络 */
    real = (sSOCKET*)netw;
    if (real->multic_have) {
        if (!socket_mcast_leave(netw))
            return (FALSE);
    }

    /* 使用本地地址作为默认组播网络 */
    if (maddr == NULL) {
        if (!real->local_have)
            return (FALSE);
        struct_cpy(&netm, &real->local_addr, SOCKADDR_IN);
    }
    else {
        /* 指定一个特定的组播地址 */
        if (!socket_set_addr(&netm, maddr, 0))
            return (FALSE);
    }

    /* 加入组播网络的网卡地址 */
    if (!socket_set_addr(&netl, laddr, 0))
        return (FALSE);

    /* 加入指定的组播网络 */
    real->multic_parm.imr_multiaddr.s_addr = netm.sin_addr.s_addr;
    real->multic_parm.imr_interface.s_addr = netl.sin_addr.s_addr;
    if (setsockopt(real->socket, SOL_IP, IP_ADD_MEMBERSHIP,
                   &real->multic_parm, sizeof(sIP_MREQ)) != 0)
        return (FALSE);
    real->multic_have = TRUE;

    /* 设定两个参数 */
    if (setsockopt(real->socket, SOL_IP, IP_MULTICAST_TTL,
                    &ttl, sizeof(ttl)) != 0) {
        socket_mcast_leave(netw);
        return (FALSE);
    }
    if (setsockopt(real->socket, SOL_IP, IP_MULTICAST_LOOP,
                    &loop, sizeof(loop)) != 0) {
        socket_mcast_leave(netw);
        return (FALSE);
    }
    return (TRUE);
#endif
}

/*
=======================================
    退出当前组播网络
=======================================
*/
CR_API bool_t
socket_mcast_leave (
  __CR_IN__ socket_t    netw
    )
{
#if !defined(SOL_IP)
    CR_NOUSE(netw);
    return (FALSE);
#else
    sSOCKET*    real;

    real = (sSOCKET*)netw;
    if (!real->multic_have)
        return (FALSE);
    if (setsockopt(real->socket, SOL_IP, IP_DROP_MEMBERSHIP,
                   &real->multic_parm, sizeof(sIP_MREQ)) != 0)
        return (FALSE);
    real->multic_have = FALSE;
    return (TRUE);
#endif
}

/*
=======================================
    获取网卡的信息
=======================================
*/
CR_API bool_t
netcard_get_info (
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ ansi_t          ip[16],
  __CR_OT__ ansi_t          gw[16],
  __CR_OT__ ansi_t          msk[16],
  __CR_OT__ byte_t          mac[6]
    )
{
#if !defined(IFNAMSIZ) || defined(_CR_OS_MACOSX_)
    CR_NOUSE(name);
    CR_NOUSE(ip); CR_NOUSE(gw);
    CR_NOUSE(msk); CR_NOUSE(mac);
    return (FALSE);
#else
    SOCKET      sock;
    sIFREQ      ifrq;
    SOCKADDR    sadr;
    SOCKADDR_IN sain;

    /* 准备好结构体 */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        return (FALSE);

    /* 获取 IP 地址 */
    if (ip != NULL) {
        str_cpyNA(ifrq.ifr_name, name, IFNAMSIZ);
        ifrq.ifr_name[IFNAMSIZ - 1] = 0;
        if (ioctl(sock, SIOCGIFADDR, &ifrq) != 0)
            goto _failure;
        mem_cpy(&sain, &ifrq.ifr_addr, sizeof(sain));
        str_cpyA(ip, inet_ntoa(sain.sin_addr));
    }

    /* 获取 MASK 地址 */
    if (msk != NULL) {
        str_cpyNA(ifrq.ifr_name, name, IFNAMSIZ);
        ifrq.ifr_name[IFNAMSIZ - 1] = 0;
        if (ioctl(sock, SIOCGIFNETMASK, &ifrq) != 0)
            goto _failure;
        mem_cpy(&sain, &ifrq.ifr_netmask, sizeof(sain));
        str_cpyA(msk, inet_ntoa(sain.sin_addr));
    }

    /* 获取网关地址 */
    if (gw != NULL)
        gw[0] = 0x00;

    /* 获取 MAC 地址 */
    if (mac != NULL) {
        str_cpyNA(ifrq.ifr_name, name, IFNAMSIZ);
        ifrq.ifr_name[IFNAMSIZ - 1] = 0;
        if (ioctl(sock, SIOCGIFHWADDR, &ifrq) != 0)
            goto _failure;
        mem_cpy(&sadr, &ifrq.ifr_hwaddr, sizeof(sain));
        mem_cpy(mac, sadr.sa_data, 6);
    }
    close(sock);
    return (TRUE);

_failure:
    close(sock);
    return (FALSE);
#endif
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
