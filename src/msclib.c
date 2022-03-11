/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 杂项操作函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "msclib.h"

#ifndef _CR_NO_STDC_
    #include <stdlib.h>
#else
    #include "templ/qsort.inl"
#endif

/* 伪随机数的种子变量 */
static int32u   s_rand_seed = 0UL;

/*
=======================================
    设置伪随机数种子
=======================================
*/
CR_API void_t
rand_seed (
  __CR_IN__ uint_t  seed
    )
{
    s_rand_seed = seed;
}

/*
=======================================
    返回伪随机数
=======================================
*/
CR_API sint_t
rand_get (void_t)
{
    int32u  temp;

    /* 有冲突会更有趣 */
    temp = s_rand_seed;
    temp *= 214013UL;
    temp += 2531011UL;
    s_rand_seed = temp;
    temp >>= 16;
    temp &= 0x7FFF;
    return ((sint_t)temp);
}

/*
=======================================
    返回伪随机数 (有范围)
=======================================
*/
CR_API sint_t
rand_getx (
  __CR_IN__ sint_t  mod
    )
{
    if (mod == 0)
        mod++;
    return (rand_get() % mod);
}

/*
=======================================
    计算伪随机数 (坐标映射到随机空间)
=======================================
*/
CR_API fp32_t
random3d (
  __CR_IN__ uint_t  x,
  __CR_IN__ uint_t  y,
  __CR_IN__ uint_t  z,
  __CR_IN__ uint_t  p0,     /* 质数 */
  __CR_IN__ uint_t  p1,     /* 质数 */
  __CR_IN__ uint_t  p2,     /* 质数 */
  __CR_IN__ uint_t  p3      /* 质数 */
    )
{

    uint_t  nn = x + p0 * y + p1 * z;

    nn = (nn << 13) ^ nn;

    return ((fp32_t)(nn * (nn * p2) + p3) / 4294967296.0f);
}

/*
=======================================
    快速排序
=======================================
*/
CR_API void_t
quick_sort (
  __CR_IO__ void_t* base,
  __CR_IN__ leng_t  num,
  __CR_IN__ leng_t  width,
  __CR_IN__ sint_t  (*comp)(const void_t*, const void_t*)
    )
{
#ifndef _CR_NO_STDC_
    qsort(base, num, width, comp);
#else
    qsort2(base, num, width, comp);
#endif
}

/*
=======================================
    32位整数开方
=======================================
*/
CR_API int16u
sqrt_int32 (
  __CR_IN__ int32u  val
    )
{
    int32u  idx, divisor;
    int32u  rem = 0, root = 0;

    for (idx = 16; idx != 0; idx--)
    {
        root <<= 1;
        rem = (rem << 2) + (val >> 30);
        val  <<= 2;
        divisor = (root << 1) + 1;

        if (divisor <= rem)
        {
            root++;
            rem -= divisor;
        }
    }
    return ((int16u)root);
}

/*
=======================================
    计算最大公约数模板
=======================================
*/
#define tmpl_numb_gcd(_func, _type) \
CR_API _type \
_func ( \
  __CR_IN__ _type   a, \
  __CR_IN__ _type   b \
    ) \
{ \
    _type   rr; \
\
    if (a < b) { \
        CR_SWAP(a, b, rr); \
    } \
    while ((rr = a % b) != 0) { \
        a = b; \
        b = rr; \
    } \
    return (b); \
}

/*
=======================================
    计算最小公约数模板
=======================================
*/
#define tmpl_numb_lcm(_func, _type, _gcd) \
CR_API _type \
_func ( \
  __CR_IN__ _type   a, \
  __CR_IN__ _type   b \
    ) \
{ \
    if (a && b) \
        return ((a * b) / _gcd(a, b)); \
    else if (b) \
        return (b); \
    return (a); \
}

/*
=======================================
    最大最小公约数函数实例化
=======================================
*/
tmpl_numb_gcd(numb_gcd  , uint_t)
tmpl_numb_lcm(numb_lcm  , uint_t, numb_gcd  )
tmpl_numb_gcd(numb_gcd32, int32u)
tmpl_numb_lcm(numb_lcm32, int32u, numb_gcd32)
tmpl_numb_gcd(numb_gcd64, int64u)
tmpl_numb_lcm(numb_lcm64, int64u, numb_gcd64)

/*
=======================================
    BCD 数转二进制数
=======================================
*/
CR_API byte_t
bcd2bin (
  __CR_IN__ byte_t  bcd
    )
{
    return ((bcd & 0x0F) + (bcd >> 4) * 10);
}

/*
=======================================
    二进制数转 BCD 数
=======================================
*/
CR_API byte_t
bin2bcd (
  __CR_IN__ byte_t  bin
    )
{
    return (((bin / 10) << 4) + bin % 10);
}

