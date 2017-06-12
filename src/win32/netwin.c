/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-05-28  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack 网络通讯函数库 for Windows <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"
#include "memlib.h"
#include "strlib.h"

/* FUCK 这些编译器 */
#if     defined(_CR_CC_BCC_)
    #pragma warn -8004
    #pragma warn -8012
    #pragma warn -8060
    #define inline  __inline
#elif   defined(_CR_CC_MSC_)
    #pragma warning (disable: 4127)
    #if (_CR_CC_VER_ >= 1800)
        #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #endif
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

/* 手工嵌入 mstcpip.h 头文件 */
#if !defined(SIO_RCVALL)
struct  tcp_keepalive
{
    u_long  onoff;
    u_long  keepalivetime;
    u_long  keepaliveinterval;
};

#define RCVALL_OFF                      0
#define RCVALL_ON                       1
#define RCVALL_SOCKETLEVELONLY          2
/* ------------------------------------------------ */
#define SIO_RCVALL              _WSAIOW(IOC_VENDOR, 1)
#define SIO_RCVALL_MCAST        _WSAIOW(IOC_VENDOR, 2)
#define SIO_RCVALL_IGMPMCAST    _WSAIOW(IOC_VENDOR, 3)
#define SIO_KEEPALIVE_VALS      _WSAIOW(IOC_VENDOR, 4)
#define SIO_ABSORB_RTRALERT     _WSAIOW(IOC_VENDOR, 5)
#define SIO_UCAST_IF            _WSAIOW(IOC_VENDOR, 6)
#define SIO_LIMIT_BROADCASTS    _WSAIOW(IOC_VENDOR, 7)
#define SIO_INDEX_BIND          _WSAIOW(IOC_VENDOR, 8)
#define SIO_INDEX_MCASTIF       _WSAIOW(IOC_VENDOR, 9)
#define SIO_INDEX_ADD_MCAST     _WSAIOW(IOC_VENDOR,10)
#define SIO_INDEX_DEL_MCAST     _WSAIOW(IOC_VENDOR,11)
/* ------------------------------------------------ */
#endif  /* !SIO_RCVALL */

/* VC++ W4 会出警告 */
#if defined(_CR_CC_MSC_)
    #pragma warning (disable: 4127)
#endif

/* WinSocket2 导入库 */
#ifndef _CR_NO_PRAGMA_LIB_
    #if defined(_CR_OS_WINCE_)
        #pragma comment (lib, "ws2.lib")
    #else
        #pragma comment (lib, "ws2_32.lib")
    #endif
    #pragma comment (lib, "iphlpapi.lib")
#endif

