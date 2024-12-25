/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-12-15  */
/*     #######          ###    ###      [SOFT]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> DTM_NEXT 类型操作函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "morder.h"
#include "msclib.h"
#include "rtclib.h"
#include "strlib.h"
#include "typelib.h"

/* 日期时间分隔符 */
#define DTM_TM_CSEP '@'
#define DTM_TM_SSEP "@"

/*****************************************************************************/
/*                               扩展格式转换                                */
/*****************************************************************************/

/*
=======================================
    合成压缩的整数类型
=======================================
*/
CR_API bool_t
pack_int (
  __CR_OT__ sPACK_INT*  dst,
  __CR_IN__ int32s      src
    )
{
    int32u  val = src;

    if (val > 0x00FFFFFFUL &&
        val < 0xFF800000UL)
        return (FALSE);
    dst->value[0] = (byte_t)(val >> 16);
    dst->value[1] = (byte_t)(val >>  8);
    dst->value[2] = (byte_t)(val >>  0);
    return (TRUE);
}

/*
=======================================
    分解压缩的整数类型
=======================================
*/
CR_API int32s
unpack_int (
  __CR_IN__ const sPACK_INT*    src
    )
{
    int32u  val;

    val  = src->value[0];
    val <<= 8;
    val |= src->value[1];
    val <<= 8;
    val |= src->value[2];
    if (val  & 0x00800000UL)
        val |= 0xFF800000UL;
    return (val);
}

/*
=======================================
    合成压缩的时间类型
=======================================
*/
CR_API bool_t
pack_date (
  __CR_OT__ sPACK_DATE*     dst,
  __CR_IN__ const byte_t    src[5]
    )
{
    if (src[0] > 15)    /* 年 */
        return (FALSE);
    if (src[1] > 12)    /* 月 */
        return (FALSE);
    if (src[2] > 31)    /* 日 */
        return (FALSE);
    if (src[3] > 23)    /* 时 */
        return (FALSE);
    if (src[4] > 59)    /* 分 */
        return (FALSE);
    dst->date[0]  = (byte_t)(src[0] << 4);
    dst->date[0] |= (byte_t)(src[1] >> 0);
    dst->date[1]  = (byte_t)(src[2] << 3);
    dst->date[1] |= (byte_t)(src[3] >> 2);
    dst->date[2]  = (byte_t)(src[3] << 6);
    dst->date[2] |= (byte_t)(src[4] >> 0);
    return (TRUE);
}

