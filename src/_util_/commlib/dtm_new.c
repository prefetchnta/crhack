/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-11-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> DTM_NEW 通讯协议函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "memlib.h"
#include "morder.h"
#include "util/commlib.h"

/* DTM_NEW 协议包结构 (BE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  head[3];    /* 标志1A 72 73 */
/*003*/ byte_t  cmd_type1;  /* 命令应用大类 */
/*004*/ byte_t  cmd_type2;  /* 命令应用子类 */
/*005*/ byte_t  frame_num;  /* 帧的序号 (+1 可以回绕) */
/*006*/ int16u  additions;  /* 返回错误代码或附加参数 */
/*008*/ int16u  data_size;  /* 后续数据长度 (包括 CRC) */
/*010*/
} CR_PACKED sDTMN_HDR;

CR_TYPEDEF struct
{
#if 0
        byte_t  data[leng-2];   /* 数据 */
#endif
        int16u  crc16c_sum_be;  /* 校验 */

} CR_PACKED sDTMN_DAT;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
---------------------------------------
    DTM_NEW 读取包头
---------------------------------------
*/
static int16u
dtmn_get_head (
  __CR_IN__ const sDTMN_HDR*    head,
  __CR_OT__ byte_t*             cmd_type1,
  __CR_OT__ byte_t*             cmd_type2,
  __CR_OT__ byte_t*             frame_num,
  __CR_OT__ int16u*             additions
    )
{
    int16u  size;

    /* 结构性检查 */
    if (head->head[0] != 0x1A ||
        head->head[1] != 0x72 ||
        head->head[2] != 0x73)
        return (0);
    size = WORD_BE(head->data_size);
    if (size < sizeof(sDTMN_DAT))
        return (0);

    /* 返回附加值 */
    if (cmd_type1 != NULL)
        *cmd_type1 = head->cmd_type1;
    if (cmd_type2 != NULL)
        *cmd_type2 = head->cmd_type2;
    if (frame_num != NULL)
        *frame_num = head->frame_num;
    if (additions != NULL)
        *additions = WORD_BE(head->additions);
    return (size);
}

/*
---------------------------------------
    DTM_NEW 设置包头
---------------------------------------
*/
static void_t
dtmn_set_head (
  __CR_OT__ sDTMN_HDR*  head,
  __CR_IN__ byte_t      cmd_type1,
  __CR_IN__ byte_t      cmd_type2,
  __CR_IN__ byte_t      frame_num,
  __CR_IN__ int16u      additions,
  __CR_IN__ int16u      data_size
    )
{
    /* 填充头结构 */
    head->head[0] = 0x1A;
    head->head[1] = 0x72;
    head->head[2] = 0x73;
    head->cmd_type1 = cmd_type1;
    head->cmd_type2 = cmd_type2;
    head->frame_num = frame_num;
    head->additions = WORD_BE(additions);
    head->data_size = WORD_BE(data_size);
}

