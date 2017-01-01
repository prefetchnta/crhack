/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-05-08  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> 自定义编码转换函数库模板 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/* 骗过 FatFS 的头文件包含 */
#define _FATFS      1
#define _USE_LFN    1

#include "memlib.h"
#include "strlib.h"
#include "util/codepage.h"
#include "../ex3rd/fatfs/integer.h"

/* 根据指定编码值重映射函数 */
#if     (_CODE_PAGE == 932)
    #define ff_convert  ff_convert_932
    #define ff_wtoupper ff_wtoupper_932
    #define str_offset  offset_sjis
    #define str_ccp2uni str_cp932_to_uni
    #define str_uni2ccp str_uni_to_cp932
    #include "../ex3rd/fatfs/option/cc932.c"
#elif   (_CODE_PAGE == 936)
    #define ff_convert  ff_convert_936
    #define ff_wtoupper ff_wtoupper_936
    #define str_offset  offset_gbk
    #define str_ccp2uni str_cp936_to_uni
    #define str_uni2ccp str_uni_to_cp936
    #include "../ex3rd/fatfs/option/cc936.c"
#elif   (_CODE_PAGE == 949)
    #define ff_convert  ff_convert_949
    #define ff_wtoupper ff_wtoupper_949
    #define str_offset  offset_gb2312
    #define str_ccp2uni str_cp949_to_uni
    #define str_uni2ccp str_uni_to_cp949
    #include "../ex3rd/fatfs/option/cc949.c"
#elif   (_CODE_PAGE == 950)
    #define ff_convert  ff_convert_950
    #define ff_wtoupper ff_wtoupper_950
    #define str_offset  offset_big5
    #define str_ccp2uni str_cp950_to_uni
    #define str_uni2ccp str_uni_to_cp950
    #include "../ex3rd/fatfs/option/cc950.c"
#else
#if     (_CODE_PAGE == 437)
    #define ff_convert  ff_convert_437
    #define ff_wtoupper ff_wtoupper_437
    #define str_ccp2uni str_cp437_to_uni
    #define str_uni2ccp str_uni_to_cp437
#elif   (_CODE_PAGE == 720)
    #define ff_convert  ff_convert_720
    #define ff_wtoupper ff_wtoupper_720
    #define str_ccp2uni str_cp720_to_uni
    #define str_uni2ccp str_uni_to_cp720
#elif   (_CODE_PAGE == 737)
    #define ff_convert  ff_convert_737
    #define ff_wtoupper ff_wtoupper_737
    #define str_ccp2uni str_cp737_to_uni
    #define str_uni2ccp str_uni_to_cp737
#elif   (_CODE_PAGE == 771)
    #define ff_convert  ff_convert_771
    #define ff_wtoupper ff_wtoupper_771
    #define str_ccp2uni str_cp771_to_uni
    #define str_uni2ccp str_uni_to_cp771
#elif   (_CODE_PAGE == 775)
    #define ff_convert  ff_convert_775
    #define ff_wtoupper ff_wtoupper_775
    #define str_ccp2uni str_cp775_to_uni
    #define str_uni2ccp str_uni_to_cp775
#elif   (_CODE_PAGE == 850)
    #define ff_convert  ff_convert_850
    #define ff_wtoupper ff_wtoupper_850
    #define str_ccp2uni str_cp850_to_uni
    #define str_uni2ccp str_uni_to_cp850
#elif   (_CODE_PAGE == 852)
    #define ff_convert  ff_convert_852
    #define ff_wtoupper ff_wtoupper_852
    #define str_ccp2uni str_cp852_to_uni
    #define str_uni2ccp str_uni_to_cp852
#elif   (_CODE_PAGE == 855)
    #define ff_convert  ff_convert_855
    #define ff_wtoupper ff_wtoupper_855
    #define str_ccp2uni str_cp855_to_uni
    #define str_uni2ccp str_uni_to_cp855
