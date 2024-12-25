/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-12-01  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> SDT22 通讯协议函数库 <<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "memlib.h"
#include "morder.h"
#include "util/commlib.h"

/* SDT22 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  head;   /* 标志0x55 */
/*001*/ int16u  addr;   /* 设备地址 */
/*003*/ int16u  leng;   /* 数据长度 */
/*005*/ byte_t  sum8;   /* 参数校验 */
/*006*/ byte_t  tail;   /* 标志0xAA */
/*007*/
} CR_PACKED sSDT22_HDR;

CR_TYPEDEF struct
{
#if 0
        byte_t  data[leng-2];   /* 数据 */
#endif
        int16u  crc16c_sum_be;  /* 校验 */

} CR_PACKED sSDT22_DAT;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
---------------------------------------
    SDT22 读取包头
---------------------------------------
*/
static int16u
sdt22_get_head (
  __CR_IN__ const sSDT22_HDR*   head,
  __CR_IN__ int16u              addr
    )
{
    int16u  addc;
    int16u  lenc;
    byte_t  sum8;
    byte_t* pntr;

    /* 结构性检查 */
    if (head->head != 0x55 ||
        head->tail != 0xAA)
        return (0);
    addc = WORD_BE(head->addr);
    lenc = WORD_BE(head->leng);
    if (addc != addr ||
        lenc < sizeof(sSDT22_DAT))
        return (0);

    /* 参数的校验 */
    pntr = (byte_t*)head;
    sum8 =        pntr[1];
    sum8 = sum8 + pntr[2];
    sum8 = sum8 + pntr[3];
    sum8 = sum8 + pntr[4];
    if (head->sum8 != sum8)
        return (0);
    return (lenc);
}

/*
---------------------------------------
    SDT22 设置包头
---------------------------------------
*/
static void_t
sdt22_set_head (
  __CR_OT__ sSDT22_HDR* head,
  __CR_IN__ int16u      addr,
  __CR_IN__ int16u      leng
    )
{
    byte_t  sum8;
    byte_t* pntr;

    /* 填充头结构 */
    head->head = 0x55;
    head->tail = 0xAA;
    head->addr = WORD_BE(addr);
    head->leng = WORD_BE(leng);
    pntr = (byte_t*)head;
    sum8 =        pntr[1];
    sum8 = sum8 + pntr[2];
    sum8 = sum8 + pntr[3];
    sum8 = sum8 + pntr[4];
    head->sum8 = sum8;
}

/*
---------------------------------------
    SDT22 生成校验
---------------------------------------
*/
static void_t
sdt22_set_hash (
  __CR_IO__ void_t* data,
  __CR_IN__ int16u  size
    )
{
    int16u  hash;

    hash = hash_crc16c_total(data, size - 2);
    hash = WORD_BE(hash);
    mem_cpy((byte_t*)data + size - 2, &hash, 2);
}

/*
---------------------------------------
    SDT22 检查校验
---------------------------------------
*/
static bool_t
sdt22_get_hash (
  __CR_IN__ const void_t*   data,
  __CR_IN__ int16u          size
    )
{
    int16u  hash;

    hash = hash_crc16c_total(data, size - 2);
    hash = WORD_BE(hash);
    if (mem_cmp(&hash, (byte_t*)data + size - 2, 2) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    SDT22 UART 发送
=======================================
*/
CR_API bool_t
sdt22_sio_send (
  __CR_IN__ uint_t          port,
  __CR_IN__ int16u          addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ int16u          size
    )
{
    uint_t      back;
    uint_t      pack;
    byte_t*     temp;
    sSDT22_HDR* head;
    sSDT22_DAT* body;

    /* 数据长度过滤 */
    if (size > 65535 - sizeof(sSDT22_DAT))
        return (FALSE);
    pack = size + sizeof(sSDT22_DAT);
#if defined(_CR_SYS16_)
    if (pack > 65535 - sizeof(sSDT22_HDR))
        return (FALSE);
#endif
    temp = (byte_t*)mem_malloc(pack + sizeof(sSDT22_HDR));
    if (temp == NULL)
        return (FALSE);

    /* 填充好所有数据 */
    head = (sSDT22_HDR*)(temp);
    body = (sSDT22_DAT*)(head + 1);
    if (data != NULL)
        mem_cpy(body, data, size);
    sdt22_set_head(head, addr, (int16u)pack);
    sdt22_set_hash(body, (int16u)pack);

    /* 发送协议包 */
    pack += sizeof(sSDT22_HDR);
    back = sio_write(port, temp, pack);
    mem_free(temp);
    return ((back != pack) ? FALSE : TRUE);
}

/*
=======================================
    SDT22 UART 读取
=======================================
*/
CR_API bool_t
sdt22_sio_recv (
  __CR_IN__ uint_t  port,
  __CR_IN__ int16u  addr,
  __CR_OT__ void_t* data,
  __CR_IO__ uint_t* size
    )
{
    uint_t      back;
    uint_t      pack;
    byte_t*     pntr;
    sSDT22_HDR  head;

    /* 阻塞搜索包头 */
    pntr = (byte_t*)(&head);
    for (;;) {
        back = sio_read(port, pntr, 1);
        if (back != 1)
            return (FALSE);
        if (pntr[0] == 0x55)
            break;
    }
    back = sio_read(port, pntr + 1, sizeof(head) - 1);
    if (back != sizeof(head) - 1)
        return (FALSE);

    /* 校验协议包头 */
    pack = sdt22_get_head(&head, addr);
    if (pack == 0)
        return (FALSE);

    /* 过滤返回的数据长度 */
    if (size != NULL) {
        if (pack > *size)
            return (FALSE);
        *size = pack - sizeof(sSDT22_DAT);
    }

    /* 接收后续数据 */
    back = sio_read(port, data, pack);
    if (back != pack)
        return (FALSE);
    return (sdt22_get_hash(data, (int16u)pack));
}

/*
=======================================
    SDT22 UART 交互
=======================================
*/
CR_API bool_t
sdt22_sio_iorw (
  __CR_IN__ uint_t          port,
  __CR_IN__ int16u          daddr,
  __CR_IN__ int16u          saddr,
  __CR_OT__ void_t*         obuff,
  __CR_IO__ uint_t*         osize,
  __CR_IN__ const void_t*   ibuff,
  __CR_IN__ int16u          isize,
  __CR_IN__ comm_delay_t    delay
    )
{
    /* 发送主动帧 */
    sio_flush(port, CR_SIO_FLU_RT);
    if (!sdt22_sio_send(port, daddr, ibuff, isize))
        return (FALSE);

    /* 处理的延时 */
    if (delay != NULL)
        delay();

    /* 接收响应帧 */
    return (sdt22_sio_recv(port, saddr, obuff, osize));
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
