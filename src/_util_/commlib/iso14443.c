/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-04-20  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> ISO14443 通讯协议函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "util/commlib.h"

/*
---------------------------------------
    ISO14443 UART 读取一帧数据
---------------------------------------
*/
static uint_t
iso14443_sio_read (
  __CR_IN__ uint_t  port,
  __CR_OT__ byte_t* data,
  __CR_IN__ uint_t  size
    )
{
    uint_t  back;

    do {
        back = sio_read(port, data, 1);
        if (back != 1)
            return (0);
    } while (data[0] != 0x10);

    size -= 1;
    back = sio_read(port, &data[1], size);
    return (back + 1);
}

/*
=======================================
    ISO14443 UART 获取读卡机版本
=======================================
*/
CR_API ansi_t*
iso14443_sio_get_ver (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr
    )
{
    ansi_t  *str;
    byte_t  buf[3];
    uint_t  len, back;

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'V';
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 3);
    if (back != 3)
        return (NULL);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 3);
    if (back != 3)
        return (NULL);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr)
        return (NULL);
    back = (uint_t)buf[2];
    str = (ansi_t*)mem_malloc(back + 1);
    if (str == NULL)
        return (NULL);
    len = sio_read(port, str, back);
    if (len != back) {
        mem_free(str);
        return (NULL);
    }
    str[back] = CR_AC(NIL);
    return (str);
}

/*
=======================================
    ISO14443 UART 设置读卡机参数
=======================================
*/
CR_API bool_t
iso14443_sio_setup (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr,
  __CR_IN__ byte_t  param
    )
{
    uint_t  back;
    byte_t  buf[7];

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'B';
    buf[3] = param;
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 4);
    if (back != 4)
        return (FALSE);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 7);
    if (back != 7)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr ||
        mem_cmp(&buf[2], "\x04\x44\x6F\x6E\x65", 5) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    ISO14443 UART 测试读卡机在线
=======================================
*/
CR_API bool_t
iso14443_sio_online (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr
    )
{
    uint_t  back;
    byte_t  buf[6];

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'A';
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 3);
    if (back != 3)
        return (FALSE);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 6);
    if (back != 6)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr ||
        mem_cmp(&buf[2], "\x03\x4F\x4B\x21", 4) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    ISO14443 UART 设置读卡机地址
=======================================
*/
CR_API bool_t
iso14443_sio_set_addr (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr,
  __CR_IN__ byte_t  naddr
    )
{
    uint_t  back;
    byte_t  buf[7];

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'a';
    buf[3] = naddr;
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 4);
    if (back != 4)
        return (FALSE);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 7);
    if (back != 7)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr ||
        mem_cmp(&buf[2], "\x04\x44\x6F\x6E\x65", 5) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    ISO14443 UART 读取一个块数据
=======================================
*/
CR_API bool_t
iso14443_sio_rd_blk (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr,
  __CR_IN__ byte_t  keyab,
  __CR_IN__ byte_t  nkey,
  __CR_IN__ byte_t  nblock,
  __CR_OT__ byte_t  data[16]
    )
{
    uint_t  back;
    byte_t  buf[5];

    /* 过滤参数 */
    if (nkey > 23 || nblock > 191)
        return (FALSE);

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'R';
    buf[3] = nkey | (keyab & 0x80);
    buf[4] = nblock;
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 5);
    if (back != 5)
        return (FALSE);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 3);
    if (back != 3)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr || buf[2] != 16)
        return (FALSE);
    back = sio_read(port, data, 16);
    return ((back != 16) ? FALSE : TRUE);
}

/*
=======================================
    ISO14443 UART 写入一个数据块
=======================================
*/
CR_API bool_t
iso14443_sio_wr_blk (
  __CR_IN__ uint_t          port,
  __CR_IN__ byte_t          addr,
  __CR_IN__ byte_t          keyab,
  __CR_IN__ byte_t          nkey,
  __CR_IN__ byte_t          nblock,
  __CR_IN__ const byte_t    data[16]
    )
{
    uint_t  back;
    byte_t  buf[21];

    /* 过滤参数 */
    if (nkey > 23 || nblock > 191)
        return (FALSE);

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'W';
    buf[3] = nkey | (keyab & 0x80);
    buf[4] = nblock;
    mem_cpy(&buf[5], data, 16);
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 21);
    if (back != 21)
        return (FALSE);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 7);
    if (back != 7)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr ||
        mem_cmp(&buf[2], "\x04\x44\x6F\x6E\x65", 5) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    ISO14443 UART 停止一张卡片
=======================================
*/
CR_API bool_t
iso14443_sio_stop (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr
    )
{
    uint_t  back;
    byte_t  buf[7];

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'H';
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 3);
    if (back != 3)
        return (FALSE);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 7);
    if (back != 7)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr ||
        mem_cmp(&buf[2], "\x04\x44\x6F\x6E\x65", 5) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    ISO14443 UART 读取卡的唯一编号
=======================================
*/
CR_API bool_t
iso14443_sio_get_uid (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr,
  __CR_IN__ byte_t  uid[5]
    )
{
    uint_t  back;
    byte_t  buf[8];

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'U';
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 3);
    if (back != 3)
        return (FALSE);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 3);
    if (back != 3)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr || buf[2] != 5)
        return (FALSE);
    back = sio_read(port, uid, 5);
    return ((back != 5) ? FALSE : TRUE);
}

