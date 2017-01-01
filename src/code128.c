/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-11-26  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack Code128 数据编码函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"

/* Code128 查表结构 */
typedef struct
{
        byte_t          code_a;     /* A 字符集 */
        byte_t          code_b;     /* B 字符集 */
        const ansi_t*   code128;    /* 转码序列 */

} sCODE128;

/* Code128 常数表 */
static const sCODE128 _rom_ s_code128[] =
{
/*000*/ {   ' ',    ' ',    "11011001100"   },
/*001*/ {   '!',    '!',    "11001101100"   },
/*002*/ {   '\"',   '\"',   "11001100110"   },
/*003*/ {   '#',    '#',    "10010011000"   },
/*004*/ {   '$',    '$',    "10010001100"   },
/*005*/ {   '%',    '%',    "10001001100"   },
/*006*/ {   '&',    '&',    "10011001000"   },
/*007*/ {   '\'',   '\'',   "10011000100"   },
/*008*/ {   '(',    '(',    "10001100100"   },
/*009*/ {   ')',    ')',    "11001001000"   },
/*010*/ {   '*',    '*',    "11001000100"   },
/*011*/ {   '+',    '+',    "11000100100"   },
/*012*/ {   ',',    ',',    "10110011100"   },
/*013*/ {   '-',    '-',    "10011011100"   },
/*014*/ {   '.',    '.',    "10011001110"   },
/*015*/ {   '/',    '/',    "10111001100"   },
/*016*/ {   '0',    '0',    "10011101100"   },
/*017*/ {   '1',    '1',    "10011100110"   },
/*018*/ {   '2',    '2',    "11001110010"   },
/*019*/ {   '3',    '3',    "11001011100"   },
/*020*/ {   '4',    '4',    "11001001110"   },
/*021*/ {   '5',    '5',    "11011100100"   },
/*022*/ {   '6',    '6',    "11001110100"   },
/*023*/ {   '7',    '7',    "11101101110"   },
/*024*/ {   '8',    '8',    "11101001100"   },
/*025*/ {   '9',    '9',    "11100101100"   },
/*026*/ {   ':',    ':',    "11100100110"   },
/*027*/ {   ';',    ';',    "11101100100"   },
/*028*/ {   '<',    '<',    "11100110100"   },
/*029*/ {   '=',    '=',    "11100110010"   },
/*030*/ {   '>',    '>',    "11011011000"   },
/*031*/ {   '?',    '?',    "11011000110"   },
/*032*/ {   '@',    '@',    "11000110110"   },
/*033*/ {   'A',    'A',    "10100011000"   },
/*034*/ {   'B',    'B',    "10001011000"   },
/*035*/ {   'C',    'C',    "10001000110"   },
/*036*/ {   'D',    'D',    "10110001000"   },
/*037*/ {   'E',    'E',    "10001101000"   },
/*038*/ {   'F',    'F',    "10001100010"   },
/*039*/ {   'G',    'G',    "11010001000"   },
/*040*/ {   'H',    'H',    "11000101000"   },
/*041*/ {   'I',    'I',    "11000100010"   },
/*042*/ {   'J',    'J',    "10110111000"   },
/*043*/ {   'K',    'K',    "10110001110"   },
/*044*/ {   'L',    'L',    "10001101110"   },
/*045*/ {   'M',    'M',    "10111011000"   },
/*046*/ {   'N',    'N',    "10111000110"   },
/*047*/ {   'O',    'O',    "10001110110"   },
/*048*/ {   'P',    'P',    "11101110110"   },
/*049*/ {   'Q',    'Q',    "11010001110"   },
/*050*/ {   'R',    'R',    "11000101110"   },
/*051*/ {   'S',    'S',    "11011101000"   },
/*052*/ {   'T',    'T',    "11011100010"   },
/*053*/ {   'U',    'U',    "11011101110"   },
/*054*/ {   'V',    'V',    "11101011000"   },
/*055*/ {   'W',    'W',    "11101000110"   },
/*056*/ {   'X',    'X',    "11100010110"   },
/*057*/ {   'Y',    'Y',    "11101101000"   },
/*058*/ {   'Z',    'Z',    "11101100010"   },
/*059*/ {   '[',    '[',    "11100011010"   },
/*060*/ {   '\\',   '\\',   "11101111010"   },
/*061*/ {   ']',    ']',    "11001000010"   },
/*062*/ {   '^',    '^',    "11110001010"   },
/*063*/ {   '_',    '_',    "10100110000"   },
/*064*/ {   0x00,   0x60,   "10100001100"   },
/*065*/ {   0x01,   'a',    "10010110000"   },
/*066*/ {   0x02,   'b',    "10010000110"   },
/*067*/ {   0x03,   'c',    "10000101100"   },
/*068*/ {   0x04,   'd',    "10000100110"   },
/*069*/ {   0x05,   'e',    "10110010000"   },
/*070*/ {   0x06,   'f',    "10110000100"   },
/*071*/ {   0x07,   'g',    "10011010000"   },
/*072*/ {   0x08,   'h',    "10011000010"   },
/*073*/ {   0x09,   'i',    "10000110100"   },
/*074*/ {   0x0A,   'j',    "10000110010"   },
/*075*/ {   0x0B,   'k',    "11000010010"   },
/*076*/ {   0x0C,   'l',    "11001010000"   },
/*077*/ {   0x0D,   'm',    "11110111010"   },
/*078*/ {   0x0E,   'n',    "11000010100"   },
/*079*/ {   0x0F,   'o',    "10001111010"   },
/*080*/ {   0x10,   'p',    "10100111100"   },
/*081*/ {   0x11,   'q',    "10010111100"   },
/*082*/ {   0x12,   'r',    "10010011110"   },
/*083*/ {   0x13,   's',    "10111100100"   },
/*084*/ {   0x14,   't',    "10011110100"   },
/*085*/ {   0x15,   'u',    "10011110010"   },
/*086*/ {   0x16,   'v',    "11110100100"   },
/*087*/ {   0x17,   'w',    "11110010100"   },
/*088*/ {   0x18,   'x',    "11110010010"   },
/*089*/ {   0x19,   'y',    "11011011110"   },
/*090*/ {   0x1A,   'z',    "11011110110"   },
/*091*/ {   0x1B,   '{',    "11110110110"   },
/*092*/ {   0x1C,   '|',    "10101111000"   },
/*093*/ {   0x1D,   '}',    "10100011110"   },
/*094*/ {   0x1E,   '~',    "10001011110"   },
/*095*/ {   0x1F,   0x7F,   "10111101000"   },
/*096*/ {   0xFF,   0xFF,   "10111100010"   },  /* FNC3 FNC3 96 */
/*097*/ {   0xFF,   0xFF,   "11110101000"   },  /* FNC2 FNC2 97 */
/*098*/ {   0xFF,   0xFF,   "11110100010"   },  /* SHIFT SHIFT 98 */
/*099*/ {   0xFF,   0xFF,   "10111011110"   },  /* CODEC CODEC 99 */
/*100*/ {   0xFF,   0xFF,   "10111101110"   },  /* CODEB FNC4 CODEB */
/*101*/ {   0xFF,   0xFF,   "11101011110"   },  /* FNC4 CODEA CODEA */
/*102*/ {   0xFF,   0xFF,   "11110101110"   },  /* FNC1 x 3 */
/*103*/ {   0xFF,   0xFF,   "11010000100"   },  /* STARTA x 3 */
/*104*/ {   0xFF,   0xFF,   "11010010000"   },  /* STARTB x 3 */
/*105*/ {   0xFF,   0xFF,   "11010011100"   },  /* STARTC x 3 */
/*106*/ {   0xFF,   0xFF,   "1100011101011" },  /* STOP x 3 */
};

