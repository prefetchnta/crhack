/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-10-01  */
/*     #######          ###    ###      [HARD]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>> LLB3 MLR 流量结算仪储存器读写函数库 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "device.h"

/*
=======================================
    读取储存器 (带校验)
=======================================
*/
CR_API retc_t
store_read (
  __CR_IN__ int16u  addr,
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    byte_t  crc8;

    x5045_wrdi();
    if (size <= 1 ||
        x5045_read(addr, data, size) != size)
        return (FALSE);
    size -= 1;
    crc8 = hash_crc8d_total(data, size);
    crc8 = ~crc8;
    if (crc8 != ((byte_t*)data)[size])
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    写入储存器 (带校验)
=======================================
*/
CR_API retc_t
store_write (
  __CR_IN__ int16u  addr,
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    byte_t  crc8;
    leng_t  tail;

    x5045_wrdi();
    if (size <= 1)
        return (FALSE);
    tail = size - 1;
    crc8 = hash_crc8d_total(data, tail);
    ((byte_t*)data)[tail] = ~crc8;

    /* 写入数据 */
    x5045_wren();
    if (x5045_write(addr, data, size, -1) != size) {
        x5045_wrdi();
        return (FALSE);
    }
    x5045_wrdi();

    /* 回读数据 */
    for (; size != 0; size--)
    {
        x5045_read(addr, &crc8, 1);
        if (crc8 != *(byte_t*)data)
            return (FALSE);
        data = (byte_t*)data + 1;
        addr += 1;
    }
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
