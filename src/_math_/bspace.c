/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-06-04  */
/*     #######          ###    ###      [MATH]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 空间哈希函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"
#include "phylib.h"

/*
=======================================
    点的空间划分
=======================================
*/
CR_API uint_t
space_divide (
  __CR_OT__ byte_t* bits,
  __CR_IN__ double  min,
  __CR_IN__ double  max,
  __CR_IN__ double  value,
  __CR_IN__ uint_t  times
    )
{
    byte_t  temp;
    double  center;
    uint_t  idx, size;

    /* 非法的输入参数 */
    if (min >= max || value < min || value > max)
        return (0);

    /* 返回结果大小 */
    size = times / 8;
    if (times % 8 != 0)
        size++;
    if (bits == NULL)
        return (size);
    idx = 0;
    temp = 0;

    /* 以中点划分点的区域 */
    for (; times != 0; times--) {
        center = (min + max) / 2;
        if (value < center) {
            max = center;
        }
        else {
            min = center;
            temp |= (1 << (7 - idx));
        }
        if (++idx == 8) {
            idx = 0;
            *bits++ = temp;
            temp = 0;
        }
    }
    if (idx != 0)
        *bits = temp;
    return (size);
}

/*
=======================================
    空间位的合并
=======================================
*/
CR_API uint_t
space_genbit (
  __CR_OT__ byte_t*     bits,
  __CR_IN__ uint_t      size,
  __CR_IN__ byte_t**    list,
  __CR_IN__ uint_t      count
    )
{
    byte_t  temp;
    uint_t  ii, jj, kk, idx;
    uint_t  lsize = size * count;

    /* 非法的输入参数 */
    if (bits == NULL || list == NULL)
        return (lsize);
    idx = 0;
    temp = 0;

    /* 按位合并列表 */
    for (ii = 0; ii < size; ii++) {
        for (jj = 0; jj < 8; jj++) {
            for (kk = 0; kk < count; kk++) {
                if ((list[kk])[ii] & (1 << (7 - jj)))
                    temp |= (1 << (7 - idx));
                if (++idx == 8) {
                    idx = 0;
                    *bits++ = temp;
                    temp = 0;
                }
            }
        }
    }
    return (lsize);
}

/*
=======================================
    二维空间坐标哈希
=======================================
*/
CR_API ansi_t*
space_hash2d (
  __CR_IN__ double  min_x,
  __CR_IN__ double  max_x,
  __CR_IN__ double  min_y,
  __CR_IN__ double  max_y,
  __CR_IN__ double  x,
  __CR_IN__ double  y,
  __CR_IN__ uint_t  times
    )
{
    leng_t  leng;
    uint_t  size;
    ansi_t* rett;
    byte_t* xbit;
    byte_t* ybit;
    byte_t* bits;
    byte_t* list[2];

    /* 分配内存 */
    size = space_divide(NULL, min_x, max_x, x, times);
    if (size == 0)
        return (NULL);
    size = space_divide(NULL, min_y, max_y, y, times);
    if (size == 0)
        return (NULL);
    xbit = (byte_t*)mem_malloc(size);
    if (xbit == NULL)
        return (NULL);
    ybit = (byte_t*)mem_malloc(size);
    if (ybit == NULL) {
        mem_free(xbit);
        return (NULL);
    }

    /* 两个维度划分 */
    space_divide(xbit, min_x, max_x, x, times);
    space_divide(ybit, min_y, max_y, y, times);

    /* 分配内存 */
    bits = (byte_t*)mem_malloc(size * 2);
    if (bits == NULL) {
        mem_free(xbit);
        mem_free(ybit);
        return (NULL);
    }

    /* 合并两个维度的数据 */
    list[0] = xbit;
    list[1] = ybit;
    space_genbit(bits, size, list, 2);
    mem_free(xbit);
    mem_free(ybit);
    size = 2 * times;
    if (size % 8 != 0)
        size = size / 8 + 1;
    else
        size = size / 8;

    /* 转换成 BASE32 */
    leng = encode_base32(NULL, 0, bits, size, 0);
    rett = str_allocA(leng + 1);
    if (rett == NULL) {
        mem_free(bits);
        return (NULL);
    }
    leng = encode_base32(rett, leng, bits, size, 0);
    mem_free(bits);
    rett[leng] = 0x00;
    return (rett);
}

/*
=======================================
    三维空间坐标哈希
=======================================
*/
CR_API ansi_t*
space_hash3d (
  __CR_IN__ double  min_x,
  __CR_IN__ double  max_x,
  __CR_IN__ double  min_y,
  __CR_IN__ double  max_y,
  __CR_IN__ double  min_z,
  __CR_IN__ double  max_z,
  __CR_IN__ double  x,
  __CR_IN__ double  y,
  __CR_IN__ double  z,
  __CR_IN__ uint_t  times
    )
{
    leng_t  leng;
    uint_t  size;
    ansi_t* rett;
    byte_t* xbit;
    byte_t* ybit;
    byte_t* zbit;
    byte_t* bits;
    byte_t* list[3];

    /* 分配内存 */
    size = space_divide(NULL, min_x, max_x, x, times);
    if (size == 0)
        return (NULL);
    size = space_divide(NULL, min_y, max_y, y, times);
    if (size == 0)
        return (NULL);
    size = space_divide(NULL, min_z, max_z, z, times);
    if (size == 0)
        return (NULL);
    xbit = (byte_t*)mem_malloc(size);
    if (xbit == NULL)
        return (NULL);
    ybit = (byte_t*)mem_malloc(size);
    if (ybit == NULL) {
        mem_free(xbit);
        return (NULL);
    }
    zbit = (byte_t*)mem_malloc(size);
    if (zbit == NULL) {
        mem_free(xbit);
        mem_free(ybit);
        return (NULL);
    }

    /* 三个维度划分 */
    space_divide(xbit, min_x, max_x, x, times);
    space_divide(ybit, min_y, max_y, y, times);
    space_divide(zbit, min_z, max_z, z, times);

    /* 分配内存 */
    bits = (byte_t*)mem_malloc(size * 3);
    if (bits == NULL) {
        mem_free(xbit);
        mem_free(ybit);
        mem_free(zbit);
        return (NULL);
    }

    /* 合并两个维度的数据 */
    list[0] = xbit;
    list[1] = ybit;
    list[2] = zbit;
    space_genbit(bits, size, list, 3);
    mem_free(xbit);
    mem_free(ybit);
    mem_free(zbit);
    size = 3 * times;
    if (size % 8 != 0)
        size = size / 8 + 1;
    else
        size = size / 8;

    /* 转换成 BASE32 */
    leng = encode_base32(NULL, 0, bits, size, 0);
    rett = str_allocA(leng + 1);
    if (rett == NULL) {
        mem_free(bits);
        return (NULL);
    }
    leng = encode_base32(rett, leng, bits, size, 0);
    mem_free(bits);
    rett[leng] = 0x00;
    return (rett);
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
