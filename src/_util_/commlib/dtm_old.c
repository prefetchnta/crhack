/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-11-14  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> DTM_OLD 通讯协议函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "morder.h"
#include "util/commlib.h"

/* DTM_OLD 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  head[3];    /* 标志1A 72 73 */
/*003*/ int16u  data_size;  /* 后续数据长度 */
/*005*/
} CR_PACKED sDTMO_HDR;

CR_TYPEDEF struct
{
#if 0
        byte_t  data[leng-1];   /* 数据 */
#endif
        byte_t  check_sum8b;    /* 校验 */

} CR_PACKED sDTMO_DAT;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
---------------------------------------
    DTM_OLD 读取包头
---------------------------------------
*/
static int16u
dtmo_get_head (
  __CR_IN__ const sDTMO_HDR*    head
    )
{
    int16u  size;

    /* 结构性检查 */
    if (head->head[0] != 0x1A ||
        head->head[1] != 0x72 ||
        head->head[2] != 0x73)
        return (0);
    size = WORD_BE(head->data_size);
    if (size < sizeof(sDTMO_DAT))
        return (0);
    return (size);
}

/*
---------------------------------------
    DTM_OLD 设置包头
---------------------------------------
*/
static void_t
dtmo_set_head (
  __CR_OT__ sDTMO_HDR*  head,
  __CR_IN__ int16u      size
    )
{
    /* 填充头结构 */
    head->head[0] = 0x1A;
    head->head[1] = 0x72;
    head->head[2] = 0x73;
    head->data_size = WORD_BE(size);
}

/*
---------------------------------------
    DTM_OLD 生成校验
---------------------------------------
*/
static void_t
dtmo_set_hash (
  __CR_IO__ void_t* data,
  __CR_IN__ int16u  size
    )
{
    byte_t  hash = 0;

    size -= 1;
    for (; size != 0; size--) {
        hash = *(byte_t*)data + hash;
        data =  (byte_t*)data + 1;
    }
    *(byte_t*)data = hash;
}

/*
---------------------------------------
    DTM_OLD 检查校验
---------------------------------------
*/
static bool_t
dtmo_get_hash (
  __CR_IN__ const sDTMO_HDR*    head,
  __CR_IN__ const void_t*       data,
  __CR_IN__ int16u              size
    )
{
    byte_t  hash = 0, *pntr;
    uint_t  idx = sizeof(sDTMO_HDR);

    pntr = (byte_t*)head;
    for (; idx != 0; idx--) {
        hash = *(byte_t*)pntr + hash;
        pntr =  (byte_t*)pntr + 1;
    }

    size -= 1;
    for (; size != 0; size--) {
        hash = *(byte_t*)data + hash;
        data =  (byte_t*)data + 1;
    }
    if (hash != *(byte_t*)data)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    DTM_OLD UART 发送
=======================================
*/
CR_API bool_t
dtmo_sio_send (
  __CR_IN__ uint_t          port,
  __CR_IN__ const void_t*   data,
  __CR_IN__ int16u          size
    )
{
    uint_t      back;
    uint_t      pack;
    byte_t*     temp;
    sDTMO_HDR*  head;
    sDTMO_DAT*  body;

    /* 数据长度过滤 */
    if (size > 65535 - sizeof(sDTMO_DAT))
        return (FALSE);
    pack = size + sizeof(sDTMO_DAT);
    if (pack > 65535 - sizeof(sDTMO_HDR))
        return (FALSE);
    temp = (byte_t*)mem_malloc(pack + sizeof(sDTMO_HDR));
    if (temp == NULL)
        return (FALSE);

    /* 填充好所有数据 */
    head = (sDTMO_HDR*)(temp);
    body = (sDTMO_DAT*)(head + 1);
    if (data != NULL)
        mem_cpy(body, data, size);
    dtmo_set_head(head, (int16u)pack);
    pack += sizeof(sDTMO_HDR);
    dtmo_set_hash(head, (int16u)pack);

    /* 发送协议包 */
    back = sio_write(port, temp, pack);
    mem_free(temp);
    return ((back != pack) ? FALSE : TRUE);
}

/*
=======================================
    DTM_OLD UART 读取
=======================================
*/
CR_API bool_t
dtmo_sio_recv (
  __CR_IN__ uint_t  port,
  __CR_OT__ void_t* data,
  __CR_IO__ uint_t* size
    )
{
    uint_t      back;
    uint_t      pack;
    byte_t      tags;
    byte_t*     pntr;
    sDTMO_HDR   head;

    /* 阻塞搜索包头 */
    tags = 0x1A;
    pntr = (byte_t*)(&head);
    for (;;) {
        back = sio_read(port, pntr, 1);
        if (back != 1)
            return (FALSE);
        if (pntr[0] == tags) {
            if (tags == 0x73) {
                pntr[0] = 0x1A;
                pntr[1] = 0x72;
                pntr[2] = 0x73;
                break;
            }
            tags = (tags == 0x1A) ? 0x72 : 0x73;
        }
        else {
            tags = 0x1A;
        }
    }
    back = sio_read(port, pntr + 3, sizeof(head) - 3);
    if (back != sizeof(head) - 3)
        return (FALSE);

    /* 校验协议包头 */
    pack = dtmo_get_head(&head);
    if (pack == 0)
        return (FALSE);

    /* 过滤返回的数据长度 */
    if (size != NULL) {
        if (pack > *size)
            return (FALSE);
        *size = pack - sizeof(sDTMO_DAT);
    }

    /* 接收后续数据 */
    back = sio_read(port, data, pack);
    if (back != pack)
        return (FALSE);
    return (dtmo_get_hash(&head, data, (int16u)pack));
}

/*
=======================================
    DTM_OLD UART 交互
=======================================
*/
CR_API bool_t
dtmo_sio_iorw (
  __CR_IN__ uint_t          port,
  __CR_OT__ void_t*         obuff,
  __CR_IO__ uint_t*         osize,
  __CR_IN__ const void_t*   ibuff,
  __CR_IN__ int16u          isize,
  __CR_IN__ comm_delay_t    delay
    )
{
    /* 发送主动帧 */
    sio_flush(port, CR_SIO_FLU_RT);
    if (!dtmo_sio_send(port, ibuff, isize))
        return (FALSE);

    /* 处理的延时 */
    if (delay != NULL)
        delay();

    /* 接收响应帧 */
    return (dtmo_sio_recv(port, obuff, osize));
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
