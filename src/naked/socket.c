/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-19  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 网络通讯函数库 for Naked <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"

/* 用户自己定义 */
#if !defined(NAKED_INL)
extern iSOCKET  g_socket;
#endif

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
    if (g_socket.socket_close == NULL)
        return;
    g_socket.socket_close(netw);
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
    if (g_socket.socket_shutdown == NULL)
        return (FALSE);
    return (g_socket.socket_shutdown(netw, flags));
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
    if (g_socket.socket_input_size == NULL)
        return (FALSE);
    return (g_socket.socket_input_size(netw, size));
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
    if (g_socket.socket_input_size2 == NULL)
        return (FALSE);
    return (g_socket.socket_input_size2(netw, size));
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
    if (g_socket.server_tcp_accept == NULL)
        return (NULL);
    return (g_socket.server_tcp_accept(netw));
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
    if (g_socket.server_tcp_open == NULL)
        return (NULL);
    return (g_socket.server_tcp_open(addr, port));
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
    if (g_socket.client_tcp_open == NULL)
        return (NULL);
    return (g_socket.client_tcp_open(addr, port, time));
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
    if (g_socket.client_tcp_open2 == NULL)
        return (NULL);
    return (g_socket.client_tcp_open2(addr, port, time, laddr, lport));
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
    if (g_socket.server_udp_open == NULL)
        return (NULL);
    return (g_socket.server_udp_open(addr, port));
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
    if (g_socket.client_udp_open == NULL)
        return (NULL);
    return (g_socket.client_udp_open(addr, port));
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
    if (g_socket.client_udp_open2 == NULL)
        return (NULL);
    return (g_socket.client_udp_open2(addr, port, laddr, lport));
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
    if (g_socket.socket_tcp_send == NULL)
        return (CR_U_ERROR);
    return (g_socket.socket_tcp_send(netw, data, size));
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
    if (g_socket.socket_udp_send == NULL)
        return (CR_U_ERROR);
    return (g_socket.socket_udp_send(netw, addr, port, data, size));
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
    if (g_socket.socket_tcp_recv == NULL)
        return (CR_U_ERROR);
    return (g_socket.socket_tcp_recv(netw, data, size));
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
    if (g_socket.socket_tcp_peek == NULL)
        return (CR_U_ERROR);
    return (g_socket.socket_tcp_peek(netw, data, size));
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
    if (g_socket.socket_udp_recv == NULL)
        return (CR_U_ERROR);
    return (g_socket.socket_udp_recv(netw, data, size));
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
    if (g_socket.socket_udp_peek == NULL)
        return (CR_U_ERROR);
    return (g_socket.socket_udp_peek(netw, data, size));
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
    if (g_socket.socket_set_timeout == NULL)
        return;
    g_socket.socket_set_timeout(netw, wr_time, rd_time);
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
    if (g_socket.socket_remote_ip == NULL)
        return (0x0000);
    return (g_socket.socket_remote_ip(netw, ip));
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
    if (g_socket.socket_remote_ipA == NULL)
        return (0x0000);
    return (g_socket.socket_remote_ipA(netw, ip));
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
    if (g_socket.socket_tcp_set_alive == NULL)
        return (FALSE);
    return (g_socket.socket_tcp_set_alive(netw, idle, interval, count));
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
    if (g_socket.socket_mcast_enter == NULL)
        return (FALSE);
    return (g_socket.socket_mcast_enter(netw, maddr, laddr, ttl, loop));
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
    if (g_socket.socket_mcast_leave == NULL)
        return (FALSE);
    return (g_socket.socket_mcast_leave(netw));
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
    if (g_socket.netcard_get_info == NULL)
        return (FALSE);
    return (g_socket.netcard_get_info(name, ip, gw, msk, mac));
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
