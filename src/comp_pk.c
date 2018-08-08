/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2018-08-06  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack PKWARE 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"
#include "ex3rd/pkware/pkware.h"

/* 用户定义结构 */
typedef struct
{
        leng_t  i_ptr;
        leng_t  o_ptr;
        leng_t  ssize;
        leng_t  dsize;
        byte_t* i_buf;
        byte_t* o_buf;

} sPK_DAT;

/*
---------------------------------------
    读取数据回调
---------------------------------------
*/
static uint_t
pkware_read (
  __CR_OT__ ansi_t* buf,
  __CR_IN__ uint_t* size,
  __CR_IO__ void_t* param
    )
{
    leng_t      length;
    sPK_DAT*    dt = (sPK_DAT*)param;

    length = (leng_t)(*size);
    if (length > dt->ssize - dt->i_ptr)
        length = dt->ssize - dt->i_ptr;
    mem_cpy(buf, &dt->i_buf[dt->i_ptr], length);
    dt->i_ptr += length;
    return ((uint_t)length);
}

/*
---------------------------------------
    写入数据回调
---------------------------------------
*/
static void_t
pkware_write (
  __CR_OT__ ansi_t* buf,
  __CR_IN__ uint_t* size,
  __CR_IO__ void_t* param
    )
{
    leng_t      length;
    sPK_DAT*    dt = (sPK_DAT*)param;

    length = (leng_t)(*size);
    if (length > dt->dsize - dt->o_ptr)
        length = dt->dsize - dt->o_ptr;
    mem_cpy(&dt->o_buf[dt->o_ptr], buf, length);
    dt->o_ptr += length;
}

/*
=======================================
    PKWARE 压缩
=======================================
*/
CR_API leng_t
compr_pkware (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          dsize
    )
{
    uint_t  type;
    uint_t  result;
    ansi_t* wrkmem;
    sPK_DAT mem_io;

    if (dst == NULL)
        return (dstlen + dstlen / 2 + 64);

    wrkmem = str_allocA(CMP_BUFFER_SIZE);
    if (wrkmem == NULL)
        return (0);
    mem_zero(wrkmem, CMP_BUFFER_SIZE);

    type = (dsize & 0xE000) ? CMP_ASCII : CMP_BINARY;
    dsize &= 0x1FFF;
    if (dsize <= CMP_IMPLODE_DICT_SIZE1)
        dsize = CMP_IMPLODE_DICT_SIZE1;
    else
    if (dsize <= CMP_IMPLODE_DICT_SIZE2)
        dsize = CMP_IMPLODE_DICT_SIZE2;
    else
        dsize = CMP_IMPLODE_DICT_SIZE3;
    mem_io.i_ptr = 0;
    mem_io.o_ptr = 0;
    mem_io.ssize = srclen;
    mem_io.dsize = dstlen;
    mem_io.i_buf = (byte_t*)src;
    mem_io.o_buf = (byte_t*)dst;
    result = implode(pkware_read, pkware_write, wrkmem, &mem_io,
                    &type, &dsize);
    mem_free(wrkmem);
    if (result != CMP_NO_ERROR)
        return (0);
    return (mem_io.o_ptr);
}

/*
=======================================
    PKWARE 解压
=======================================
*/
CR_API leng_t
uncompr_pkware (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    uint_t  result;
    ansi_t* wrkmem;
    sPK_DAT mem_io;

    wrkmem = str_allocA(EXP_BUFFER_SIZE);
    if (wrkmem == NULL)
        return (0);
    mem_zero(wrkmem, EXP_BUFFER_SIZE);

    mem_io.i_ptr = 0;
    mem_io.o_ptr = 0;
    mem_io.ssize = srclen;
    mem_io.dsize = dstlen;
    mem_io.i_buf = (byte_t*)src;
    mem_io.o_buf = (byte_t*)dst;
    result = explode(pkware_read, pkware_write, wrkmem, &mem_io);
    mem_free(wrkmem);
    if (result != CMP_NO_ERROR)
        return (0);
    return (mem_io.o_ptr);
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
