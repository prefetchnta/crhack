/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-03-02  */
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
/*  >>>>>>>>>>>>>>>> CrHack 字符串编码转换函数库 for POSIX <<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"
#include "memlib.h"
#include "strlib.h"

#if !defined(_CR_NO_LIBICONV_)

#include <iconv.h>

/* 使用自定义的编码值 */
extern uint_t   g_codepage;

/* 外挂的编码转换函数 */
extern cr_acp2uni_t g_str_acp2uni;
extern cr_uni2acp_t g_str_uni2acp;

/*
=======================================
    获取系统本地编码值
=======================================
*/
CR_API uint_t
get_sys_codepage (void_t)
{
    return (g_codepage);
}

/*
---------------------------------------
    根据编码值返回编码字符串
---------------------------------------
*/
static const ansi_t*
iconv_get_codepage (
  __CR_IN__ uint_t  codepage
    )
{
    switch (codepage)
    {
        default:
            break;

        /* 本地编码 */
        case CR_LOCAL:
            return (iconv_get_codepage(g_codepage));

        /* IBM EBCDIC - U.S./Canada */
        case 37:
            return (NULL);

        /* OEM - United States */
        case 437:
            return ("CP437");

        /* IBM EBCDIC - International */
        case 500:
            return (NULL);

        /* Arabic - ASMO 708 */
        case 708:
            return (NULL);

        /* Arabic - ASMO 449+, BCON V4 */
        case 709:
            return (NULL);

        /* Arabic - Transparent Arabic */
        case 710:
            return (NULL);

        /* Arabic - Transparent ASMO */
        case 720:
            return (NULL);

        /* OEM - Greek (formerly 437G) */
        case 737:
            return ("CP737");

        /* OEM - Baltic */
        case 775:
            return ("CP775");

        /* OEM - Multilingual Latin I */
        case 850:
            return ("CP850");

        /* OEM - Latin II */
        case 852:
            return ("CP852");

        /* CodePage - 853 (Windows not supported) */
        case 853:
            return ("CP853");

        /* OEM - Cyrillic (primarily Russian) */
        case 855:
            return ("CP855");

        /* OEM - Turkish */
        case 857:
            return ("CP857");

        /* OEM - Multilingual Latin I + Euro symbol */
        case 858:
            return ("CP858");

        /* OEM - Portuguese */
        case 860:
            return ("CP860");

        /* OEM - Icelandic */
        case 861:
            return ("CP861");

        /* OEM - Hebrew */
        case 862:
            return ("CP862");

        /* OEM - Canadian-French */
        case 863:
            return ("CP863");

        /* OEM - Arabic */
        case 864:
            return ("CP864");

        /* OEM - Nordic */
        case 865:
            return ("CP865");

        /* OEM - Russian */
        case 866:
            return ("CP866");

        /* OEM - Modern Greek */
        case 869:
            return ("CP869");

        /* IBM EBCDIC - Multilingual/ROECE (Latin-2) */
        case 870:
            return (NULL);

        /* ANSI/OEM - Thai (same as 28605, ISO 8859-15) */
        case 874:
            return ("CP874");

        /* IBM EBCDIC - Modern Greek */
        case 875:
            return (NULL);

        /* ANSI/OEM - Japanese, Shift-JIS */
        case 932:
            return ("SHIFT_JIS");

        /* ANSI/OEM - Simplified Chinese (PRC, Singapore) */
        case 936:
            return ("GBK");

        /* ANSI/OEM - Korean (Unified Hangul Code) */
        case 949:
            return ("CP949");

        /* ANSI/OEM - Traditional Chinese (Taiwan; Hong Kong SAR, PRC) */
        case 950:
            return ("BIG5");

        /* IBM EBCDIC - Turkish (Latin-5) */
        case 1026:
            return (NULL);

        /* IBM EBCDIC - Latin 1/Open System */
        case 1047:
            return (NULL);

        /* CodePage - 1125 (Windows not supported) */
        case 1125:
            return ("CP1125");

        /* CodePage - 1131 (Windows not supported) */
        case 1131:
            return ("CP1131");

        /* CodePage - 1133 (Windows not supported) */
        case 1133:
            return ("CP1133");

        /* IBM EBCDIC - U.S./Canada (037 + Euro symbol) */
        case 1140:
            return (NULL);

        /* IBM EBCDIC - Germany (20273 + Euro symbol) */
        case 1141:
            return (NULL);

        /* IBM EBCDIC - Denmark/Norway (20277 + Euro symbol) */
        case 1142:
            return (NULL);

        /* IBM EBCDIC - Finland/Sweden (20278 + Euro symbol) */
        case 1143:
            return (NULL);

        /* IBM EBCDIC - Italy (20280 + Euro symbol) */
        case 1144:
            return (NULL);

        /* IBM EBCDIC - Latin America/Spain (20284 + Euro symbol) */
        case 1145:
            return (NULL);

        /* IBM EBCDIC - United Kingdom (20285 + Euro symbol) */
        case 1146:
            return (NULL);

        /* IBM EBCDIC - France (20297 + Euro symbol) */
        case 1147:
            return (NULL);

        /* IBM EBCDIC - International (500 + Euro symbol) */
        case 1148:
            return (NULL);

        /* IBM EBCDIC - Icelandic (20871 + Euro symbol) */
        case 1149:
            return (NULL);

        /* Unicode UCS-2 Little-Endian (BMP of ISO 10646) */
        case 1200:
            return ("UCS-2LE");

        /* Unicode UCS-2 Big-Endian */
        case 1201:
            return ("UCS-2BE");

        /* ANSI - Central European */
        case 1250:
            return ("CP1250");

        /* ANSI - Cyrillic */
        case 1251:
            return ("CP1251");

        /* ANSI - Latin I */
        case 1252:
            return ("CP1252");

        /* ANSI - Greek */
        case 1253:
            return ("CP1253");

        /* ANSI - Turkish */
        case 1254:
            return ("CP1254");

        /* ANSI - Hebrew */
        case 1255:
            return ("CP1255");

        /* ANSI - Arabic */
        case 1256:
            return ("CP1256");

        /* ANSI - Baltic */
        case 1257:
            return ("CP1257");

        /* ANSI/OEM - Vietnamese */
        case 1258:
            return ("CP1258");

        /* Korean (Johab) */
        case 1361:
            return ("JOHAB");

        /* MAC - Roman */
        case 10000:
            return ("MACROMAN");

        /* MAC - Japanese */
        case 10001:
            return (NULL);

        /* MAC - Traditional Chinese (Big5) */
        case 10002:
            return (NULL);

        /* MAC - Korean */
        case 10003:
            return (NULL);

        /* MAC - Arabic */
        case 10004:
            return ("MACARABIC");

        /* MAC - Hebrew */
        case 10005:
            return ("MACHEBREW");

        /* MAC - Greek I */
        case 10006:
            return ("MACGREEK");

        /* MAC - Cyrillic */
        case 10007:
            return ("MACCYRILLIC");

        /* MAC - Simplified Chinese (GB 2312) */
        case 10008:
            return (NULL);

        /* MAC - Romania */
        case 10010:
            return ("MACROMANIA");

        /* MAC - Ukraine */
        case 10017:
            return ("MACUKRAINE");

        /* MAC - Thai */
        case 10021:
            return ("MACTHAI");

        /* MAC - Latin II */
        case 10029:
            return (NULL);

        /* MAC - Icelandic */
        case 10079:
            return ("MACICELAND");

        /* MAC - Turkish */
        case 10081:
            return ("MACTURKISH");

        /* MAC - Croatia */
        case 10082:
            return ("MACCROATIAN");

        /* Unicode UCS-4 Little-Endian */
        case 12000:
            return ("UCS-4LE");

        /* Unicode UCS-4 Big-Endian */
        case 12001:
            return ("UCS-4BE");

        /* CNS - Taiwan */
        case 20000:
            return (NULL);

        /* TCA - Taiwan */
        case 20001:
            return (NULL);

        /* Eten - Taiwan */
        case 20002:
            return (NULL);

        /* IBM5550 - Taiwan */
        case 20003:
            return (NULL);

        /* TeleText - Taiwan */
        case 20004:
            return (NULL);

        /* Wang - Taiwan */
        case 20005:
            return (NULL);

        /* IA5 IRV International Alphabet No. 5 (7-bit) */
        case 20105:
            return (NULL);

        /* IA5 German (7-bit) */
        case 20106:
            return (NULL);

        /* IA5 Swedish (7-bit) */
        case 20107:
            return (NULL);

        /* IA5 Norwegian (7-bit) */
        case 20108:
            return (NULL);

        /* US-ASCII (7-bit) */
        case 20127:
            return ("ASCII");

        /* T.61 */
        case 20261:
            return (NULL);

        /* ISO 6937 Non-Spacing Accent */
        case 20269:
            return (NULL);

        /* IBM EBCDIC - Germany */
        case 20273:
            return (NULL);

        /* IBM EBCDIC - Denmark/Norway */
        case 20277:
            return (NULL);

        /* IBM EBCDIC - Finland/Sweden */
        case 20278:
            return (NULL);

        /* IBM EBCDIC - Italy */
        case 20280:
            return (NULL);

        /* IBM EBCDIC - Latin America/Spain */
        case 20284:
            return (NULL);

        /* IBM EBCDIC - United Kingdom */
        case 20285:
            return (NULL);

        /* IBM EBCDIC - Japanese Katakana Extended */
        case 20290:
            return (NULL);

        /* IBM EBCDIC - France */
        case 20297:
            return (NULL);

        /* IBM EBCDIC - Arabic */
        case 20420:
            return (NULL);

        /* IBM EBCDIC - Greek */
        case 20423:
            return (NULL);

        /* IBM EBCDIC - Hebrew */
        case 20424:
            return (NULL);

        /* IBM EBCDIC - Korean Extended */
        case 20833:
            return (NULL);

        /* IBM EBCDIC - Thai */
        case 20838:
            return (NULL);

        /* Russian - KOI8-R */
        case 20866:
            return ("KOI8-R");

        /* IBM EBCDIC - Icelandic */
        case 20871:
            return (NULL);

        /* IBM EBCDIC - Cyrillic (Russian) */
        case 20880:
            return (NULL);

        /* IBM EBCDIC - Turkish */
        case 20905:
            return (NULL);

        /* IBM EBCDIC - Latin-1/Open System (1047 + Euro symbol) */
        case 20924:
            return (NULL);

        /* JIS X 0208-1990 & 0121-1990 */
        case 20932:
            return (NULL);

        /* Simplified Chinese (GB2312) */
        case 20936:
            return ("GB2312");

        /* IBM EBCDIC - Cyrillic (Serbian, Bulgarian) */
        case 21025:
            return (NULL);

        /* (deprecated) */
        case 21027:
            return (NULL);

        /* Ukrainian (KOI8-U) */
        case 21866:
            return ("KOI8-RU");

        /* ISO 8859-1 Latin I */
        case 28591:
            return ("ISO-8859-1");

        /* ISO 8859-2 Central Europe */
        case 28592:
            return ("ISO-8859-2");

        /* ISO 8859-3 Latin 3 */
        case 28593:
            return ("ISO-8859-3");

        /* ISO 8859-4 Baltic */
        case 28594:
            return ("ISO-8859-4");

        /* ISO 8859-5 Cyrillic */
        case 28595:
            return ("ISO-8859-5");

        /* ISO 8859-6 Arabic */
        case 28596:
            return ("ISO-8859-6");

        /* ISO 8859-7 Greek */
        case 28597:
            return ("ISO-8859-7");

        /* ISO 8859-8 Hebrew */
        case 28598:
            return ("ISO-8859-8");

        /* ISO 8859-9 Latin 5 */
        case 28599:
            return ("ISO-8859-9");

        /* ISO 8859-15 Latin 9 */
        case 28605:
            return ("ISO-8859-15");

        /* Europa 3 */
        case 29001:
            return (NULL);

        /* ISO 8859-8 Hebrew */
        case 38598:
            return ("ISO-8859-8");

        /* ISO 2022 Japanese with no halfwidth Katakana */
        case 50220:
            return ("ISO-2022-JP-2");

        /* ISO 2022 Japanese with halfwidth Katakana */
        case 50221:
            return ("ISO-2022-JP-2");

        /* ISO 2022 Japanese JIS X 0201-1989 */
        case 50222:
            return ("ISO-2022-JP-1");

        /* ISO 2022 Korean */
        case 50225:
            return ("ISO-2022-KR");

        /* ISO 2022 Simplified Chinese */
        case 50227:
            return ("ISO-2022-CN-EXT");

        /* ISO 2022 Traditional Chinese */
        case 50229:
            return (NULL);

        /* Japanese (Katakana) Extended */
        case 50930:
            return (NULL);

        /* US/Canada and Japanese */
        case 50931:
            return (NULL);

        /* Korean Extended and Korean */
        case 50933:
            return (NULL);

        /* Simplified Chinese Extended and Simplified Chinese */
        case 50935:
            return (NULL);

        /* Simplified Chinese */
        case 50936:
            return ("GBK");

        /* US/Canada and Traditional Chinese */
        case 50937:
            return (NULL);

        /* Japanese (Latin) Extended and Japanese */
        case 50939:
            return (NULL);

        /* EUC - Japanese */
        case 51932:
            return ("EUC-JP");

        /* EUC - Simplified Chinese */
        case 51936:
            return ("GB2312");

        /* EUC - Korean */
        case 51949:
            return ("EUC-KR");

        /* EUC - Traditional Chinese */
        case 51950:
            return ("EUC-TW");

        /* HZ-GB2312 Simplified Chinese */
        case 52936:
            return (NULL);

        /* GB18030 Simplified Chinese (4 Byte) */
        case 54936:
            return ("GB18030");

        /* ISCII Devanagari */
        case 57002:
            return (NULL);

        /* ISCII Bengali */
        case 57003:
            return (NULL);

        /* ISCII Tamil */
        case 57004:
            return (NULL);

        /* ISCII Telugu */
        case 57005:
            return (NULL);

        /* ISCII Assamese */
        case 57006:
            return (NULL);

        /* ISCII Oriya */
        case 57007:
            return (NULL);

        /* ISCII Kannada */
        case 57008:
            return (NULL);

        /* ISCII Malayalam */
        case 57009:
            return (NULL);

        /* ISCII Gujarati */
        case 57010:
            return (NULL);

        /* ISCII Punjabi */
        case 57011:
            return (NULL);

        /* Unicode UTF-7 */
        case 65000:
            return ("UTF-7");

        /* Unicode UTF-8 */
        case 65001:
            return ("UTF-8");
    }
    return (NULL);
}

