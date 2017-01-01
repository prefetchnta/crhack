/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-28  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 文件读写接口内部头文件 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DATIO_H__
#define __CR_DATIO_H__

#include "fileio.h"

/* 内存文件读取接口 */
typedef struct
{
        /* 通用部分 */
        iDATIN  datin;

        /* 个性部分 */
        bool_t          m_rel;  /* 析构时是否释放 */
        dist_t          m_pos;  /* 当前输入的位置 */
        const byte_t*   m_mem;  /* 内存缓冲区指针 */
        const byte_t*   m_ptr;  /* 真正读取的指针 */

        /* 内部使用 */
        dist_t  m_head;     /* 逻辑起始位置 (内部使用) */
        dist_t  m_tail;     /* 逻辑结束位置 (内部使用) */
        dist_t  m_size;     /* 数据逻辑大小 (内部使用) */

} iMEMIN;

/* 读取接口的共用函数 */
byte_t* iDatIN_get (iDATIN *that, leng_t *size, bool_t rel);
bool_t  iDatIN_seek64 (iDATIN *that, int64s offset, uint_t whence);
bool_t  iDatIN_check (iDATIN *that, void_t *temp, const ansi_t *tag,
                      uint_t size);
bool_t  iDatIN_setArea (iDATIN *that, fdist_t head, fdist_t tail);

/* 写入接口的共用函数 */
bool_t  iDatOT_putsA (iDATOT *that, const ansi_t *str, leng_t len);
bool_t  iDatOT_putsW (iDATOT *that, const wide_t *str, leng_t len);

/* 内存文件读取接口的共用函数声明 */
fsize_t iMemIN_tell (iDATIN *that);
bool_t  iMemIN_seek (iDATIN *that, fdist_t offset, uint_t whence);
bool_t  iMemIN_isEOF (iDATIN *that);
bool_t  iMemIN_rewind (iDATIN *that);
bool_t  iMemIN_getb_no (iDATIN *that, byte_t *val);
bool_t  iMemIN_getw_le (iDATIN *that, int16u *val);
bool_t  iMemIN_getd_le (iDATIN *that, int32u *val);
bool_t  iMemIN_getq_le (iDATIN *that, int64u *val);
bool_t  iMemIN_getw_be (iDATIN *that, int16u *val);
bool_t  iMemIN_getd_be (iDATIN *that, int32u *val);
bool_t  iMemIN_getq_be (iDATIN *that, int64u *val);
leng_t  iMemIN_read (iDATIN *that, void_t *buffer, leng_t size);
bool_t  iMemIN_setArea (iDATIN *that, fdist_t head, fdist_t tail);

#endif  /* !__CR_DATIO_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