/*
=======================================
    ISO14443 UART 设置机内保存的密钥
=======================================
*/
CR_API bool_t
iso14443_sio_set_eep (
  __CR_IN__ uint_t          port,
  __CR_IN__ byte_t          addr,
  __CR_IN__ byte_t          nkey,
  __CR_IN__ const byte_t    data[6]
    )
{
    uint_t  back;
    byte_t  buf[10];

    /* 过滤参数 */
    if (nkey > 23)
        return (FALSE);

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'E';
    buf[3] = nkey;
    mem_cpy(&buf[4], data, 6);
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 10);
    if (back != 10)
        return (FALSE);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 7);
    if (back != 7)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr ||
        mem_cmp(&buf[2], "\x04\x44\x6F\x6E\x65", 5) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    ISO14443 UART 设置卡片的密钥
=======================================
*/
CR_API bool_t
iso14443_sio_set_key (
  __CR_IN__ uint_t          port,
  __CR_IN__ byte_t          addr,
  __CR_IN__ byte_t          keyab,
  __CR_IN__ byte_t          nkey,
  __CR_IN__ byte_t          nblock,
  __CR_IN__ const byte_t    data[16]
    )
{
    uint_t  back;
    byte_t  buf[21];

    /* 过滤参数 */
    if (nkey > 23 || nblock > 191)
        return (FALSE);

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'K';
    buf[3] = nkey | (keyab & 0x80);
    buf[4] = nblock;
    mem_cpy(&buf[5], data, 16);
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 21);
    if (back != 21)
        return (FALSE);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 7);
    if (back != 7)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr ||
        mem_cmp(&buf[2], "\x04\x44\x6F\x6E\x65", 5) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    ISO14443 UART 钱包减值功能
=======================================
*/
CR_API bool_t
iso14443_sio_vsub (
  __CR_IN__ uint_t          port,
  __CR_IN__ byte_t          addr,
  __CR_IN__ byte_t          keyab,
  __CR_IN__ byte_t          nkey,
  __CR_IN__ byte_t          nblock,
  __CR_IN__ const byte_t    data[4]
    )
{
    uint_t  back;
    byte_t  buf[9];

    /* 过滤参数 */
    if (nkey > 23 || nblock > 191)
        return (FALSE);

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'd';
    buf[3] = nkey | (keyab & 0x80);
    buf[4] = nblock;
    mem_cpy(&buf[5], data, 4);
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 9);
    if (back != 9)
        return (FALSE);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 7);
    if (back != 7)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr ||
        mem_cmp(&buf[2], "\x04\x44\x6F\x6E\x65", 5) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    ISO14443 UART 钱包加值功能
=======================================
*/
CR_API bool_t
iso14443_sio_vadd (
  __CR_IN__ uint_t          port,
  __CR_IN__ byte_t          addr,
  __CR_IN__ byte_t          keyab,
  __CR_IN__ byte_t          nkey,
  __CR_IN__ byte_t          nblock,
  __CR_IN__ const byte_t    data[4]
    )
{
    uint_t  back;
    byte_t  buf[9];

    /* 过滤参数 */
    if (nkey > 23 || nblock > 191)
        return (FALSE);

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'i';
    buf[3] = nkey | (keyab & 0x80);
    buf[4] = nblock;
    mem_cpy(&buf[5], data, 4);
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 9);
    if (back != 9)
        return (FALSE);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 7);
    if (back != 7)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr ||
        mem_cmp(&buf[2], "\x04\x44\x6F\x6E\x65", 5) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    ISO14443 UART 连续读卡功能
=======================================
*/
CR_API bool_t
iso14443_sio_rd_blks (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr,
  __CR_IN__ byte_t  start,
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  size
    )
{
    uint_t  back;
    byte_t  buf[5];

    /* 过滤参数 */
    if (size == 0)
        return (TRUE);
    if (size % 16 != 0 || size / 16 > 54 ||
        start > 191 || size / 16 > (uint_t)(191 - start))
        return (FALSE);

    /* 发送命令 */
    buf[0] = 0x10;
    buf[1] = addr;
    buf[2] = 'r';
    buf[3] = start;
    buf[4] = (byte_t)(size / 16);
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 5);
    if (back != 5)
        return (FALSE);

    /* 等待回包 */
    back = iso14443_sio_read(port, buf, 3);
    if (back != 3)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0x10 || buf[1] != addr || buf[2] != buf[4])
        return (FALSE);
    back = sio_read(port, data, size);
    return ((back != size) ? FALSE : TRUE);
}

/*
=======================================
    ISO14443 UART 连续写卡功能
=======================================
*/
CR_API bool_t
iso14443_sio_wr_blks (
  __CR_IN__ uint_t          port,
  __CR_IN__ byte_t          addr,
  __CR_IN__ byte_t          start,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    uint_t  back;
    byte_t* buff;

    /* 过滤参数 */
    if (size == 0)
        return (TRUE);
    if (size % 16 != 0 || size / 16 > 54 ||
        start > 191 || size / 16 > (uint_t)(191 - start))
        return (FALSE);

    /* 分配内存 */
    buff = (byte_t*)mem_malloc(5 + size);
    if (buff == NULL)
        return (FALSE);

    /* 发送命令 */
    buff[0] = 0x10;
    buff[1] = addr;
    buff[2] = 'w';
    buff[3] = start;
    buff[4] = (byte_t)(size / 16);
    mem_cpy(&buff[5], data, size);
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buff, 5 + size);
    if (back != 5 + size)
        goto _failure;

    /* 等待回包 */
    back = iso14443_sio_read(port, buff, 7);
    if (back != 7)
        goto _failure;

    /* 数据校验 */
    if (buff[0] != 0x10 || buff[1] != addr ||
        mem_cmp(&buff[2], "\x04\x44\x6F\x6E\x65", 5) != 0)
        goto _failure;
    mem_free(buff);
    return (TRUE);

_failure:
    mem_free(buff);
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
