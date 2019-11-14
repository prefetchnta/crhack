/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2019-11-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack SPLAY 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"

/* SPLAY 常数 */
#define SPLAY_ROOT       0      /* 索引的根值 */
#define SPLAY_MAXCHAR   256     /* 最大的字符 */
#define SPLAY_EOFCHAR   256     /* 压缩结束标志 */
#define SPLAY_PREDMAX   255     /* SPLAY_MAXCHAR - 1 */
#define SPLAY_TWICEMAX  512     /* 2 * SPLAY_MAXCHAR */

/* 解码位的掩码 */
static const byte_t _rom_ s_splay_mask[8] =
{
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

/* SPLAY 类型映射 */
typedef int16u      CodeType;
typedef byte_t      UpIndex;
typedef int16u      DownIndex;
typedef UpIndex     TreeUpArray[SPLAY_TWICEMAX + 1];
typedef DownIndex   TreeDownArray[SPLAY_PREDMAX + 1];

/* SPLAY 内部结构 */
typedef struct
{
        /* SPLAY 部分 */
        TreeUpArray     Up;
        TreeDownArray   Left;
        TreeDownArray   Right;
        byte_t          BitPos;
        CodeType        TmpByte;

        /* 内存访问部分 */
        uchar*  buff;
        leng_t  pntr, size;

} sSPLAY;

/*
---------------------------------------
    SPLAY 树初始化
---------------------------------------
*/
static void_t
InitializeSplay (
  __CR_OT__ sSPLAY* handle
    )
{
    uint_t      J;
    DownIndex   I, K;

    for (I = 1; I <= SPLAY_TWICEMAX; I++)
        handle->Up[I] = (UpIndex)((I - 1) >> 1);
    for (J = 0; J <= SPLAY_PREDMAX; J++) {
        K = ((byte_t)J + 1) << 1;
        handle->Left[J] = K - 1;
        handle->Right[J] = K;
    }
}

/*
---------------------------------------
    SPLAY 树重排
---------------------------------------
*/
static void_t
Splay (
  __CR_IO__ sSPLAY*     handle,
  __CR_IN__ CodeType    Plain
    )
{
    UpIndex     C, D;
    DownIndex   A, B;

    A = Plain + SPLAY_MAXCHAR;

    do {
        C = handle->Up[A];
        if (C != SPLAY_ROOT) {
            D = handle->Up[C];
            B = handle->Left[D];
            if (C == B) {
                B = handle->Right[D];
                handle->Right[D] = A;
            }
            else {
                handle->Left[D] = A;
            }

            if (A == handle->Left[C])
                handle->Left[C] = B;
            else
                handle->Right[C] = B;
            handle->Up[A] = D;
            handle->Up[B] = C;
            A = D;
        }
        else {
            A = C;
        }
    } while (A != SPLAY_ROOT);
}

/*
---------------------------------------
    SPLAY 写入字节
---------------------------------------
*/
static void_t
WriteByte (
  __CR_IO__ sSPLAY* handle
    )
{
    if (handle->pntr < handle->size)
        handle->buff[handle->pntr] = (byte_t)handle->TmpByte;
    handle->pntr++;
}

/*
---------------------------------------
    SPLAY 压缩一个字符
---------------------------------------
*/
static void_t
Compress (
  __CR_IO__ sSPLAY*     handle,
  __CR_IN__ CodeType    Plain
    )
{
    UpIndex     U;
    DownIndex   A;
    /* -------- */
    int16u  Sp = 0;
    byte_t  Stack[SPLAY_PREDMAX + 1];

    A = Plain + SPLAY_MAXCHAR;

    do {
        U = handle->Up[A];
        Stack[Sp++] = (byte_t)(handle->Right[U] == A);
        A = U;
    } while (A != SPLAY_ROOT);

    do {
        if (Stack[--Sp])
            handle->TmpByte |= s_splay_mask[handle->BitPos];
        if (handle->BitPos == 7) {
            WriteByte(handle);
            handle->BitPos = 0;
            handle->TmpByte = 0;
        }
        else {
            handle->BitPos++;
        }
    } while (Sp != 0);

    Splay(handle, Plain);
}

/*
---------------------------------------
    SPLAY 读取字节
---------------------------------------
*/
static void_t
ReadByte (
  __CR_IO__ sSPLAY* handle
    )
{
    if (handle->pntr < handle->size)
        handle->TmpByte = handle->buff[handle->pntr];
    else
        handle->TmpByte = 0;
    handle->pntr++;
}

/*
---------------------------------------
    SPLAY 解压一个字符
---------------------------------------
*/
static CodeType
Expand (
  __CR_IO__ sSPLAY* handle
    )
{
    DownIndex   A = SPLAY_ROOT;

    do {
        if (handle->BitPos == 7) {
            ReadByte(handle);
            handle->BitPos = 0;
        }
        else {
            handle->BitPos++;
        }

        if (handle->TmpByte & s_splay_mask[handle->BitPos])
            A = handle->Right[A];
        else
            A = handle->Left[A];
    }
    while (A <= SPLAY_PREDMAX);

    A -= SPLAY_MAXCHAR;
    Splay(handle, A);
    return (A);
}

/*
=======================================
    SPLAY 压缩
=======================================
*/
CR_API leng_t
compr_splay (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    sSPLAY  handle;

    if (dst == NULL)
        return (srclen * 2 + 1);

    struct_zero(&handle, sSPLAY);
    handle.buff = (byte_t*)dst;
    handle.size = dstlen;

    InitializeSplay(&handle);

    for (; srclen != 0; srclen--) {
        if (handle.pntr >= dstlen)
            return (0);
        Compress(&handle, *(byte_t*)src);
        src = (byte_t*)src + 1;
    }
    if (handle.pntr >= dstlen)
        return (0);
    Compress(&handle, SPLAY_EOFCHAR);
    if (handle.BitPos != 0)
        WriteByte(&handle);
    if (handle.pntr > dstlen)
        return (0);
    return (handle.pntr);
}

/*
=======================================
    SPLAY 解压
=======================================
*/
CR_API leng_t
uncompr_splay (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    sSPLAY      handle;
    leng_t      dstidx;
    byte_t*     dstptr;
    CodeType    otbyte;

    struct_zero(&handle, sSPLAY);
    handle.buff = (byte_t*)src;
    handle.size = srclen;
    handle.BitPos = 7;

    InitializeSplay(&handle);

    dstidx = 0;
    dstptr = (byte_t*)dst;

    for (;;) {
        if (handle.pntr >= srclen)
            break;
        otbyte = Expand(&handle);
        if (otbyte == SPLAY_EOFCHAR)
            break;
        if (dstidx++ >= dstlen)
            return (0);
        *dstptr++ = (byte_t)otbyte;
    }
    return (dstidx);
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
