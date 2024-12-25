/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-06-28  */
/*     #######          ###    ###      [UTIL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>> COMM 通讯协议函数库 <<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "morder.h"
#include "util/commlib.h"

/* 最大数据长度 */
#define COMM_MAX_LENG   65529

/*
=======================================
    COMM UART 发送
=======================================
*/
CR_API bool_t
comm_sio_send (
  __CR_IN__ uint_t              port,
  __CR_IN__ const sCOMM_CTX*    parm,
  __CR_IN__ const void_t*       data,
  __CR_IN__ int16u              size
    )
{
    int16u  hash;
    uint_t  back;
    uint_t  pack;
    byte_t* temp;

    /* 参数过滤 */
    if (size > COMM_MAX_LENG)
        return (FALSE);
    pack = size + 6;
    temp = (byte_t*)mem_malloc(pack);
    if (temp == NULL)
        return (FALSE);

    /* 组装封包 */
    temp[0] = 0x55;
    temp[1] = 0xAA;
    *(int16u*)(&temp[2]) = WORD_BE(size);
    if (data != NULL)
        mem_cpy(temp + 4, data, size);
    if (parm->hasher != NULL) {
        hash = parm->hasher(data, size);
        hash = WORD_BE(hash);
        mem_cpy(temp + pack - 2, &hash, 2);
    }
    else {
        mem_zero(temp + pack - 2, 2);
    }

    /* 数据加密 */
    if (parm->encode != NULL)
        parm->encode(parm->ctx_param, temp + 4, size);

    /* 发送协议包 */
    back = sio_write(port, temp, pack);
    mem_free(temp);
    return ((back != pack) ? FALSE : TRUE);
}

/*
=======================================
    COMM UART 读取
=======================================
*/
CR_API void_t*
comm_sio_recv (
  __CR_IN__ uint_t              port,
  __CR_IN__ const sCOMM_CTX*    parm,
  __CR_OT__ uint_t*             size
    )
{
    uint_t  back;
    int16u  pack, hash;
    byte_t  tags, *temp;

    /* 阻塞搜索包头 */
    for (;;) {
        back = sio_read(port, &tags, 1);
        if (back != 1)
            return (NULL);
        if (tags != 0x55)
            continue;
        back = sio_read(port, &tags, 1);
        if (back != 1)
            return (NULL);
        if (tags == 0xAA)
            break;
    }

    /* 接收长度 */
    back = sio_read(port, &pack, 2);
    if (back != 2)
        return (NULL);
    pack = WORD_BE(pack);
    if (pack > COMM_MAX_LENG)
        return (NULL);
    temp = (byte_t*)mem_malloc(pack);
    if (temp == NULL)
        return (NULL);

    /* 接收数据 */
    back = sio_read(port, temp, pack);
    if (back != (uint_t)pack)
        goto _failure;

    /* 接收校验 */
    back = sio_read(port, &hash, 2);
    if (back != 2)
        goto _failure;

    /* 数据解密 */
    if (parm->decode != NULL)
        parm->decode(parm->ctx_param, temp, pack);

    /* 数据校验 */
    if (parm->hasher != NULL) {
        hash = WORD_BE(hash);
        if (hash != parm->hasher(temp, pack))
            goto _failure;
    }
    else {
        if (hash != 0x0000)
            goto _failure;
    }

    /* 返回数据 */
    if (size != NULL)
        *size = pack;
    return (temp);

_failure:
    mem_free(temp);
    return (NULL);
}

/*
=======================================
    COMM UART 交互
=======================================
*/
CR_API void_t*
comm_sio_iorw (
  __CR_IN__ uint_t              port,
  __CR_IN__ const sCOMM_CTX*    parm,
  __CR_OT__ uint_t*             osize,
  __CR_IN__ const void_t*       ibuff,
  __CR_IN__ int16u              isize,
  __CR_IN__ comm_delay_t        delay
    )
{
    /* 发送主动帧 */
    sio_flush(port, CR_SIO_FLU_RT);
    if (!comm_sio_send(port, parm, ibuff, isize))
        return (NULL);

    /* 处理的延时 */
    if (delay != NULL)
        delay();

    /* 接收响应帧 */
    return (comm_sio_recv(port, parm, osize));
}