/* Code128 特殊字符值 */
#define CODE128_INVS    255
#define CODE128_STOP    106
#define CODE128_STARTA  103
#define CODE128_STARTB  104
#define CODE128_STARTC  105

/*
---------------------------------------
    Code128-A 查找字符
---------------------------------------
*/
static byte_t
code128a_find (
  __CR_IN__ const void_t*   inp
    )
{
    byte_t  cha = *(byte_t*)inp;
    leng_t  idx, cnts = cntsof(s_code128);

    if (cha == 0xFF)
        return (CODE128_INVS);
    for (idx = 0; idx < cnts; idx++) {
        if (cha == s_code128[idx].code_a)
            return ((byte_t)idx);
    }
    return (CODE128_INVS);
}

/*
---------------------------------------
    Code128-B 查找字符
---------------------------------------
*/
static byte_t
code128b_find (
  __CR_IN__ const void_t*   inp
    )
{
    byte_t  cha = *(byte_t*)inp;
    leng_t  idx, cnts = cntsof(s_code128);

    if (cha == 0xFF)
        return (CODE128_INVS);
    for (idx = 0; idx < cnts; idx++) {
        if (cha == s_code128[idx].code_b)
            return ((byte_t)idx);
    }
    return (CODE128_INVS);
}

/*
---------------------------------------
    Code128-C 查找字符
---------------------------------------
*/
static byte_t
code128c_find (
  __CR_IN__ const void_t*   inp
    )
{
    leng_t  cha;
    byte_t  txt[2];

    mem_cpy(txt, inp, sizeof(txt));
    if (txt[0] < '0' || txt[0] > '9')
        return (CODE128_INVS);
    if (txt[1] < '0' || txt[1] > '9')
        return (CODE128_INVS);
    cha  = (leng_t)((txt[0] - '0') * 10);
    cha += (leng_t) (txt[1] - '0');
    return ((byte_t)cha);
}

