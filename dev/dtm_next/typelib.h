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
/*  >>>>>>>>>>>>>>>>>>>>>>> DTM_NEXT 类型操作头文件 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_TYPELIB_H__
#define __CR_TYPELIB_H__

#include "util/totcfg.h"

/*****************************************************************************/
/*                               扩展格式转换                                */
/*****************************************************************************/

/* 用到的扩展类型 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* 日期 - 年月日 */
CR_TYPEDEF struct
{
        byte_t  year;       /* 年 */
        byte_t  month;      /* 月 */
        byte_t  day;        /* 日 */

} CR_PACKED sDATE_YMD;

/* 时间 - 时分 */
CR_TYPEDEF struct
{
        byte_t  hour;       /* 时 */
        byte_t  minute;     /* 分 */

} CR_PACKED sDATE_HM;

/* 日期时间 - 年月日时分 */
CR_TYPEDEF struct
{
        byte_t  year;       /* 年 */
        byte_t  month;      /* 月 */
        byte_t  day;        /* 日 */
        byte_t  hour;       /* 时 */
        byte_t  minute;     /* 分 */

} CR_PACKED sDATE_YMDHM;

/* 时钟时间 - 年月日时分秒 (BCD) */
CR_TYPEDEF struct
{
        byte_t  year;       /* 年 */
        byte_t  month;      /* 月 */
        byte_t  day;        /* 日 */
        byte_t  hour;       /* 时 */
        byte_t  minute;     /* 分 */
        byte_t  second;     /* 秒 */

} CR_PACKED sDATE_RTC;

/* 版本号类型 */
CR_TYPEDEF struct
{
        byte_t  vers[3];    /* 格式为 255.255.255 */

} CR_PACKED sVERSION;

/* 压缩的整数类型 */
CR_TYPEDEF struct
{
        byte_t  value[3];   /* 四个字节压缩成三个字节 (老版本用)
                               部分合计里程和合计费用使用此结构 */
} CR_PACKED sPACK_INT;

/* 压缩的时间类型 */
CR_TYPEDEF struct
{
        byte_t  date[3];    /* 五个字节压缩成三个字节, 年月日时分
                               格式为 YYYYMMMM DDDDDHHH HHMMMMMM */
} CR_PACKED sPACK_DATE;

/* 压缩的起止时间类型 */
CR_TYPEDEF struct
{
        byte_t  beg_date[2];    /* 三个字节压缩成两个字节, 年月日
                                   格式为 YYYYYYYM MMMDDDDD */
        byte_t  end_date[2];    /* 三个字节压缩成两个字节, 年月日
                                   格式为 YYYYYYYM MMMDDDDD */
        byte_t  bet_time[3];    /* 四个字节压缩成三个字节, 起始 + 中止时分
                                   格式为 HHHHHMMM MMMEEEEE TTTTTTSS
                                   SS 为保留位域 */
} CR_PACKED sPACK_BEG_END;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

CR_API bool_t   pack_int (sPACK_INT *dst, int32s src);
CR_API int32s   unpack_int (const sPACK_INT *src);
CR_API bool_t   pack_date (sPACK_DATE *dst, const byte_t src[5]);
CR_API bool_t   unpack_date (byte_t dst[5], const sPACK_DATE *src);
CR_API bool_t   pack_beg_end (sPACK_BEG_END *dst, const byte_t src[11]);
CR_API bool_t   unpack_beg_end (byte_t dst[11], const sPACK_BEG_END *src);

/*****************************************************************************/
/*                             序列化/反序列化                               */
/*****************************************************************************/

/***** 序列化组 *****/
CR_API ansi_t*  drvlic_bin2str (const void_t *data, leng_t size,
                                const sSRZ_MEMB *memb);
CR_API ansi_t*  bool_t_bin2str (const void_t *data, leng_t size,
                                const sSRZ_MEMB *memb);
CR_API ansi_t*  bin1_t_bin2str (const void_t *data, leng_t size,
                                const sSRZ_MEMB *memb);
CR_API ansi_t*  bin2_t_bin2str (const void_t *data, leng_t size,
                                const sSRZ_MEMB *memb);
CR_API ansi_t*  bin4_t_bin2str (const void_t *data, leng_t size,
                                const sSRZ_MEMB *memb);
CR_API ansi_t*  vers_t_bin2str (const void_t *data, leng_t size,
                                const sSRZ_MEMB *memb);
CR_API ansi_t*  date_t_bin2str (const void_t *data, leng_t size,
                                const sSRZ_MEMB *memb);
CR_API ansi_t*  time_t_bin2str (const void_t *data, leng_t size,
                                const sSRZ_MEMB *memb);
CR_API ansi_t*  dttm_t_bin2str (const void_t *data, leng_t size,
                                const sSRZ_MEMB *memb);
CR_API ansi_t*  drtc_t_bin2str (const void_t *data, leng_t size,
                                const sSRZ_MEMB *memb);
/***** 反序列化组 *****/
CR_API bool_t   drvlic_str2bin (void_t *data, leng_t size, uint_t attr,
                                leng_t index, uint_t cpage, sXNODEw *node);
CR_API bool_t   bool_t_str2bin (void_t *data, leng_t size, uint_t attr,
                                leng_t index, uint_t cpage, sXNODEw *node);
CR_API bool_t   bin1_t_str2bin (void_t *data, leng_t size, uint_t attr,
                                leng_t index, uint_t cpage, sXNODEw *node);
CR_API bool_t   bin2_t_str2bin (void_t *data, leng_t size, uint_t attr,
                                leng_t index, uint_t cpage, sXNODEw *node);
CR_API bool_t   bin4_t_str2bin (void_t *data, leng_t size, uint_t attr,
                                leng_t index, uint_t cpage, sXNODEw *node);
CR_API bool_t   vers_t_str2bin (void_t *data, leng_t size, uint_t attr,
                                leng_t index, uint_t cpage, sXNODEw *node);
CR_API bool_t   date_t_str2bin (void_t *data, leng_t size, uint_t attr,
                                leng_t index, uint_t cpage, sXNODEw *node);
CR_API bool_t   time_t_str2bin (void_t *data, leng_t size, uint_t attr,
                                leng_t index, uint_t cpage, sXNODEw *node);
CR_API bool_t   dttm_t_str2bin (void_t *data, leng_t size, uint_t attr,
                                leng_t index, uint_t cpage, sXNODEw *node);
CR_API bool_t   drtc_t_str2bin (void_t *data, leng_t size, uint_t attr,
                                leng_t index, uint_t cpage, sXNODEw *node);

#endif  /* !__CR_TYPELIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