/*
=======================================
    分解压缩的时间类型
=======================================
*/
CR_API bool_t
unpack_date (
  __CR_OT__ byte_t              dst[5],
  __CR_IN__ const sPACK_DATE*   src
    )
{
    dst[0]  = (byte_t)((src->date[0]) >> 4);
    dst[1]  = (byte_t)((src->date[0]) & 15);
    dst[2]  = (byte_t)((src->date[1]) >> 3);
    dst[3]  = (byte_t)((src->date[1]  &  7) << 2);
    dst[3] |= (byte_t)((src->date[2]) >> 6);
    dst[4]  = (byte_t)((src->date[2]) & 0x3F);
    if (dst[1] > 12)    /* 月 */
        return (FALSE);
    if (dst[3] > 23)    /* 时 */
        return (FALSE);
    if (dst[4] > 59)    /* 分 */
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    合成压缩的起止时间类型
=======================================
*/
CR_API bool_t
pack_beg_end (
  __CR_OT__ sPACK_BEG_END*  dst,
  __CR_IN__ const byte_t    src[11]
    )
{
    if (src[10] > 3)                    /* 保留区 */
        return (FALSE);
    if (src[0] > 127 || src[5] > 127)   /* 起止-年 */
        return (FALSE);
    if (src[1] >  12 || src[6] >  12)   /* 起止-月 */
        return (FALSE);
    if (src[2] >  31 || src[7] >  31)   /* 起止-日 */
        return (FALSE);
    if (src[3] >  23 || src[8] >  23)   /* 起止-时 */
        return (FALSE);
    if (src[4] >  59 || src[9] >  59)   /* 起止-分 */
        return (FALSE);
    dst->beg_date[0]  = (byte_t)(src[ 0] << 1);
    dst->end_date[0]  = (byte_t)(src[ 5] << 1);
    dst->beg_date[0] |= (byte_t)(src[ 1] >> 3);
    dst->end_date[0] |= (byte_t)(src[ 6] >> 3);
    dst->beg_date[1]  = (byte_t)(src[ 1] << 5);
    dst->end_date[1]  = (byte_t)(src[ 6] << 5);
    dst->beg_date[1] |= (byte_t)(src[ 2] >> 0);
    dst->end_date[1] |= (byte_t)(src[ 7] >> 0);
    dst->bet_time[0]  = (byte_t)(src[ 3] << 3);
    dst->bet_time[0] |= (byte_t)(src[ 4] >> 3);
    dst->bet_time[1]  = (byte_t)(src[ 4] << 5);
    dst->bet_time[1] |= (byte_t)(src[ 8] >> 0);
    dst->bet_time[2]  = (byte_t)(src[ 9] << 2);
    dst->bet_time[2] |= (byte_t)(src[10] >> 0);
    return (TRUE);
}

/*
=======================================
    分解压缩的起止时间类型
=======================================
*/
CR_API bool_t
unpack_beg_end (
  __CR_OT__ byte_t                  dst[11],
  __CR_IN__ const sPACK_BEG_END*    src
    )
{
    dst[0]  = (byte_t) (src->beg_date[0] >> 1);
    dst[5]  = (byte_t) (src->end_date[0] >> 1);
    dst[1]  = (byte_t)((src->beg_date[0] &  1) << 3);
    dst[6]  = (byte_t)((src->end_date[0] &  1) << 3);
    dst[1] |= (byte_t) (src->beg_date[1] >> 5);
    dst[6] |= (byte_t) (src->end_date[1] >> 5);
    dst[2]  = (byte_t) (src->beg_date[1] & 31);
    dst[7]  = (byte_t) (src->end_date[1] & 31);
    dst[3]  = (byte_t) (src->bet_time[0] >> 3);
    dst[8]  = (byte_t) (src->bet_time[1] & 31);
    dst[4]  = (byte_t)((src->bet_time[0] &  7) << 3);
    dst[4] |= (byte_t) (src->bet_time[1] >> 5);
    dst[9]  = (byte_t) (src->bet_time[2] >> 2);
    dst[10] = (byte_t) (src->bet_time[2] &  3);
    if (dst[1] > 12 || dst[6] > 12)     /* 起止-月 */
        return (FALSE);
    if (dst[3] > 23 || dst[8] > 23)     /* 起止-时 */
        return (FALSE);
    if (dst[4] > 59 || dst[9] > 59)     /* 起止-分 */
        return (FALSE);
    return (TRUE);
}

/*****************************************************************************/
/*                             序列化/反序列化                               */
/*****************************************************************************/

#if !defined(_CR_NO_WIDE_)

/*
=======================================
    准驾车型转字符串
=======================================
*/
CR_API ansi_t*
drvlic_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    ansi_t* outp;
    ansi_t* temp;

    if (size % 2 != 0)
        return (NULL);
    temp = (ansi_t*)mem_malloc(size + 1);
    if (temp == NULL)
        return (NULL);
    mem_cpy(temp, data, size);
    temp[size] = CR_AC(NIL);
    outp = str_fmtA("<%s value=\"%s\" />", memb->show, temp);
    mem_free(temp);
    return (outp);
}

/*
=======================================
    布尔类型转字符串
=======================================
*/
CR_API ansi_t*
bool_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    if (size != sizeof(byte_t))
        return (NULL);
    if (*(byte_t*)data)
        return (str_fmtA("<%s value=\"true\" />", memb->show));
    return (str_fmtA("<%s value=\"false\" />", memb->show));
}

/*
=======================================
    字节位域转字符串
=======================================
*/
CR_API ansi_t*
bin1_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    uint_t  idx;
    byte_t  vals;
    ansi_t  temp[9];

    if (size != sizeof(byte_t))
        return (NULL);
    vals = *(byte_t*)data;
    for (idx = 0; idx < 8; idx++) {
        if (vals & 0x80)
            temp[idx] = CR_AC('1');
        else
            temp[idx] = CR_AC('0');
        vals <<= 1;
    }
    temp[idx] = CR_AC(NIL);
    return (str_fmtA("<%s value=\"%s\" />", memb->show, temp));
}