/*
=======================================
    Code128-A 编码生成
=======================================
*/
CR_API leng_t
gen_code128a (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    uint_t  val;
    uint_t  cksum;
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  i_ptr;
    leng_t  o_ptr;

    if (dst == NULL) {
        dstlen = 2 + srclen;
        return (dstlen * 11 + 13);
    }
    i_ptr = 0;
    o_ptr = 0;
    i_buf = (uchar*)src;
    o_buf = (uchar*)dst;

    /* 输出起始码 */
    if (o_ptr + 11 > dstlen)
        return (0);
    cksum = CODE128_STARTA;
    mem_cpy(o_buf, s_code128[CODE128_STARTA].code128, 11);
    o_buf += 11;
    o_ptr += 11;

    /* 逐位输出数据值 */
    while (i_ptr < srclen)
    {
        /* 查找转换码 */
        val = (uint_t)code128a_find(i_buf);
        if (val == CODE128_INVS)
            return (0);
        i_buf += 1;
        i_ptr += 1;
        cksum = (uint_t)(cksum + val * i_ptr);

        /* 输出转换码 */
        if (o_ptr + 11 > dstlen)
            return (0);
        mem_cpy(o_buf, s_code128[val].code128, 11);
        o_buf += 11;
        o_ptr += 11;
    }

    /* 输出校验码 */
    if (o_ptr + 11 > dstlen)
        return (0);
    mem_cpy(o_buf, s_code128[cksum % 103].code128, 11);
    o_buf += 11;
    o_ptr += 11;

    /* 输出结束码 */
    if (o_ptr + 13 > dstlen)
        return (0);
    mem_cpy(o_buf, s_code128[CODE128_STOP].code128, 13);
    return (o_ptr + 13);
}

/*
=======================================
    Code128-B 编码生成
=======================================
*/
CR_API leng_t
gen_code128b (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    uint_t  val;
    uint_t  cksum;
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  i_ptr;
    leng_t  o_ptr;

    if (dst == NULL) {
        dstlen = 2 + srclen;
        return (dstlen * 11 + 13);
    }
    i_ptr = 0;
    o_ptr = 0;
    i_buf = (uchar*)src;
    o_buf = (uchar*)dst;

    /* 输出起始码 */
    if (o_ptr + 11 > dstlen)
        return (0);
    cksum = CODE128_STARTB;
    mem_cpy(o_buf, s_code128[CODE128_STARTB].code128, 11);
    o_buf += 11;
    o_ptr += 11;

    /* 逐位输出数据值 */
    while (i_ptr < srclen)
    {
        /* 查找转换码 */
        val = (uint_t)code128b_find(i_buf);
        if (val == CODE128_INVS)
            return (0);
        i_buf += 1;
        i_ptr += 1;
        cksum = (uint_t)(cksum + val * i_ptr);

        /* 输出转换码 */
        if (o_ptr + 11 > dstlen)
            return (0);
        mem_cpy(o_buf, s_code128[val].code128, 11);
        o_buf += 11;
        o_ptr += 11;
    }

    /* 输出校验码 */
    if (o_ptr + 11 > dstlen)
        return (0);
    mem_cpy(o_buf, s_code128[cksum % 103].code128, 11);
    o_buf += 11;
    o_ptr += 11;

    /* 输出结束码 */
    if (o_ptr + 13 > dstlen)
        return (0);
    mem_cpy(o_buf, s_code128[CODE128_STOP].code128, 13);
    return (o_ptr + 13);
}

/*
=======================================
    Code128-C 编码生成
=======================================
*/
CR_API leng_t
gen_code128c (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    uint_t  val;
    uint_t  cksum;
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  i_ptr;
    leng_t  o_ptr;

    if (srclen % 2 != 0)
        return (0);
    if (dst == NULL) {
        dstlen = 2 + srclen / 2;
        return (dstlen * 11 + 13);
    }
    i_ptr = 0;
    o_ptr = 0;
    i_buf = (uchar*)src;
    o_buf = (uchar*)dst;

    /* 输出起始码 */
    if (o_ptr + 11 > dstlen)
        return (0);
    cksum = CODE128_STARTC;
    mem_cpy(o_buf, s_code128[CODE128_STARTC].code128, 11);
    o_buf += 11;
    o_ptr += 11;

    /* 逐位输出数据值 */
    while (i_ptr < srclen)
    {
        /* 查找转换码 */
        val = (uint_t)code128c_find(i_buf);
        if (val == CODE128_INVS)
            return (0);
        i_buf += 2;
        i_ptr += 2;
        cksum = (uint_t)(cksum + val * (i_ptr / 2));

        /* 输出转换码 */
        if (o_ptr + 11 > dstlen)
            return (0);
        mem_cpy(o_buf, s_code128[val].code128, 11);
        o_buf += 11;
        o_ptr += 11;
    }

    /* 输出校验码 */
    if (o_ptr + 11 > dstlen)
        return (0);
    mem_cpy(o_buf, s_code128[cksum % 103].code128, 11);
    o_buf += 11;
    o_ptr += 11;

    /* 输出结束码 */
    if (o_ptr + 13 > dstlen)
        return (0);
    mem_cpy(o_buf, s_code128[CODE128_STOP].code128, 13);
    return (o_ptr + 13);
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