/*
---------------------------------------
    计算转换目标字符串需要的大小
---------------------------------------
*/
static leng_t
iconv_dst_size (
  __CR_IN__ iconv_t         cd,
  __CR_IN__ const ansi_t*   src,
  __CR_IN__ leng_t          len
    )
{
    leng_t  dst_size = 0;
    achar*  temp, buff[4096];
    size_t  rett, rest, size = (size_t)len;

    while (size != 0)
    {
        temp = buff;
        rest = sizeof(buff);

        /* 转换尽可能多的多字节字符 */
        rett = iconv(cd, (char**)&src, &size, &temp, &rest);
        if (temp != buff)
            dst_size += (leng_t)(temp - buff);
        else if (rett == (size_t)-1)
            return (0);

        temp = buff;
        rest = sizeof(buff);

        /* 转换完一个完整的多字节字符 */
        rett = iconv(cd, NULL, NULL, &temp, &rest);
        if (temp != buff)
            dst_size += (leng_t)(temp - buff);
        else if (rett == (size_t)-1)
            return (0);
    }
    return (dst_size);
}

/*
=======================================
    多字节转 UNICODE
=======================================
*/
CR_API void_t*
str_acp2uni (
  __CR_IN__ uint_t          codepage,
  __CR_IN__ const ansi_t*   str,
  __CR_OT__ leng_t*         size,
  __CR_IN__ bool_t          use_utf16
    )
{
#if (_LIBICONV_VERSION >= 0x0108)
    sint_t  ilseq = 1;
#endif
    iconv_t         cd;
    const ansi_t*   to;
    const ansi_t*   from;
    /* --------------- */
    ansi_t  *temp, *dest;
    leng_t  dst_size, src_size;

    /* 使用外挂 */
    if (g_str_acp2uni != NULL)
        return (g_str_acp2uni(codepage, str, size, use_utf16));

    /* 输入必须是多字节编码 */
    if (is_cr_widechar(codepage))
        return (NULL);
    from = iconv_get_codepage(codepage);
    if (from == NULL)
        return (NULL);

    if (!use_utf16) {
        to = "UTF-8";
    }
    else {
#if defined(_CR_ORDER_LE_)
        to = "UTF-16LE";
#else   /* (_CR_ORDER_BE_) */
        to = "UTF-16BE";
#endif
    }

    /* 初始化 iconv */
    cd = iconv_open(to, from);
    if (cd == (iconv_t)-1)
        return (NULL);

    /* 设置非法字符继续的开关 */
#if (_LIBICONV_VERSION >= 0x0108)
    if (iconvctl(cd, ICONV_SET_DISCARD_ILSEQ, &ilseq) != 0)
        goto _failure;
#endif

    /* 计算目标缓冲的大小 */
    src_size = str_sizeA(str);
    dst_size = iconv_dst_size(cd, str, src_size);
    if (dst_size == 0)
        goto _failure;

    if (size != NULL)
        *size = dst_size;
    dst_size += 2;
    temp = dest = str_allocA(dst_size);
    if (dest == NULL)
        goto _failure;

    mem_zero(dest, dst_size);
    iconv(cd, NULL, NULL, NULL, NULL);

    /* 一次性转换字符串 */
    if (iconv(cd, (char**)&str, (size_t*)&src_size, &temp,
                (size_t*)&dst_size) == (size_t)-1) {
        mem_free(dest);
        goto _failure;
    }
    iconv_close(cd);
    return (dest);

_failure:
    iconv_close(cd);
    return (NULL);
}