/*
=======================================
    单字位域转字符串
=======================================
*/
CR_API ansi_t*
bin2_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    uint_t  idx;
    int16u  vals;
    ansi_t  temp[17];

    if (size != sizeof(int16u))
        return (NULL);
    mem_cpy(&vals, data, sizeof(int16u));
    if (memb->attr == 1)
        vals = WORD_LE(vals);
    else
    if (memb->attr == 2)
        vals = WORD_BE(vals);
    for (idx = 0; idx < 16; idx++) {
        if (vals & 0x8000)
            temp[idx] = CR_AC('1');
        else
            temp[idx] = CR_AC('0');
        vals <<= 1;
    }
    temp[idx] = CR_AC(NIL);
    return (str_fmtA("<%s value=\"%s\" />", memb->show, temp));
}

/*
=======================================
    双字位域转字符串
=======================================
*/
CR_API ansi_t*
bin4_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    uint_t  idx;
    int32u  vals;
    ansi_t  temp[33];

    if (size != sizeof(int32u))
        return (NULL);
    mem_cpy(&vals, data, sizeof(int32u));
    if (memb->attr == 1)
        vals = DWORD_LE(vals);
    else
    if (memb->attr == 2)
        vals = DWORD_BE(vals);
    for (idx = 0; idx < 32; idx++) {
        if (vals & 0x80000000UL)
            temp[idx] = CR_AC('1');
        else
            temp[idx] = CR_AC('0');
        vals <<= 1;
    }
    temp[idx] = CR_AC(NIL);
    return (str_fmtA("<%s value=\"%s\" />", memb->show, temp));
}

/*
=======================================
    版本类型转字符串
=======================================
*/
CR_API ansi_t*
vers_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    byte_t* vals;

    if (size != sizeof(sVERSION))
        return (NULL);
    vals = (byte_t*)data;
    return (str_fmtA("<%s value=\"%u.%u.%u\" />", memb->show,
                            vals[0], vals[1], vals[2]));
}

/*
=======================================
    日期类型转字符串
=======================================
*/
CR_API ansi_t*
date_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    uint_t      year;
    sDATE_YMD*  vals;

    if (size != sizeof(sDATE_YMD))
        return (NULL);
    vals = (sDATE_YMD*)data;
    year = (uint_t)vals->year + 2000;
    return (str_fmtA("<%s value=\"%04u-%02u-%02u\" />", memb->show,
                            year, vals->month, vals->day));
}

/*
=======================================
    时间类型转字符串
=======================================
*/
CR_API ansi_t*
time_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    sDATE_HM*   vals;

    if (size != sizeof(sDATE_HM))
        return (NULL);
    vals = (sDATE_HM*)data;
    return (str_fmtA("<%s value=\"%02u:%02u:00\" />", memb->show,
                            vals->hour, vals->minute));
}

/*
=======================================
    日期时间转字符串
=======================================
*/
CR_API ansi_t*
dttm_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    uint_t          year;
    sDATE_YMDHM*    vals;

    if (size != sizeof(sDATE_YMDHM))
        return (NULL);
    vals = (sDATE_YMDHM*)data;
    year = (uint_t)vals->year + 2000;
    return (str_fmtA("<%s value=\"%04u-%02u-%02u" DTM_TM_SSEP
                "%02u:%02u:00\" />", memb->show, year, vals->month,
                    vals->day, vals->hour, vals->minute));
}

/*
=======================================
    时钟时间转字符串
=======================================
*/
CR_API ansi_t*
drtc_t_bin2str (
  __CR_IN__ const void_t*       data,
  __CR_IN__ leng_t              size,
  __CR_IN__ const sSRZ_MEMB*    memb
    )
{
    uint_t      year;
    sDATE_RTC*  vals;

    if (size != sizeof(sDATE_RTC))
        return (NULL);
    vals = (sDATE_RTC*)data;
    year = (uint_t)bcd2bin(vals->year) + 2000;
    return (str_fmtA("<%s value=\"%04u-%02x-%02x" DTM_TM_SSEP
                "%02x:%02x:%02x\" />", memb->show, year, vals->month,
                 vals->day, vals->hour, vals->minute, vals->second));
}

