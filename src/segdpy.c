/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-09-17  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack 七段数码管字符串转换函数库 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "msclib.h"

/* dp-gfedcba */
/* 共阴七段数码管笔划表 (1亮0灭) */
#ifndef _CR_CUSTOM_SEG7_

/* 小数点 */
#ifndef SEG7DP
    #define SEG7DP  0x80
#endif
#ifndef SEG7ERR
    #define SEG7ERR 0xFF
#endif

/* 不分大小写 */
static const byte_t _rom_ s_seg7u[] =
{
    0x3F,   /* 0 */
    0x06,   /* 1 */
    0x5B,   /* 2 */
    0x4F,   /* 3 */
    0x66,   /* 4 */
    0x6D,   /* 5 */
    0x7D,   /* 6 */
    0x07,   /* 7 */
    0x7F,   /* 8 */
    0x6F,   /* 9 */
    0x77,   /* A */
    0x7C,   /* b */
    0x39,   /* C */
    0x5E,   /* d */
    0x79,   /* E */
    0x71,   /* F */
    0x73,   /* P, 16 */
    0x6E,   /* y, 17 */
    0x38,   /* L, 18 */
    0x78,   /* t, 19 */
    0x67,   /* q, 20 */
    0x6D,   /* S, 21 */
    0x50,   /* r, 22 */
    0x0E,   /* J, 23 */
};

/* 区分大小写 */
static const byte_t _rom_ s_seg7s[] =
{
    0x00,   /*  , 00 */
    0x40,   /* -, 01 */
    0x08,   /* _, 02 */
    0x39,   /* C, 03 */
    0x58,   /* c, 04 */
    0x37,   /* N, 05 */
    0x54,   /* n, 06 */
    0x3E,   /* U, 07 */
    0x1C,   /* u, 08 */
    0x76,   /* H, 09 */
    0x74,   /* h, 10 */
    0x3F,   /* O, 11 */
    0x5C,   /* o, 12 */
};

#endif  /* !_CR_CUSTOM_SEG7_ */

/*
---------------------------------------
    七段数码管字符转换
---------------------------------------
*/
static byte_t
seg7dp_ch_gen (
  __CR_IN__ ansi_t  ch
    )
{
#ifndef _CR_CUSTOM_SEG7X_
    if (ch < CR_AC(' '))
        return (SEG7ERR);
#else
    /* 有32个自定义的字符可用 */
    if (ch >= CR_AC(NIL) && ch < CR_AC(' '))
        return (s_seg7x[(byte_t)ch]);
#endif
    /* 数字优先转换 */
    if (ch >= CR_AC('0') && ch <= CR_AC('9'))
        return (s_seg7u[(byte_t)ch & 0x0F]);

    /* 区分大小写 */
    switch (ch)
    {
        case CR_AC(' '): return (s_seg7s[ 0]);
        case CR_AC('-'): return (s_seg7s[ 1]);
        case CR_AC('_'): return (s_seg7s[ 2]);
        case CR_AC('C'): return (s_seg7s[ 3]);
        case CR_AC('c'): return (s_seg7s[ 4]);
        case CR_AC('N'): return (s_seg7s[ 5]);
        case CR_AC('n'): return (s_seg7s[ 6]);
        case CR_AC('U'): return (s_seg7s[ 7]);
        case CR_AC('u'): return (s_seg7s[ 8]);
        case CR_AC('H'): return (s_seg7s[ 9]);
        case CR_AC('h'): return (s_seg7s[10]);
        case CR_AC('O'): return (s_seg7s[11]);
        case CR_AC('o'): return (s_seg7s[12]);
        default: break;
    }

    /* 不分大小写 */
    if (ch >= CR_AC('a') && ch <= CR_AC('z'))
        ch = ch - 'a' + 'A';
    if (ch >= CR_AC('A') && ch <= CR_AC('F'))
        return (s_seg7u[((byte_t)ch & 0x0F) + 9]);
    switch (ch)
    {
        case CR_AC('p'):
        case CR_AC('P'): return (s_seg7u[16]);
        case CR_AC('y'):
        case CR_AC('Y'): return (s_seg7u[17]);
        case CR_AC('l'):
        case CR_AC('L'): return (s_seg7u[18]);
        case CR_AC('t'):
        case CR_AC('T'): return (s_seg7u[19]);
        case CR_AC('q'):
        case CR_AC('Q'): return (s_seg7u[20]);
        case CR_AC('s'):
        case CR_AC('S'): return (s_seg7u[21]);
        case CR_AC('r'):
        case CR_AC('R'): return (s_seg7u[22]);
        case CR_AC('j'):
        case CR_AC('J'): return (s_seg7u[23]);
        default: break;
    }
    return (SEG7ERR);
}

/*
=======================================
    七段数码管字符串转换
=======================================
*/
CR_API byte_t*
seg7dp_gen (
  __CR_OT__ byte_t*         dst,
  __CR_IN__ const ansi_t*   src,
  __CR_IN__ ubit_t          neg,
  __CR_IN__ leng_t          size
    )
{
    byte_t  seg, msk;
    byte_t* ret = dst;

    if (neg)
        msk = 0xFF;
    else
        msk = 0x00;
    while (size != 0 && *src != CR_AC(NIL)) {
        seg = seg7dp_ch_gen(*src++);
        if (*src == CR_AC('.')) {
            src += 1;
            seg |= SEG7DP;
        }
        size -= 1;
        *dst++ = seg ^ msk;
    }
    return (ret);
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