#elif   (_CODE_PAGE == 857)
    #define ff_convert  ff_convert_857
    #define ff_wtoupper ff_wtoupper_857
    #define str_ccp2uni str_cp857_to_uni
    #define str_uni2ccp str_uni_to_cp857
#elif   (_CODE_PAGE == 860)
    #define ff_convert  ff_convert_860
    #define ff_wtoupper ff_wtoupper_860
    #define str_ccp2uni str_cp860_to_uni
    #define str_uni2ccp str_uni_to_cp860
#elif   (_CODE_PAGE == 861)
    #define ff_convert  ff_convert_861
    #define ff_wtoupper ff_wtoupper_861
    #define str_ccp2uni str_cp861_to_uni
    #define str_uni2ccp str_uni_to_cp861
#elif   (_CODE_PAGE == 862)
    #define ff_convert  ff_convert_862
    #define ff_wtoupper ff_wtoupper_862
    #define str_ccp2uni str_cp862_to_uni
    #define str_uni2ccp str_uni_to_cp862
#elif   (_CODE_PAGE == 863)
    #define ff_convert  ff_convert_863
    #define ff_wtoupper ff_wtoupper_863
    #define str_ccp2uni str_cp863_to_uni
    #define str_uni2ccp str_uni_to_cp863
#elif   (_CODE_PAGE == 864)
    #define ff_convert  ff_convert_864
    #define ff_wtoupper ff_wtoupper_864
    #define str_ccp2uni str_cp864_to_uni
    #define str_uni2ccp str_uni_to_cp864
#elif   (_CODE_PAGE == 865)
    #define ff_convert  ff_convert_865
    #define ff_wtoupper ff_wtoupper_865
    #define str_ccp2uni str_cp865_to_uni
    #define str_uni2ccp str_uni_to_cp865
#elif   (_CODE_PAGE == 866)
    #define ff_convert  ff_convert_866
    #define ff_wtoupper ff_wtoupper_866
    #define str_ccp2uni str_cp866_to_uni
    #define str_uni2ccp str_uni_to_cp866
#elif   (_CODE_PAGE == 869)
    #define ff_convert  ff_convert_869
    #define ff_wtoupper ff_wtoupper_869
    #define str_ccp2uni str_cp869_to_uni
    #define str_uni2ccp str_uni_to_cp869
#endif
    #define str_offset  offset_ascii
    #include "../ex3rd/fatfs/option/ccsbcs.c"
#endif

/*
=======================================
    多字节转 UNICODE
=======================================
*/
CR_API void_t*
str_ccp2uni (
  __CR_IN__ const ansi_t*   str,
  __CR_OT__ leng_t*         size,
  __CR_IN__ bool_t          use_utf16
    )
{
    WCHAR   temp;
    ansi_t  save;
    uint_t  move;
    wide_t* back;
    ansi_t* pntr;
    leng_t  dstlen;

    /* 先转换到 UTF-16 */
    dstlen = 0;
    pntr = (ansi_t*)str;
    do {
        save = *pntr;
        move = str_offset(pntr);
        if (move == 0)
            pntr += sizeof(ansi_t);
        else
            pntr += move;
        dstlen += 1;
    } while (save != 0);

    /* 分配内存 */
    back = str_allocW(dstlen);
    if (back == NULL)
        return (NULL);

    /* 字符转换过去 */
    dstlen = 0;
    pntr = (ansi_t*)str;
    do {
        save = *pntr;
        move = str_offset(pntr);
        if (move == 0) {
            back[dstlen++] = 0x20;
            pntr += sizeof(ansi_t);
        }
        else {
            temp = pntr[0];
            if (move == 2) {
                temp <<= 8;
                temp |= pntr[1];
            }
            pntr += move;
            back[dstlen++] = ff_convert(temp, 1);
        }
    } while (save != 0);

    /* 直接返回 */
    if (use_utf16) {
        if (size != NULL)
            *size = dstlen * sizeof(wide_t);
        return (back);
    }

    /* 转换到 UTF-8 */
    pntr = utf16_to_utf8(back);
    mem_free(back);
    if (pntr == NULL)
        return (NULL);
    if (size != NULL)
        *size = str_sizeA(pntr);
    return (pntr);
}

