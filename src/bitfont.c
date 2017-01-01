/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-17  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 点阵图形函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "gfx2.h"

/*
=======================================
    获取1位字模 (高位先画)
=======================================
*/
CR_API void_t*
font1_h2l (
  __CR_OT__ void_t*         mask,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          width
    )
{
    byte_t  bits, cnts;
    uint_t  jj = 0, rest;
    byte_t* dst = (uchar*)mask;
    byte_t* src = (uchar*)data;

    rest  = width % 8;
    width = width / 8;
    for (; width != 0; width--, jj += 8)
    {
        bits = *src++;
        dst[jj + 0] = (bits >> 7) & 1;
        dst[jj + 1] = (bits >> 6) & 1;
        dst[jj + 2] = (bits >> 5) & 1;
        dst[jj + 3] = (bits >> 4) & 1;
        dst[jj + 4] = (bits >> 3) & 1;
        dst[jj + 5] = (bits >> 2) & 1;
        dst[jj + 6] = (bits >> 1) & 1;
        dst[jj + 7] = (bits >> 0) & 1;
    }
    /* 处理尾部 */
    if (rest != 0) {
        bits = *src++;
        for (cnts = 7; rest != 0; rest--, cnts--, jj++)
            dst[jj] = (bits >> cnts) & 1;
    }
    return ((void_t*)src);
}

/*
=======================================
    获取1位字模 (低位先画)
=======================================
*/
CR_API void_t*
font1_l2h (
  __CR_OT__ void_t*         mask,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          width
    )
{
    byte_t  bits;
    uint_t  jj = 0, rest;
    byte_t* dst = (uchar*)mask;
    byte_t* src = (uchar*)data;

    rest  = width % 8;
    width = width / 8;
    for (; width != 0; width--, jj += 8)
    {
        bits = *src++;
        dst[jj + 0] = bits & 1; bits >>= 1;
        dst[jj + 1] = bits & 1; bits >>= 1;
        dst[jj + 2] = bits & 1; bits >>= 1;
        dst[jj + 3] = bits & 1; bits >>= 1;
        dst[jj + 4] = bits & 1; bits >>= 1;
        dst[jj + 5] = bits & 1; bits >>= 1;
        dst[jj + 6] = bits & 1; bits >>= 1;
        dst[jj + 7] = bits & 1;
    }
    /* 处理尾部 */
    if (rest != 0) {
        bits = *src++;
        for (; rest != 0; rest--, jj++) {
            dst[jj] = bits & 1;
            bits >>= 1;
        }
    }
    return ((void_t*)src);
}

/*
=======================================
    获取2位字模 (高位先画)
=======================================
*/
CR_API void_t*
font2_h2l (
  __CR_OT__ void_t*         mask,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          width
    )
{
    byte_t  bits, cnts;
    uint_t  jj = 0, rest;
    byte_t* dst = (uchar*)mask;
    byte_t* src = (uchar*)data;

    rest  = width % 4;
    width = width / 4;
    for (; width != 0; width--, jj += 4)
    {
        bits = *src++;
        dst[jj + 0] = (bits >> 6) & 3;
        dst[jj + 1] = (bits >> 4) & 3;
        dst[jj + 2] = (bits >> 2) & 3;
        dst[jj + 3] = (bits >> 0) & 3;
    }
    /* 处理尾部 */
    if (rest != 0) {
        bits = *src++;
        for (cnts = 6; rest != 0; rest--, cnts -= 2, jj++)
            dst[jj] = (bits >> cnts) & 3;
    }
    return ((void_t*)src);
}

/*
=======================================
    获取2位字模 (低位先画)
=======================================
*/
CR_API void_t*
font2_l2h (
  __CR_OT__ void_t*         mask,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          width
    )
{
    byte_t  bits;
    uint_t  jj = 0, rest;
    byte_t* dst = (uchar*)mask;
    byte_t* src = (uchar*)data;

    rest  = width % 4;
    width = width / 4;
    for (; width != 0; width--, jj += 4)
    {
        bits = *src++;
        dst[jj + 0] = bits & 3; bits >>= 2;
        dst[jj + 1] = bits & 3; bits >>= 2;
        dst[jj + 2] = bits & 3; bits >>= 2;
        dst[jj + 3] = bits & 3;
    }
    /* 处理尾部 */
    if (rest != 0) {
        bits = *src++;
        for (; rest != 0; rest--, jj++) {
            dst[jj] = bits & 3;
            bits >>= 2;
        }
    }
    return ((void_t*)src);
}

/*
=======================================
    获取4位字模 (高位先画)
=======================================
*/
CR_API void_t*
font4_h2l (
  __CR_OT__ void_t*         mask,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          width
    )
{
    byte_t  bits;
    uint_t  jj = 0, rest;
    byte_t* dst = (uchar*)mask;
    byte_t* src = (uchar*)data;

    rest  = width % 2;
    width = width / 2;
    for (; width != 0; width--, jj += 2)
    {
        bits = *src++;
        dst[jj + 0] = bits >> 4;
        dst[jj + 1] = bits & 15;
    }
    /* 处理尾部 */
    if (rest != 0) {
        bits = *src++;
        dst[jj] = bits >> 4;
    }
    return ((void_t*)src);
}

/*
=======================================
    获取4位字模 (低位先画)
=======================================
*/
CR_API void_t*
font4_l2h (
  __CR_OT__ void_t*         mask,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          width
    )
{
    byte_t  bits;
    uint_t  jj = 0, rest;
    byte_t* dst = (uchar*)mask;
    byte_t* src = (uchar*)data;

    rest  = width % 2;
    width = width / 2;
    for (; width != 0; width--, jj += 2)
    {
        bits = *src++;
        dst[jj + 0] = bits & 15;
        dst[jj + 1] = bits >> 4;
    }
    /* 处理尾部 */
    if (rest != 0) {
        bits = *src++;
        dst[jj] = bits & 15;
    }
    return ((void_t*)src);
}

/*
=======================================
    获取N位字模 (高位先画)
=======================================
*/
CR_API void_t*
fontX_h2l (
  __CR_OT__ void_t*         mask,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          width,
  __CR_IN__ byte_t          hpart,
  __CR_IN__ byte_t          lpart
    )
{
    byte_t  bits;
    uint_t  jj = 0, rest;
    byte_t* dst = (uchar*)mask;
    byte_t* src = (uchar*)data;

    rest  = width % 2;
    width = width / 2;
    for (; width != 0; width--, jj += 2)
    {
        bits = *src++;
        dst[jj + 0] = bits >> hpart;
        dst[jj + 1] = bits  & lpart;
    }
    /* 处理尾部 */
    if (rest != 0) {
        bits = *src++;
        dst[jj] = bits >> hpart;
    }
    return ((void_t*)src);
}

/*
=======================================
    获取N位字模 (低位先画)
=======================================
*/
CR_API void_t*
fontX_l2h (
  __CR_OT__ void_t*         mask,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          width,
  __CR_IN__ byte_t          hpart,
  __CR_IN__ byte_t          lpart
    )
{
    byte_t  bits;
    uint_t  jj = 0, rest;
    byte_t* dst = (uchar*)mask;
    byte_t* src = (uchar*)data;

    rest  = width % 2;
    width = width / 2;
    for (; width != 0; width--, jj += 2)
    {
        bits = *src++;
        dst[jj + 0] = bits  & lpart;
        dst[jj + 1] = bits >> hpart;
    }
    /* 处理尾部 */
    if (rest != 0) {
        bits = *src++;
        dst[jj] = bits & lpart;
    }
    return ((void_t*)src);
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