/* 方便代码的书写 */
typedef struct ip_mreq          sIP_MREQ;
typedef struct tcp_keepalive    sTCP_KEEPALIVE;

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
    SD_RECEIVE, SD_SEND, SD_BOTH,
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
        dest->sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        /* 判断地址是否为域名 */
        if (!str2ip4addrA(addr, NULL)) {
            host = gethostbyname(addr);
            if (host == NULL)
                return (FALSE);
            dest->sin_addr.s_addr = *(u_long*)host->h_addr_list[0];
        }
        else {
            /* 直接的 IP 地址 */
            dest->sin_addr.s_addr = inet_addr(addr);
            if (dest->sin_addr.s_addr == INADDR_NONE)
                return (FALSE);
        }
    }
    dest->sin_family = AF_INET;
    dest->sin_port = htons(port);
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
    WSADATA wsa;

    /* 加载网络库 */
    if (WSAStartup(0x0202, &wsa) != NO_ERROR)
        return (FALSE);

    /* 对比库的版本 */
    if (wsa.wVersion != 0x0202) {
        WSACleanup();
        return (FALSE);
    }
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
    WSACleanup();
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
    closesocket(((sSOCKET*)netw)->socket);
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
    if (shutdown(((sSOCKET*)netw)->socket, flags) == SOCKET_ERROR)
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
    u_long      rdsz;
    sSOCKET*    real;

    /*
        在使用 UDP 的情况下，Win32 与 WinCE 平台这里有差异。
        Win32 返回的是总数据长度，可能包含多个包。WinCE 下返回
        的是最前面一个包的大小，不读掉大小是不会变的（同 UNIX 下）。
    */
    real = (sSOCKET*)netw;
    if (ioctlsocket(real->socket, FIONREAD, &rdsz) == SOCKET_ERROR)
        return (FALSE);
    *size = (uint_t)rdsz;
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
    u_long      rdsz;
    sSOCKET*    real;

    FD_ZERO(&rset);
    real = (sSOCKET*)netw;
    FD_SET(real->socket, &rset);
    rett = select(0, &rset, NULL, NULL, real->timeout_rd);
    if (rett == SOCKET_ERROR)
        return (FALSE);

    /* 超时发生 */
    if (rett == 0)
        return (FALSE);

    /* 套接字有状态 */
    if (FD_ISSET(real->socket, &rset)) {
        if (ioctlsocket(real->socket, FIONREAD, &rdsz) == SOCKET_ERROR)
            return (FALSE);
    }
    else {
        return (FALSE);
    }
    *size = (uint_t)rdsz;
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
    sint_t      size;
    fd_set      rset;
    sSOCKET     temp;
    sSOCKET*    real;
    sSOCKET*    rett;

    real = (sSOCKET*)netw;
    size = sizeof(temp.remote_addr);
    struct_zero(&temp, sSOCKET);
    if (real->timeout_rd == NULL)
    {
        /* 阻塞模式 */
        temp.socket = accept(real->socket,
            (SOCKADDR*)(&temp.remote_addr), &size);
        if (temp.socket == INVALID_SOCKET)
            return (NULL);
    }
    else
    {
        /* 超时模式 */
        FD_ZERO(&rset);
        FD_SET(real->socket, &rset);
        errs = select(0, &rset, NULL, NULL, real->timeout_rd);
        if (errs == SOCKET_ERROR)
            return (NULL);

        /* 超时发生 */
        if (errs == 0)
            return (NULL);

        /* 套接字有状态 */
        if (FD_ISSET(real->socket, &rset)) {
            temp.socket = accept(real->socket,
                (SOCKADDR*)(&temp.remote_addr), &size);
            if (temp.socket == INVALID_SOCKET)
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
        closesocket(temp.socket);
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
    u_long      opts;
    sSOCKET     temp;
    sSOCKET*    rett;

    /* 填充本地连接结构 */
    struct_zero(&temp, sSOCKET);
    if (!socket_set_addr(&temp.local_addr, addr, port))
        return (NULL);

    /* 生成 TCP 套接字 */
    temp.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (temp.socket == INVALID_SOCKET)
        return (NULL);
    opts = TRUE;
    if (setsockopt(temp.socket, SOL_SOCKET, SO_REUSEADDR,
            (char*)(&opts), sizeof(opts)) == SOCKET_ERROR)
        goto _failure;

    /* 绑定套接字并监听端口 */
    if (bind(temp.socket, (SOCKADDR*)(&temp.local_addr),
             sizeof(temp.local_addr)) == SOCKET_ERROR)
        goto _failure;
    if (listen(temp.socket, SOMAXCONN) == SOCKET_ERROR)
        goto _failure;

    /* 返回生成的对象 */
    temp.local_have = TRUE;
    rett = struct_dup(&temp, sSOCKET);
    if (rett == NULL)
        goto _failure;
    return ((socket_t)rett);

_failure:
    closesocket(temp.socket);
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
    sint_t      size;
    u_long      opts;
    fd_set      wset;
    TIMEVAL     tout;
    sSOCKET     temp;
    sSOCKET*    rett;

    /* 填充远程连接结构 */
    struct_zero(&temp, sSOCKET);
    if (!socket_set_addr(&temp.remote_addr, addr, port))
        return (NULL);

    /* 生成 TCP 套接字 */
    temp.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (temp.socket == INVALID_SOCKET)
        return (NULL);

    /* 连接到远程服务端 */
    if (time < 0)
    {
        /* 阻塞模式 */
        if (connect(temp.socket, (SOCKADDR*)(&temp.remote_addr),
                    sizeof(temp.remote_addr)) == SOCKET_ERROR)
            goto _failure;
    }
    else
    {
        /* 超时模式 */
        opts = TRUE;
        if (ioctlsocket(temp.socket, FIONBIO, (u_long*)&opts) == SOCKET_ERROR)
            goto _failure;
        FD_ZERO(&wset);
        FD_SET(temp.socket, &wset);
        tout.tv_sec  = (time / 1000);
        tout.tv_usec = (time % 1000) * 1000;
        connect(temp.socket, (SOCKADDR*)(&temp.remote_addr),
                sizeof(temp.remote_addr));
        slct = select(0, NULL, &wset, NULL, &tout);
        if (slct == SOCKET_ERROR)
            goto _failure;

        /* 超时发生 */
        if (slct == 0)
            goto _failure;

        /* 判断是否是出错 */
        if (FD_ISSET(temp.socket, &wset)) {
            slct = 0;
            size = sizeof(slct);
            if (getsockopt(temp.socket, SOL_SOCKET, SO_ERROR,
                    (char*)(&slct), &size) == SOCKET_ERROR)
                goto _failure;
            if (slct != 0)
                goto _failure;
        }
        else {
            goto _failure;
        }

        /* 恢复阻塞 */
        opts = FALSE;
        if (ioctlsocket(temp.socket, FIONBIO, (u_long*)&opts) == SOCKET_ERROR)
            goto _failure;
    }

    /* 返回生成的对象 */
    temp.remote_have = TRUE;
    rett = struct_dup(&temp, sSOCKET);
    if (rett == NULL)
        goto _failure;
    return ((socket_t)rett);

_failure:
    closesocket(temp.socket);
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
    sint_t      size;
    u_long      opts;
    fd_set      wset;
    TIMEVAL     tout;
    sSOCKET     temp;
    sSOCKET*    rett;

    /* 填充远程 & 本地连接结构 */
    struct_zero(&temp, sSOCKET);
    if (!socket_set_addr(&temp.remote_addr, addr, port))
        return (NULL);
    if (!socket_set_addr(&temp.local_addr, laddr, lport))
        return (NULL);

    /* 生成 TCP 套接字 */
    temp.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (temp.socket == INVALID_SOCKET)
        return (NULL);
    opts = TRUE;
    if (setsockopt(temp.socket, SOL_SOCKET, SO_REUSEADDR,
            (char*)(&opts), sizeof(opts)) == SOCKET_ERROR)
        goto _failure;

    /* 绑定本地地址和端口 */
    if (bind(temp.socket, (SOCKADDR*)(&temp.local_addr),
             sizeof(temp.local_addr)) == SOCKET_ERROR)
        goto _failure;

    /* 连接到远程服务端 */
    if (time < 0)
    {
        /* 阻塞模式 */
        if (connect(temp.socket, (SOCKADDR*)(&temp.remote_addr),
                    sizeof(temp.remote_addr)) == SOCKET_ERROR)
            goto _failure;
    }
    else
    {
        /* 超时模式 */
        opts = TRUE;
        if (ioctlsocket(temp.socket, FIONBIO, (u_long*)&opts) == SOCKET_ERROR)
            goto _failure;
        FD_ZERO(&wset);
        FD_SET(temp.socket, &wset);
        tout.tv_sec  = (time / 1000);
        tout.tv_usec = (time % 1000) * 1000;
        connect(temp.socket, (SOCKADDR*)(&temp.remote_addr),
                sizeof(temp.remote_addr));
        slct = select(0, NULL, &wset, NULL, &tout);
        if (slct == SOCKET_ERROR)
            goto _failure;

        /* 超时发生 */
        if (slct == 0)
            goto _failure;

        /* 判断是否是出错 */
        if (FD_ISSET(temp.socket, &wset)) {
            slct = 0;
            size = sizeof(slct);
            if (getsockopt(temp.socket, SOL_SOCKET, SO_ERROR,
                    (char*)(&slct), &size) == SOCKET_ERROR)
                goto _failure;
            if (slct != 0)
                goto _failure;
        }
        else {
            goto _failure;
        }

        /* 恢复阻塞 */
        opts = FALSE;
        if (ioctlsocket(temp.socket, FIONBIO, (u_long*)&opts) == SOCKET_ERROR)
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
    closesocket(temp.socket);
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
    u_long      opts;
    sSOCKET     temp;
    sSOCKET*    rett;

    /* 填充本地连接结构 */
    struct_zero(&temp, sSOCKET);
    if (!socket_set_addr(&temp.local_addr, addr, port))
        return (NULL);

    /* 生成 UDP 套接字 */
    temp.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (temp.socket == INVALID_SOCKET)
        return (NULL);
    opts = TRUE;
    if (setsockopt(temp.socket, SOL_SOCKET, SO_REUSEADDR,
            (char*)(&opts), sizeof(opts)) == SOCKET_ERROR)
        goto _failure;

    /* 绑定套接字到本地地址 */
    if (bind(temp.socket, (SOCKADDR*)(&temp.local_addr),
             sizeof(temp.local_addr)) == SOCKET_ERROR)
        goto _failure;

    /* 返回生成的对象 */
    temp.local_have = TRUE;
    rett = struct_dup(&temp, sSOCKET);
    if (rett == NULL)
        goto _failure;
    return ((socket_t)rett);

_failure:
    closesocket(temp.socket);
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
    if (temp.socket == INVALID_SOCKET)
        return (NULL);

    /* 返回生成的对象 */
    temp.remote_have = TRUE;
    rett = struct_dup(&temp, sSOCKET);
    if (rett == NULL)
        closesocket(temp.socket);
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
    u_long      opts;
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
    if (temp.socket == INVALID_SOCKET)
        return (NULL);
    opts = TRUE;
    if (setsockopt(temp.socket, SOL_SOCKET, SO_REUSEADDR,
            (char*)(&opts), sizeof(opts)) == SOCKET_ERROR)
        goto _failure;

    /* 绑定本地地址和端口 */
    if (bind(temp.socket, (SOCKADDR*)(&temp.local_addr),
             sizeof(temp.local_addr)) == SOCKET_ERROR)
        goto _failure;

    /* 返回生成的对象 */
    temp.local_have  = TRUE;
    temp.remote_have = TRUE;
    rett = struct_dup(&temp, sSOCKET);
    if (rett == NULL)
        goto _failure;
    return ((socket_t)rett);

_failure:
    closesocket(temp.socket);
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
            rett = send(sock, pntr, rest, 0);
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
        rett = select(0, NULL, &wset, NULL, real->timeout_wr);
        if (rett == SOCKET_ERROR)
            return (CR_U_ERROR);

        /* 超时发生 */
        if (rett == 0)
            return (CR_SOCKET_TIMEOUT);

        /* 套接字有状态 */
        if (FD_ISSET(sock, &wset)) {
            rett = send(sock, pntr, rest, 0);
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
        rett = sendto(sock, (char*)data, size, 0, sdto, tlen);
    }
    else
    {
        /* 超时模式 */
        FD_ZERO(&wset);
        FD_SET(sock, &wset);
        rett = select(0, NULL, &wset, NULL, real->timeout_wr);
        if (rett == SOCKET_ERROR)
            return (CR_U_ERROR);

        /* 超时发生 */
        if (rett == 0)
            return (CR_SOCKET_TIMEOUT);

        /* 套接字有状态 */
        if (FD_ISSET(sock, &wset))
            rett = sendto(sock, (char*)data, size, 0, sdto, tlen);
        else
            return (CR_U_ERROR);
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
            rett = recv(sock, pntr, rest, 0);
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
        rett = select(0, &rset, NULL, NULL, real->timeout_rd);
        if (rett == SOCKET_ERROR)
            return (CR_U_ERROR);

        /* 超时发生 */
        if (rett == 0)
            return (CR_SOCKET_TIMEOUT);

        /* 套接字有状态 */
        if (FD_ISSET(sock, &rset)) {
            rett = recv(sock, pntr, rest, 0);
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
        rett = recv(sock, (char*)data, size, MSG_PEEK);
    }
    else
    {
        /* 超时模式 */
        FD_ZERO(&rset);
        FD_SET(sock, &rset);
        rett = select(0, &rset, NULL, NULL, real->timeout_rd);
        if (rett == SOCKET_ERROR)
            return (CR_U_ERROR);

        /* 超时发生 */
        if (rett == 0)
            return (CR_SOCKET_TIMEOUT);

        /* 套接字有状态 */
        if (FD_ISSET(sock, &rset))
            rett = recv(sock, (char*)data, size, MSG_PEEK);
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
    fd_set      rset;
    SOCKET      sock;
    sint_t      flen;
    sint_t      rett;
    sSOCKET*    real;
    SOCKADDR*   from;

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
        rett = recvfrom(sock, (char*)data, size, 0, from, &flen);
    }
    else
    {
        /* 超时模式 */
        FD_ZERO(&rset);
        FD_SET(sock, &rset);
        rett = select(0, &rset, NULL, NULL, real->timeout_rd);
        if (rett == SOCKET_ERROR)
            return (CR_U_ERROR);

        /* 超时发生 */
        if (rett == 0)
            return (CR_SOCKET_TIMEOUT);

        /* 套接字有状态 */
        if (FD_ISSET(sock, &rset))
            rett = recvfrom(sock, (char*)data, size, 0, from, &flen);
        else
            return (CR_U_ERROR);
    }

    /* 错误判别 */
    if (rett < 0)
    {
        /* UDP 需要容忍这个错误 */
        rett = WSAGetLastError();
        if (rett == WSAEMSGSIZE)
            return ((uint_t)size);

        /* 其他值表示真正的出错 */
        return (CR_U_ERROR);
    }
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
    fd_set      rset;
    SOCKET      sock;
    sint_t      flen;
    sint_t      rett;
    sSOCKET*    real;
    SOCKADDR*   from;

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
        rett = recvfrom(sock, (char*)data, size, MSG_PEEK, from, &flen);
    }
    else
    {
        /* 超时模式 */
        FD_ZERO(&rset);
        FD_SET(sock, &rset);
        rett = select(0, &rset, NULL, NULL, real->timeout_rd);
        if (rett == SOCKET_ERROR)
            return (CR_U_ERROR);

        /* 超时发生 */
        if (rett == 0)
            return (CR_SOCKET_TIMEOUT);

        /* 套接字有状态 */
        if (FD_ISSET(sock, &rset))
            rett = recvfrom(sock, (char*)data, size, MSG_PEEK, from, &flen);
        else
            return (CR_U_ERROR);
    }

    /* 错误判别 */
    if (rett < 0)
    {
        /* UDP 需要容忍这个错误 */
        rett = WSAGetLastError();
        if (rett == WSAEMSGSIZE)
            return ((uint_t)size);

        /* 其他值表示真正的出错 */
        return (CR_U_ERROR);
    }
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
    *ip = (int32u)ntohl(real->remote_addr.sin_addr.s_addr);
    return (ntohs(real->remote_addr.sin_port));
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
    return (ntohs(real->remote_addr.sin_port));
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
    DWORD           bytes_return = 0;
    sSOCKET*        real = (sSOCKET*)netw;
    sTCP_KEEPALIVE  in_keep_alive = { 0 };
    sTCP_KEEPALIVE  ot_keep_alive = { 0 };

    CR_NOUSE(count);
    in_keep_alive.onoff = 1;
    in_keep_alive.keepalivetime = idle;
    in_keep_alive.keepaliveinterval = interval;
    if (WSAIoctl(real->socket, SIO_KEEPALIVE_VALS,
                 (LPVOID)(&in_keep_alive), sizeof(sTCP_KEEPALIVE),
                 (LPVOID)(&ot_keep_alive), sizeof(sTCP_KEEPALIVE),
                    &bytes_return, NULL, NULL) == SOCKET_ERROR)
        return (FALSE);
    return (TRUE);
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
    DWORD       parm;
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
        /*
            这里只有在 Linux 等系统下有用
            因为它们必须先绑定组播地址和端口
        */
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
    if (setsockopt(real->socket, IPPROTO_IP, IP_ADD_MEMBERSHIP,
            (char*)(&real->multic_parm), sizeof(sIP_MREQ)) == SOCKET_ERROR)
        return (FALSE);
    real->multic_have = TRUE;

    /* 设定两个参数 */
    parm = (DWORD)ttl;
    if (setsockopt(real->socket, IPPROTO_IP, IP_MULTICAST_TTL,
            (char*)(&parm), sizeof(DWORD)) == SOCKET_ERROR) {
        socket_mcast_leave(netw);
        return (FALSE);
    }
#if defined(_CR_OS_WINCE_)
    CR_NOUSE(loop);
#else
    parm = (DWORD)loop;
    if (setsockopt(real->socket, IPPROTO_IP, IP_MULTICAST_LOOP,
            (char*)(&parm), sizeof(DWORD)) == SOCKET_ERROR) {
        socket_mcast_leave(netw);
        return (FALSE);
    }
#endif
    return (TRUE);
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
    sSOCKET*    real;

    real = (sSOCKET*)netw;
    if (!real->multic_have)
        return (FALSE);
    if (setsockopt(real->socket, IPPROTO_IP, IP_DROP_MEMBERSHIP,
            (char*)(&real->multic_parm), sizeof(sIP_MREQ)) == SOCKET_ERROR)
        return (FALSE);
    real->multic_have = FALSE;
    return (TRUE);
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
    ULONG               size;
    PIP_ADAPTER_INFO    list;
    PIP_ADAPTER_INFO    node;

    /* 获取网卡参数列表 */
    list = struct_new(IP_ADAPTER_INFO);
    if (list == NULL)
        return (FALSE);
    size = sizeof(IP_ADAPTER_INFO);
    if (GetAdaptersInfo(list, &size) == ERROR_BUFFER_OVERFLOW) {
        mem_free(list);
        list = (IP_ADAPTER_INFO*)mem_malloc(size);
        if (list == NULL)
            return (FALSE);
    }
    if (GetAdaptersInfo(list, &size) != NO_ERROR) {
        mem_free(list);
        return (FALSE);
    }

    /* 查找有用的网卡参数 */
    for (node = list; node != NULL; node = node->Next)
    {
        /* 根据描述来查找 */
        if (node->Type == MIB_IF_TYPE_ETHERNET &&
            str_cmpA(name, node->Description) == 0) {
            if (ip != NULL)
                str_cpyA(ip, node->IpAddressList.IpAddress.String);
            if (msk != NULL)
                str_cpyA(msk, node->IpAddressList.IpMask.String);
            if (gw != NULL)
                str_cpyA(gw, node->GatewayList.IpAddress.String);
            if (mac != NULL)
                mem_cpy(mac, node->Address, 6);
            mem_free(list);
            return (TRUE);
        }
    }
    mem_free(list);
    return (FALSE);
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