/*
=======================================
    COMM TCP 发送
=======================================
*/
CR_API bool_t
comm_tcp_send (
  __CR_IN__ socket_t            netw,
  __CR_IN__ const sCOMM_CTX*    parm,
  __CR_IN__ const void_t*       data,
  __CR_IN__ int16u              size
    )
{
    int16u  hash;
    uint_t  back;
    uint_t  pack;
    byte_t* temp;

    /* 参数过滤 */
    if (size > COMM_MAX_LENG)
        return (FALSE);
    pack = size + 6;
    temp = (byte_t*)mem_malloc(pack);
    if (temp == NULL)
        return (FALSE);

    /* 组装封包 */
    temp[0] = 0x55;
    temp[1] = 0xAA;
    *(int16u*)(&temp[2]) = WORD_BE(size);
    if (data != NULL)
        mem_cpy(temp + 4, data, size);
    if (parm->hasher != NULL) {
        hash = parm->hasher(data, size);
        hash = WORD_BE(hash);
        mem_cpy(temp + pack - 2, &hash, 2);
    }
    else {
        mem_zero(temp + pack - 2, 2);
    }

    /* 数据加密 */
    if (parm->encode != NULL)
        parm->encode(parm->ctx_param, temp + 4, size);

    /* 发送协议包 */
    back = socket_tcp_send(netw, temp, pack);
    mem_free(temp);
    return ((back != pack) ? FALSE : TRUE);
}

/*
=======================================
    COMM TCP 读取
=======================================
*/
CR_API void_t*
comm_tcp_recv (
  __CR_IN__ socket_t            netw,
  __CR_IN__ const sCOMM_CTX*    parm,
  __CR_OT__ uint_t*             size
    )
{
    uint_t  back;
    int16u  pack, hash;
    byte_t  tag[2], *temp;

    /* 接收头部 */
    back = socket_tcp_recv(netw, tag, sizeof(tag));
    if (back != sizeof(tag))
        return (NULL);
    if (tag[0] != 0x55 || tag[1] != 0xAA)
        return (NULL);

    /* 接收长度 */
    back = socket_tcp_recv(netw, &pack, 2);
    if (back != 2)
        return (NULL);
    pack = WORD_BE(pack);
    if (pack > COMM_MAX_LENG)
        return (NULL);
    temp = (byte_t*)mem_malloc(pack);
    if (temp == NULL)
        return (NULL);

    /* 接收数据 */
    back = socket_tcp_recv(netw, temp, pack);
    if (back != (uint_t)pack)
        goto _failure;

    /* 接收校验 */
    back = socket_tcp_recv(netw, &hash, 2);
    if (back != 2)
        goto _failure;

    /* 数据解密 */
    if (parm->decode != NULL)
        parm->decode(parm->ctx_param, temp, pack);

    /* 数据校验 */
    if (parm->hasher != NULL) {
        hash = WORD_BE(hash);
        if (hash != parm->hasher(temp, pack))
            goto _failure;
    }
    else {
        if (hash != 0x0000)
            goto _failure;
    }

    /* 返回数据 */
    if (size != NULL)
        *size = pack;
    return (temp);

_failure:
    mem_free(temp);
    return (NULL);
}

/*
=======================================
    COMM TCP 交互
=======================================
*/
CR_API void_t*
comm_tcp_iorw (
  __CR_IN__ socket_t            netw,
  __CR_IN__ const sCOMM_CTX*    parm,
  __CR_OT__ uint_t*             osize,
  __CR_IN__ const void_t*       ibuff,
  __CR_IN__ int16u              isize,
  __CR_IN__ comm_delay_t        delay
    )
{
    /* 发送主动帧 */
    if (!comm_tcp_send(netw, parm, ibuff, isize))
        return (NULL);

    /* 处理的延时 */
    if (delay != NULL)
        delay();

    /* 接收响应帧 */
    return (comm_tcp_recv(netw, parm, osize));
}