/*
=======================================
    字符串转准驾车型
=======================================
*/
CR_API bool_t
drvlic_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    leng_t  idx;
    ansi_t* temps;
    wide_t* value;

    CR_NOUSE(attr);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    temps = utf16_to_local(cpage, value);
    mem_free(value);
    if (temps == NULL)
        return (FALSE);
    index = str_lenA(temps);
    if (index > size || index % 2 != 0)
        goto _failure;
    for (idx = 0; idx < index; idx += 2) {
        if ((!is_alphaA(temps[idx + 0])) ||
            (!is_alnumA(temps[idx + 1]) && temps[idx + 1] != CR_AC(' ')))
            goto _failure;
    }
    mem_zero(data, size);
    if (index != 0)
        mem_cpy(data, temps, index);
    mem_free(temps);
    return (TRUE);

_failure:
    mem_free(temps);
    return (FALSE);
}

/*
=======================================
    字符串转布尔类型
=======================================
*/
CR_API bool_t
bool_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    wide_t* value;

    CR_NOUSE(cpage);
    CR_NOUSE(attr); CR_NOUSE(index);
    if (size != sizeof(byte_t))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    if (str_cmpIW(value, CR_WS("true")) == 0) {
        *(byte_t*)data = TRUE;
    }
    else
    if (str_cmpIW(value, CR_WS("false")) == 0) {
        *(byte_t*)data = FALSE;
    }
    else {
        mem_free(value);
        return (FALSE);
    }
    mem_free(value);
    return (TRUE);
}

/*
=======================================
    字符串转字节位域
=======================================
*/
CR_API bool_t
bin1_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    wide_t* value;

    CR_NOUSE(attr); CR_NOUSE(cpage);
    if (size != sizeof(byte_t))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    *(byte_t*)data = (byte_t)str2binW(value, &index);
    mem_free(value);
    return ((index != 8) ? FALSE : TRUE);
}

/*
=======================================
    字符串转单字位域
=======================================
*/
CR_API bool_t
bin2_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    int16u  copys;
    wide_t* value;

    CR_NOUSE(cpage);
    if (size != sizeof(int16u))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    copys = (int16u)str2binW(value, &index);
    mem_free(value);
    if (index != 16)
        return (FALSE);
    if (attr == 1)
        copys = WORD_LE(copys);
    else
    if (attr == 2)
        copys = WORD_BE(copys);
    mem_cpy(data, &copys, sizeof(int16u));
    return (TRUE);
}

/*
=======================================
    字符串转双字位域
=======================================
*/
CR_API bool_t
bin4_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    int32u  copys;
    wide_t* value;

    CR_NOUSE(cpage);
    if (size != sizeof(int32u))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    copys = (int32u)str2bin32W(value, &index);
    mem_free(value);
    if (index != 32)
        return (FALSE);
    if (attr == 1)
        copys = DWORD_LE(copys);
    else
    if (attr == 2)
        copys = DWORD_BE(copys);
    mem_cpy(data, &copys, sizeof(int32u));
    return (TRUE);
}

/*
=======================================
    字符串转版本类型
=======================================
*/
CR_API bool_t
vers_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    uint_t      idx;
    uint_t      tp[3];
    wide_t*     value;
    wide_t**    lists;

    CR_NOUSE(attr); CR_NOUSE(cpage);
    if (size != sizeof(sVERSION))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    lists = str_splitW(value, CR_WC('.'), &index);
    if (lists == NULL || index != 3)
        goto _failure;
    for (idx = 0; idx < 3; idx++) {
        tp[idx] = (uint_t)str2intW(lists[idx], &index);
        if (!is_digitW((lists[idx])[0]) ||
            (lists[idx])[index] != CR_NC(NIL) ||
            index > 3 || index == 0 || tp[idx] > 255)
            goto _failure;
    }
    mem_free(lists);
    mem_free(value);
    ((byte_t*)data)[0] = (byte_t)tp[0];
    ((byte_t*)data)[1] = (byte_t)tp[1];
    ((byte_t*)data)[2] = (byte_t)tp[2];
    return (TRUE);

_failure:
    TRY_FREE(lists);
    mem_free(value);
    return (FALSE);
}

