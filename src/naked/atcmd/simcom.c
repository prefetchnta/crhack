/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-16  */
/*     #######          ###    ###      [KRNL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>> CrHack SIMCOM 3G/4G 模块驱动函数库 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"
#include "memlib.h"

/*
=======================================
    创建客户端 TCP 套接字
=======================================
*/
CR_API socket_t
simcom_client_tcp_open (
  __CR_IN__ const ansi_t*   addr,
  __CR_IN__ int16u          port,
  __CR_IN__ int32s          time
    )
{

}

/*
=======================================
    创建客户端 UDP 套接字
=======================================
*/
CR_API socket_t
simcom_client_udp_open (
  __CR_IN__ const ansi_t*   addr,
  __CR_IN__ int16u          port
    )
{

}

/*
=======================================
    关闭套接字连接
=======================================
*/
CR_API void_t
simcom_socket_close (
  __CR_IN__ socket_t    netw
    )
{

}

/*
=======================================
    在 TCP 套接字上发送数据
=======================================
*/
CR_API uint_t
simcom_socket_tcp_send (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{

}

/*
=======================================
    在 UDP 套接字上发送数据
=======================================
*/
CR_API uint_t
simcom_socket_udp_send (
  __CR_IN__ socket_t        netw,
  __CR_IN__ const ansi_t*   addr,
  __CR_IN__ int16u          port,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{

}

/*
=======================================
    在 TCP 套接字上接收数据
=======================================
*/
CR_API uint_t
simcom_socket_tcp_recv (
  __CR_IN__ socket_t    netw,
  __CR_OT__ void_t*     data,
  __CR_IN__ uint_t      size
    )
{

}

/*
=======================================
    在 TCP 套接字上预读数据
=======================================
*/
CR_API uint_t
simcom_socket_tcp_peek (
  __CR_IN__ socket_t    netw,
  __CR_OT__ void_t*     data,
  __CR_IN__ uint_t      size
    )
{

}

/*
=======================================
    在 UDP 套接字上接收数据
=======================================
*/
CR_API uint_t
simcom_socket_udp_recv (
  __CR_IN__ socket_t    netw,
  __CR_OT__ void_t*     data,
  __CR_IN__ uint_t      size
    )
{

}

/*
=======================================
    在 UDP 套接字上预读数据
=======================================
*/
CR_API uint_t
simcom_socket_udp_peek (
  __CR_IN__ socket_t    netw,
  __CR_OT__ void_t*     data,
  __CR_IN__ uint_t      size
    )
{

}

/*
=======================================
    设置套接字超时参数
=======================================
*/
CR_API void_t
simcom_socket_set_timeout (
  __CR_IN__ socket_t    netw,
  __CR_IN__ int32s      wr_time,
  __CR_IN__ int32s      rd_time
    )
{

}

/*
=======================================
    获取接收缓冲的数据长度
=======================================
*/
CR_API bool_t
simcom_socket_input_size (
  __CR_IN__ socket_t    netw,
  __CR_OT__ uint_t*     size
    )
{

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