/*
=======================================
    COMM UDP 发送
=======================================
*/
CR_API bool_t
comm_udp_send (
  __CR_IN__ socket_t            netw,
  __CR_IN__ const sCOMM_CTX*    parm,
  __CR_IN__ const ansi_t*       addr,
  __CR_IN__ int16u              port,
  __CR_IN__ const void_t*       data,
  __CR_IN__ int16u              size
    )
{
    int16u  hash;
    uint_t  back;
    uint_t  pack;
    byte_t* temp;

    /* 参数过滤 */
    if (size > COMM_MAX_LENG)
        return (FALSE);
    pack = size + 6;
    temp = (byte_t*)mem_malloc(pack);
    if (temp == NULL)
        return (FALSE);

    /* 组装封包 */
    temp[0] = 0x55;
    temp[1] = 0xAA;
    *(int16u*)(&temp[2]) = WORD_BE(size);
    if (data != NULL)
        mem_cpy(temp + 4, data, size);
    if (parm->hasher != NULL) {
        hash = parm->hasher(data, size);
        hash = WORD_BE(hash);
        mem_cpy(temp + pack - 2, &hash, 2);
    }
    else {
        mem_zero(temp + pack - 2, 2);
    }

    /* 数据加密 */
    if (parm->encode != NULL)
        parm->encode(parm->ctx_param, temp + 4, size);

    /* 发送协议包 */
    back = socket_udp_send(netw, addr, port, temp, pack);
    mem_free(temp);
    return ((back != pack) ? FALSE : TRUE);
}

/*
=======================================
    COMM UDP 读取
=======================================
*/
CR_API void_t*
comm_udp_recv (
  __CR_IN__ socket_t            netw,
  __CR_IN__ const sCOMM_CTX*    parm,
  __CR_OT__ uint_t*             size
    )
{
    uint_t  back;
    uint_t  full;
    int16u  pack;
    int16u  hash;
    void_t* rett;
    byte_t* temp;

#if defined(_CR_NET_NO_PEEK_)
    if (!socket_input_size2(netw, &full))
        return (NULL);
    if (full < 6) {
        socket_udp_recv(netw, &back, 1);
        return (NULL);
    }
#else
    byte_t  tag[4];

    /* 偷窥头部 & 长度 */
    back = socket_udp_peek(netw, tag, sizeof(tag));
    if (back != sizeof(tag)) {
        if ((sint_t)back > 0)
            socket_udp_recv(netw, &back, 1);
        return (NULL);
    }
    if (tag[0] != 0x55 || tag[1] != 0xAA) {
        socket_udp_recv(netw, &back, 1);
        return (NULL);
    }
    pack  = tag[2];
    pack <<= 8;
    pack |= tag[3];
    if (pack > COMM_MAX_LENG) {
        socket_udp_recv(netw, &back, 1);
        return (NULL);
    }
    full = pack + 6;
#endif
    temp = (byte_t*)mem_malloc(full);
    if (temp == NULL)
        return (NULL);

    /* 接收整个协议包 */
    back = socket_udp_recv(netw, temp, full);
    if (back != full)
        goto _failure;
#if defined(_CR_NET_NO_PEEK_)
    if (temp[0] != 0x55 || temp[1] != 0xAA)
        goto _failure;
    pack  = temp[2];
    pack <<= 8;
    pack |= temp[3];
    if (pack != full - 6)
        goto _failure;
#endif
    /* 数据解密 */
    if (parm->decode != NULL)
        parm->decode(parm->ctx_param, temp + 4, pack);

    /* 数据校验 */
    hash  = temp[pack + 4];
    hash <<= 8;
    hash |= temp[pack + 5];
    if (parm->hasher != NULL) {
        hash = WORD_BE(hash);
        if (hash != parm->hasher(temp + 4, pack))
            goto _failure;
    }
    else {
        if (hash != 0x0000)
            goto _failure;
    }

    /* 返回数据 */
    if (size != NULL)
        *size = pack;
    rett = mem_dup(temp + 4, pack);
    mem_free(temp);
    return (rett);

_failure:
    mem_free(temp);
    return (NULL);
}

/*
=======================================
    COMM UDP 交互
=======================================
*/
CR_API void_t*
comm_udp_iorw (
  __CR_IN__ socket_t            netw,
  __CR_IN__ const sCOMM_CTX*    parm,
  __CR_IN__ const ansi_t*       addr,
  __CR_IN__ int16u              port,
  __CR_OT__ uint_t*             osize,
  __CR_IN__ const void_t*       ibuff,
  __CR_IN__ int16u              isize,
  __CR_IN__ comm_delay_t        delay
    )
{
    /* 发送主动帧 */
    if (!comm_udp_send(netw, parm, addr, port, ibuff, isize))
        return (NULL);

    /* 处理的延时 */
    if (delay != NULL)
        delay();

    /* 接收响应帧 */
    return (comm_udp_recv(netw, parm, osize));
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