/*
=======================================
    UNICODE 转多字节
=======================================
*/
CR_API ansi_t*
str_uni2ccp (
  __CR_IN__ const void_t*   str,
  __CR_OT__ leng_t*         size,
  __CR_IN__ bool_t          use_utf16
    )
{
    WCHAR   temp;
    uint_t  move;
    int32u  ucs4;
    ansi_t* back;
    ansi_t* pntr;
    leng_t  dstlen;

    /* 输入是 UTF-16 */
    dstlen = 0;
    pntr = (ansi_t*)str;
    if (use_utf16)
    {
        /* 估算最后的大小 */
        do {
            move = utf16_to_ucs4(&ucs4, pntr);
            if (move == 0) {
                ucs4 = 0x20;
                dstlen += 1;
                pntr += sizeof(wide_t);
            }
            else {
                if (ucs4 <= 0xFFFF) {
                    temp = ff_convert((WCHAR)ucs4, 0);
                    if (temp <= 0xFF)
                        dstlen += 1;
                    else
                        dstlen += 2;
                }
                else {
                    dstlen += 1;
                }
                pntr += move;
            }
        } while (ucs4 != 0);

        /* 分配内存 */
        back = str_allocA(dstlen);
        if (back == NULL)
            return (NULL);

        /* 字符转换过去 */
        dstlen = 0;
        pntr = (ansi_t*)str;
        do {
            move = utf16_to_ucs4(&ucs4, pntr);
            if (move == 0) {
                back[dstlen++] = 0x20;
                pntr += sizeof(wide_t);
            }
            else {
                if (ucs4 <= 0xFFFF) {
                    temp = ff_convert((WCHAR)ucs4, 0);
                    if (temp > 0xFF)
                        back[dstlen++] = (ansi_t)(temp >> 8);
                    back[dstlen++] = (ansi_t)(temp);
                }
                else {
                    back[dstlen++] = 0x20;
                }
                pntr += move;
            }
        } while (ucs4 != 0);
    }
    else
    {
        /* 估算最后的大小 */
        do {
            move = utf8_to_ucs4(&ucs4, pntr);
            if (move == 0) {
                ucs4 = 0x20;
                dstlen += 1;
                pntr += sizeof(ansi_t);
            }
            else {
                if (ucs4 <= 0xFFFF) {
                    temp = ff_convert((WCHAR)ucs4, 0);
                    if (temp <= 0xFF)
                        dstlen += 1;
                    else
                        dstlen += 2;
                }
                else {
                    dstlen += 1;
                }
                pntr += move;
            }
        } while (ucs4 != 0);

        /* 分配内存 */
        back = str_allocA(dstlen);
        if (back == NULL)
            return (NULL);

        /* 字符转换过去 */
        dstlen = 0;
        pntr = (ansi_t*)str;
        do {
            move = utf8_to_ucs4(&ucs4, pntr);
            if (move == 0) {
                back[dstlen++] = 0x20;
                pntr += sizeof(ansi_t);
            }
            else {
                if (ucs4 <= 0xFFFF) {
                    temp = ff_convert((WCHAR)ucs4, 0);
                    if (temp > 0xFF)
                        back[dstlen++] = (ansi_t)(temp >> 8);
                    back[dstlen++] = (ansi_t)(temp);
                }
                else {
                    back[dstlen++] = 0x20;
                }
                pntr += move;
            }
        } while (ucs4 != 0);
    }

    /* 返回结果 */
    if (size != NULL)
        *size = dstlen;
    return (back);
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