/*
=======================================
    UNICODE 转多字节
=======================================
*/
CR_API ansi_t*
str_uni2acp (
  __CR_IN__ uint_t          codepage,
  __CR_IN__ const void_t*   str,
  __CR_OT__ leng_t*         size,
  __CR_IN__ bool_t          use_utf16
    )
{
#if (_LIBICONV_VERSION >= 0x0108)
    sint_t  ilseq = 1;
#endif
    iconv_t         cd;
    const ansi_t*   to;
    const ansi_t*   src;
    const ansi_t*   from;
    /* --------------- */
    ansi_t  *temp, *dest;
    leng_t  dst_size, src_size;

    /* 使用外挂 */
    if (g_str_uni2acp != NULL)
        return (g_str_uni2acp(codepage, str, size, use_utf16));

    /* 输入必须是多字节编码 */
    if (is_cr_widechar(codepage))
        return (NULL);
    to = iconv_get_codepage(codepage);
    if (to == NULL)
        return (NULL);

    if (!use_utf16) {
        from = "UTF-8";
    }
    else {
#if defined(_CR_ORDER_LE_)
        from = "UTF-16LE";
#else   /* (_CR_ORDER_BE_) */
        from = "UTF-16BE";
#endif
    }

    /* 初始化 iconv */
    cd = iconv_open(to, from);
    if (cd == (iconv_t)-1)
        return (NULL);

    /* 设置非法字符继续的开关 */
#if (_LIBICONV_VERSION >= 0x0108)
    if (iconvctl(cd, ICONV_SET_DISCARD_ILSEQ, &ilseq) != 0)
        goto _failure;
#endif

    /* 计算目标缓冲的大小 */
    src = (const ansi_t*)str;
    if (!use_utf16)
        src_size = str_sizeA((ansi_t*)str);
    else
        src_size = str_sizeW((wide_t*)str);
    dst_size = iconv_dst_size(cd, src, src_size);
    if (dst_size == 0)
        goto _failure;

    if (size != NULL)
        *size = dst_size;
    dst_size += 1;
    temp = dest = str_allocA(dst_size);
    if (dest == NULL)
        goto _failure;

    mem_zero(dest, dst_size);
    iconv(cd, NULL, NULL, NULL, NULL);

    /* 一次性转换字符串 */
    if (iconv(cd, (char**)&src, (size_t*)&src_size, &temp,
                (size_t*)&dst_size) == (size_t)-1) {
        mem_free(dest);
        goto _failure;
    }
    iconv_close(cd);
    return (dest);

_failure:
    iconv_close(cd);
    return (NULL);
}

#else

#include "../naked/codepage.c"

#endif  /* !_CR_NO_LIBICONV_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