/*
=======================================
    字符串转日期类型
=======================================
*/
CR_API bool_t
date_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    wide_t*     value;
    sDATETIME   temps;
    sDATE_YMD*  copys;

    CR_NOUSE(cpage);
    CR_NOUSE(attr); CR_NOUSE(index);
    if (size != sizeof(sDATE_YMD))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    if (!str2datetimeW(&temps, value, CR_NC(DTM_TM_CSEP))) {
        mem_free(value);
        return (FALSE);
    }
    mem_free(value);
    if (temps.year < 2000 || temps.year > 2255)
        return (FALSE);
    temps.year -= 2000;
    copys = (sDATE_YMD*)data;
    copys->year  = (byte_t)(temps.year);
    copys->month = (byte_t)(temps.month);
    copys->day   = (byte_t)(temps.day);
    return (TRUE);
}

/*
=======================================
    字符串转时间类型
=======================================
*/
CR_API bool_t
time_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    wide_t      tp[20];
    wide_t*     value;
    sDATE_HM*   copys;
    sDATETIME   temps;

    CR_NOUSE(cpage);
    CR_NOUSE(attr); CR_NOUSE(index);
    if (size != sizeof(sDATE_HM))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    index = str_lenW(value);
    if (index > 8) {
        mem_free(value);
        return (FALSE);
    }
    str_catW(str_cpyW(tp, CR_WS("1983-08-10@")), value);
    mem_free(value);
    if (!str2datetimeW(&temps, tp, CR_WC('@')))
        return (FALSE);
    copys = (sDATE_HM*)data;
    copys->hour   = (byte_t)(temps.hour);
    copys->minute = (byte_t)(temps.minute);
    return (TRUE);
}

/*
=======================================
    字符串转日期时间
=======================================
*/
CR_API bool_t
dttm_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    wide_t*         value;
    sDATETIME       temps;
    sDATE_YMDHM*    copys;

    CR_NOUSE(cpage);
    CR_NOUSE(attr); CR_NOUSE(index);
    if (size != sizeof(sDATE_YMDHM))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    if (!str2datetimeW(&temps, value, CR_NC(DTM_TM_CSEP))) {
        mem_free(value);
        return (FALSE);
    }
    mem_free(value);
    if (temps.year < 2000 || temps.year > 2255)
        return (FALSE);
    temps.year -= 2000;
    copys = (sDATE_YMDHM*)data;
    copys->year   = (byte_t)(temps.year);
    copys->month  = (byte_t)(temps.month);
    copys->day    = (byte_t)(temps.day);
    copys->hour   = (byte_t)(temps.hour);
    copys->minute = (byte_t)(temps.minute);
    return (TRUE);
}

/*
=======================================
    字符串转时钟时间
=======================================
*/
CR_API bool_t
drtc_t_str2bin (
  __CR_IO__ void_t*     data,
  __CR_IN__ leng_t      size,
  __CR_IN__ uint_t      attr,
  __CR_IN__ leng_t      index,
  __CR_IN__ uint_t      cpage,
  __CR_IN__ sXNODEw*    node
    )
{
    wide_t*     value;
    sDATETIME   temps;
    sDATE_RTC*  copys;

    CR_NOUSE(cpage);
    CR_NOUSE(attr); CR_NOUSE(index);
    if (size != sizeof(sDATE_RTC))
        return (FALSE);
    value = xml_attr_stringW(CR_WS("value"), node);
    if (value == NULL)
        return (FALSE);
    if (!str2datetimeW(&temps, value, CR_NC(DTM_TM_CSEP))) {
        mem_free(value);
        return (FALSE);
    }
    mem_free(value);
    if (temps.year < 2000 || temps.year > 2099)
        return (FALSE);
    temps.year -= 2000;
    copys = (sDATE_RTC*)data;
    copys->year   = bin2bcd((byte_t)temps.year);
    copys->month  = bin2bcd((byte_t)temps.month);
    copys->day    = bin2bcd((byte_t)temps.day);
    copys->hour   = bin2bcd((byte_t)temps.hour);
    copys->minute = bin2bcd((byte_t)temps.minute);
    copys->second = bin2bcd((byte_t)temps.second);
    return (TRUE);
}

#endif  /* !_CR_NO_WIDE_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