/*
---------------------------------------
    DTM_NEW 生成校验
---------------------------------------
*/
static void_t
dtmn_set_hash (
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
    DTM_NEW 检查校验
---------------------------------------
*/
static bool_t
dtmn_get_hash (
  __CR_IN__ const sDTMN_HDR*    head,
  __CR_IN__ const void_t*       data,
  __CR_IN__ int16u              size
    )
{
    int16u  hash;

    hash = hash_crc16c_init();
    hash = hash_crc16c_update(hash, head, sizeof(sDTMN_HDR));
    hash = hash_crc16c_update(hash, data, size - 2);
    hash = hash_crc16c_finish(hash);
    hash = WORD_BE(hash);
    if (mem_cmp(&hash, (byte_t*)data + size - 2, 2) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    DTM_NEW UART 发送
=======================================
*/
CR_API bool_t
dtmn_sio_send (
  __CR_IN__ uint_t          port,
  __CR_IN__ byte_t          cmd_type1,
  __CR_IN__ byte_t          cmd_type2,
  __CR_IN__ byte_t          frame_num,
  __CR_IN__ int16u          additions,
  __CR_IN__ const void_t*   data,
  __CR_IN__ int16u          size
    )
{
    uint_t      back;
    uint_t      pack;
    byte_t*     temp;
    sDTMN_HDR*  head;
    sDTMN_DAT*  body;

    /* 数据长度过滤 */
    if (size > 65535 - sizeof(sDTMN_DAT))
        return (FALSE);
    pack = size + sizeof(sDTMN_DAT);
    if (pack > 65535 - sizeof(sDTMN_HDR))
        return (FALSE);
    temp = (byte_t*)mem_malloc(pack + sizeof(sDTMN_HDR));
    if (temp == NULL)
        return (FALSE);

    /* 填充好所有数据 */
    head = (sDTMN_HDR*)(temp);
    body = (sDTMN_DAT*)(head + 1);
    if (data != NULL)
        mem_cpy(body, data, size);
    dtmn_set_head(head, cmd_type1, cmd_type2, frame_num,
                        additions, (int16u)pack);
    pack += sizeof(sDTMN_HDR);
    dtmn_set_hash(head, (int16u)pack);

    /* 发送协议包 */
    back = sio_write(port, temp, pack);
    mem_free(temp);
    return ((back != pack) ? FALSE : TRUE);
}

/*
=======================================
    DTM_NEW UART 读取
=======================================
*/
CR_API bool_t
dtmn_sio_recv (
  __CR_IN__ uint_t  port,
  __CR_OT__ void_t* data,
  __CR_IO__ uint_t* size,
  __CR_OT__ byte_t* cmd_type1,
  __CR_OT__ byte_t* cmd_type2,
  __CR_OT__ byte_t* frame_num,
  __CR_OT__ int16u* additions
    )
{
    uint_t      back;
    uint_t      pack;
    byte_t      tags;
    byte_t*     pntr;
    sDTMN_HDR   head;

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
    pack = dtmn_get_head(&head, cmd_type1, cmd_type2, frame_num, additions);
    if (pack == 0)
        return (FALSE);

    /* 过滤返回的数据长度 */
    if (size != NULL) {
        if (pack > *size)
            return (FALSE);
        *size = pack - sizeof(sDTMN_DAT);
    }

    /* 接收后续数据 */
    back = sio_read(port, data, pack);
    if (back != pack)
        return (FALSE);
    return (dtmn_get_hash(&head, data, (int16u)pack));
}

/*
=======================================
    DTM_NEW UART 交互
=======================================
*/
CR_API bool_t
dtmn_sio_iorw (
  __CR_IN__ uint_t          port,
  __CR_IN__ byte_t          cmd_type1,
  __CR_IN__ byte_t          cmd_type2,
  __CR_OT__ void_t*         obuff,
  __CR_IO__ uint_t*         osize,
  __CR_OT__ int16u*         retcode,
  __CR_IN__ byte_t          frame_num,
  __CR_IN__ int16u          additions,
  __CR_IN__ const void_t*   ibuff,
  __CR_IN__ int16u          isize,
  __CR_IN__ comm_delay_t    delay
    )
{
    byte_t  cmd1, cmd2, fnum;

    /* 发送主动帧 */
    sio_flush(port, CR_SIO_FLU_RT);
    if (!dtmn_sio_send(port, cmd_type1, cmd_type2, frame_num,
                       additions, ibuff, isize))
        return (FALSE);

    /* 处理的延时 */
    if (delay != NULL)
        delay();

    /* 接收响应帧 */
    if (!dtmn_sio_recv(port, obuff, osize, &cmd1, &cmd2, &fnum, retcode))
        return (FALSE);

    /* 过滤非法帧 */
    if (cmd1 != cmd_type1 || cmd2 != cmd_type2 || fnum != frame_num)
        return (FALSE);
    return (TRUE);
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