/*
=======================================
    BCD 数转二进制数 (检查)
=======================================
*/
CR_API bool_t
bcd2bin_okay (
  __CR_IN__ byte_t  bcd
    )
{
    if ((bcd & 15) > 9)
        return (FALSE);
    if ((bcd >> 4) > 9)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    二进制数转 BCD 数 (检查)
=======================================
*/
CR_API bool_t
bin2bcd_okay (
  __CR_IN__ byte_t  bin
    )
{
    return ((bin > 99) ? FALSE : TRUE);
}

/*
=======================================
    填充1到最高位08
=======================================
*/
CR_API byte_t
fill2top08 (
  __CR_IN__ byte_t  val
    )
{
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    return (val);
}

/*
=======================================
    填充1到最高位16
=======================================
*/
CR_API int16u
fill2top16 (
  __CR_IN__ int16u  val
    )
{
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    return (val);
}

/*
=======================================
    填充1到最高位32
=======================================
*/
CR_API int32u
fill2top32 (
  __CR_IN__ int32u  val
    )
{
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    return (val);
}

/*
=======================================
    填充1到最高位64
=======================================
*/
CR_API int64u
fill2top64 (
  __CR_IN__ int64u  val
    )
{
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    val |= val >> 32;
    return (val);
}

/*
=======================================
    统计字节1的个数
=======================================
*/
CR_API uint_t
count_bits08 (
  __CR_IN__ byte_t  val
    )
{
    uint_t  res = val - ((val >> 1) & 0x55);

    res = (res & 0x33) + ((res >> 2) & 0x33);
    return ((uint_t)((res + (res >> 4)) & 0x0F));
}

/*
=======================================
    统计单字1的个数
=======================================
*/
CR_API uint_t
count_bits16 (
  __CR_IN__ int16u  val
    )
{
    uint_t  res = val - ((val >> 1) & 0x5555);

    res = (res & 0x3333) + ((res >> 2) & 0x3333);
    res = (res + (res >> 4)) & 0x0F0F;
    return ((uint_t)((res + (res >> 8)) & 0x00FF));
}

/*
=======================================
    统计双字1的个数
=======================================
*/
CR_API uint_t
count_bits32 (
  __CR_IN__ int32u  val
    )
{
    int32u  res = val - ((val >> 1) & 0x55555555UL);

    res = (res & 0x33333333UL) + ((res >> 2) & 0x33333333UL);
    res = (res + (res >> 4)) & 0x0F0F0F0FUL;
    res = (res + (res >> 8));
    return ((uint_t)((res + (res >> 16)) & 0x000000FFUL));
}

/*
=======================================
    统计四字1的个数
=======================================
*/
CR_API uint_t
count_bits64 (
  __CR_IN__ int64u  val
    )
{
#ifndef _CR_SYS64_
    #ifndef _CR_NO_INT64_
    return (count_bits32((int32u)(val >> 32)) +
            count_bits32((int32u)(val <<  0)));
    #else
    return (count_bits32((int32u)(val.hi32)) +
            count_bits32((int32u)(val.lo32)));
    #endif
#else
    int64u  res = val - ((val >> 1) & CR_ULL(0x5555555555555555));

    res = ((res >> 0) & CR_ULL(0x3333333333333333)) +
          ((res >> 2) & CR_ULL(0x3333333333333333));
    res = (res + (res >>  4)) & CR_ULL(0x0F0F0F0F0F0F0F0F);
    res = (res + (res >>  8));
    res = (res + (res >> 16));
    return ((uint_t)((res + (res >> 32)) & CR_ULL(0x00000000000000FF)));
#endif
}

/*
=======================================
    字节数组颠倒
=======================================
*/
CR_API void_t
bytes_swap (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size,
  __CR_IN__ leng_t  block
    )
{
    leng_t  blk = size / block;
    leng_t  idx, half = block / 2;
    byte_t  tmp, *ptr = (byte_t*)data;

    for (; blk != 0; blk--) {
        for (idx = 0; idx < half; idx++) {
            CR_SWAP(ptr[idx], ptr[block - 1 - idx], tmp);
        }
        ptr += block;
    }
}

/*
=======================================
    字节数组填充
=======================================
*/
CR_API void_t
bytes_fill (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size,
  __CR_IN__ leng_t  start,
  __CR_IN__ byte_t  (*fill)(leng_t, leng_t)
    )
{
    leng_t  ii, kk;
    byte_t* ptr = (byte_t*)data;

    if (fill == NULL) {
        for (ii = start; ii < size; ii++)
            ptr[ii] = 0x00;
    }
    else {
        for (kk = 0, ii = start; ii < size; ii++, kk++)
            ptr[ii] = fill(ii, kk);
    }
}

/*
=======================================
    位串颠倒 (<= 8)
=======================================
*/
CR_API byte_t
bits_swap08 (
  __CR_IN__ byte_t  val,
  __CR_IN__ ufast_t bits
    )
{
    byte_t  tmp = 0;
    ufast_t back = bits;

    for (; bits != 0; bits--) {
        tmp <<= 1;
        tmp |= (val & 1);
        val >>= 1;
    }
    return ((byte_t)(tmp | (val << back)));
}

/*
=======================================
    位串颠倒 (<= 16)
=======================================
*/
CR_API int16u
bits_swap16 (
  __CR_IN__ int16u  val,
  __CR_IN__ ufast_t bits
    )
{
    int16u  tmp = 0;
    ufast_t back = bits;

    for (; bits != 0; bits--) {
        tmp <<= 1;
        tmp |= (val & 1);
        val >>= 1;
    }
    return ((int16u)(tmp | (val << back)));
}

/*
=======================================
    位串颠倒 (<= 32)
=======================================
*/
CR_API int32u
bits_swap32 (
  __CR_IN__ int32u  val,
  __CR_IN__ ufast_t bits
    )
{
    int32u  tmp = 0;
    ufast_t back = bits;

    for (; bits != 0; bits--) {
        tmp <<= 1;
        tmp |= (val & 1);
        val >>= 1;
    }
    return ((int32u)(tmp | (val << back)));
}

/*
=======================================
    位串颠倒 (<= 64)
=======================================
*/
CR_API int64u
bits_swap64 (
  __CR_IN__ int64u  val,
  __CR_IN__ ufast_t bits
    )
{
    int64u  tmp = 0;
    ufast_t back = bits;

    for (; bits != 0; bits--) {
        tmp <<= 1;
        tmp |= (val & 1);
        val >>= 1;
    }
    return ((int64u)(tmp | (val << back)));
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
