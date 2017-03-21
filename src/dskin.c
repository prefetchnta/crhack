/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-29  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 磁盘文件读取接口实现 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "datio.h"
#include "memlib.h"
#include "strlib.h"

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iDATIN  datin;

        /* 个性部分 */
        file_t  m_fpp;  /* 文件句柄 */
        fdist_t m_pos;  /* 指针位置 */

} iDISKIN;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iDiskIN_release (
  __CR_IN__ iDATIN* that
    )
{
    iDISKIN*    real;

    real = (iDISKIN*)that;
    file_close(real->m_fpp);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iDiskIN_getMore (
  __CR_IN__ iDATIN* that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iDATIN::DISK") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    返回指针位置
---------------------------------------
*/
static fsize_t
iDiskIN_tell (
  __CR_IN__ iDATIN* that
    )
{
    iDISKIN*    real = (iDISKIN*)that;

    return (real->m_pos - that->__head__);
}

/*
---------------------------------------
    读取指针定位
---------------------------------------
*/
static bool_t
iDiskIN_seek (
  __CR_IO__ iDATIN* that,
  __CR_IN__ fdist_t offset,
  __CR_IN__ uint_t  whence
    )
{
    fdist_t     offset_set;
    iDISKIN*    real = (iDISKIN*)that;

    switch (whence)
    {
        case SEEK_SET:
            if (cut_addfs(&offset_set, offset, that->__head__))
                return (FALSE);
            break;

        case SEEK_CUR:
            if (offset == 0)
                return (TRUE);
            if (cut_addfs(&offset_set, offset, real->m_pos))
                return (FALSE);
            break;

        case SEEK_END:
            if (cut_addfs(&offset_set, offset, that->__tail__))
                return (FALSE);
            break;

        default:
            return (FALSE);
    }

    /* 过滤偏移 */
    if (offset_set < that->__head__)
        offset_set = that->__head__;
    else
    if (offset_set > that->__tail__)
        offset_set = that->__tail__;
    if (!file_seek(real->m_fpp, offset_set, SEEK_SET))
        return (FALSE);
    real->m_pos = offset_set;
    return (TRUE);
}

/*
---------------------------------------
    是否处于文件尾部
---------------------------------------
*/
static bool_t
iDiskIN_isEOF (
  __CR_IN__ iDATIN* that
    )
{
    iDISKIN*    real = (iDISKIN*)that;

    return ((real->m_pos >= that->__tail__) ? TRUE : FALSE);
}

/*
---------------------------------------
    重新定位读取指针
---------------------------------------
*/
static bool_t
iDiskIN_rewind (
  __CR_IO__ iDATIN* that
    )
{
    iDISKIN*    real = (iDISKIN*)that;

    if (!file_seek(real->m_fpp, that->__head__, SEEK_SET))
        return (FALSE);
    real->m_pos = that->__head__;
    return (TRUE);
}

/*
---------------------------------------
    读取一个字节
---------------------------------------
*/
static bool_t
iDiskIN_getb_no (
  __CR_IO__ iDATIN* that,
  __CR_OT__ byte_t* val
    )
{
    leng_t      red;
    iDISKIN*    real = (iDISKIN*)that;

    if (real->m_pos > that->__tail__ - 1) {
        real->m_pos = that->__tail__;
        file_seek(real->m_fpp, real->m_pos, SEEK_SET);
        return (FALSE);
    }

    red = file_read(val, 1, real->m_fpp);
    real->m_pos += red;
    if (red != 1)
        return (FALSE);
    return (TRUE);
}

/*
---------------------------------------
    读取一个单字LE
---------------------------------------
*/
static bool_t
iDiskIN_getw_le (
  __CR_IO__ iDATIN* that,
  __CR_OT__ int16u* val
    )
{
    leng_t      red;
    iDISKIN*    real = (iDISKIN*)that;

    if (real->m_pos > that->__tail__ - 2) {
        real->m_pos = that->__tail__;
        file_seek(real->m_fpp, real->m_pos, SEEK_SET);
        return (FALSE);
    }

    red = file_read(val, 2, real->m_fpp);
    real->m_pos += red;
    if (red != 2)
        return (FALSE);
    *val = WORD_LE(*val);
    return (TRUE);
}

/*
---------------------------------------
    读取一个双字LE
---------------------------------------
*/
static bool_t
iDiskIN_getd_le (
  __CR_IO__ iDATIN* that,
  __CR_OT__ int32u* val
    )
{
    leng_t      red;
    iDISKIN*    real = (iDISKIN*)that;

    if (real->m_pos > that->__tail__ - 4) {
        real->m_pos = that->__tail__;
        file_seek(real->m_fpp, real->m_pos, SEEK_SET);
        return (FALSE);
    }

    red = file_read(val, 4, real->m_fpp);
    real->m_pos += red;
    if (red != 4)
        return (FALSE);
    *val = DWORD_LE(*val);
    return (TRUE);
}

/*
---------------------------------------
    读取一个四字LE
---------------------------------------
*/
static bool_t
iDiskIN_getq_le (
  __CR_IO__ iDATIN* that,
  __CR_OT__ int64u* val
    )
{
    leng_t      red;
    iDISKIN*    real = (iDISKIN*)that;

    if (real->m_pos > that->__tail__ - 8) {
        real->m_pos = that->__tail__;
        file_seek(real->m_fpp, real->m_pos, SEEK_SET);
        return (FALSE);
    }

    red = file_read(val, 8, real->m_fpp);
    real->m_pos += red;
    if (red != 8)
        return (FALSE);
    *val = QWORD_LE(*val);
    return (TRUE);
}

/*
---------------------------------------
    读取一个单字BE
---------------------------------------
*/
static bool_t
iDiskIN_getw_be (
  __CR_IO__ iDATIN* that,
  __CR_OT__ int16u* val
    )
{
    leng_t      red;
    iDISKIN*    real = (iDISKIN*)that;

    if (real->m_pos > that->__tail__ - 2) {
        real->m_pos = that->__tail__;
        file_seek(real->m_fpp, real->m_pos, SEEK_SET);
        return (FALSE);
    }

    red = file_read(val, 2, real->m_fpp);
    real->m_pos += red;
    if (red != 2)
        return (FALSE);
    *val = WORD_BE(*val);
    return (TRUE);
}

/*
---------------------------------------
    读取一个双字BE
---------------------------------------
*/
static bool_t
iDiskIN_getd_be (
  __CR_IO__ iDATIN* that,
  __CR_OT__ int32u* val
    )
{
    leng_t      red;
    iDISKIN*    real = (iDISKIN*)that;

    if (real->m_pos > that->__tail__ - 4) {
        real->m_pos = that->__tail__;
        file_seek(real->m_fpp, real->m_pos, SEEK_SET);
        return (FALSE);
    }

    red = file_read(val, 4, real->m_fpp);
    real->m_pos += red;
    if (red != 4)
        return (FALSE);
    *val = DWORD_BE(*val);
    return (TRUE);
}

/*
---------------------------------------
    读取一个四字BE
---------------------------------------
*/
static bool_t
iDiskIN_getq_be (
  __CR_IO__ iDATIN* that,
  __CR_OT__ int64u* val
    )
{
    leng_t      red;
    iDISKIN*    real = (iDISKIN*)that;

    if (real->m_pos > that->__tail__ - 8) {
        real->m_pos = that->__tail__;
        file_seek(real->m_fpp, real->m_pos, SEEK_SET);
        return (FALSE);
    }

    red = file_read(val, 8, real->m_fpp);
    real->m_pos += red;
    if (red != 8)
        return (FALSE);
    *val = QWORD_BE(*val);
    return (TRUE);
}

/*
---------------------------------------
    读取一组数据
---------------------------------------
*/
static leng_t
iDiskIN_read (
  __CR_IO__ iDATIN* that,
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    leng_t      temp;
    iDISKIN*    real = (iDISKIN*)that;

    if (size > (fsize_t)that->__tail__ ||
        real->m_pos > (fdist_t)(that->__tail__ - size))
        size = (leng_t)(that->__tail__ - real->m_pos);

    temp = file_read(data, size, real->m_fpp);
    real->m_pos += temp;
    return (temp);
}

/* 接口虚函数表 */
static const iDATIN_vtbl _rom_ s_datin_vtbl =
{
    iDiskIN_release, iDiskIN_getMore,
    iDiskIN_tell, iDiskIN_seek, iDatIN_seek64,
    iDiskIN_isEOF, iDiskIN_rewind, iDiskIN_getb_no,
    iDiskIN_getw_le, iDiskIN_getd_le, iDiskIN_getq_le,
    iDiskIN_getw_be, iDiskIN_getd_be, iDiskIN_getq_be,
    iDatIN_get, iDiskIN_read, iDatIN_check, iDatIN_setArea,
};

/*
=======================================
    生成磁盘文件读取接口A
=======================================
*/
CR_API iDATIN*
create_disk_inA (
  __CR_IN__ const ansi_t*   name
    )
{
    fbuf_t      file;
    fsize_t     size;
    iDISKIN*    disk;

    disk = struct_new(iDISKIN);
    if (disk == NULL)
        return (NULL);

    file = file_openA(name, CR_FO_RO);
    if (file == NULL)
        goto _failure2;

    size = file_size(file);
    if (size == CR_F_ERROR)
        goto _failure1;

    if ((fdist_t)size <= 0)
        goto _failure1;
    disk->m_pos = 0;
    disk->m_fpp = file;
    disk->datin.__head__ = 0;
    disk->datin.__tail__ = size;
    disk->datin.__size__ = size;
    disk->datin.__vptr__ = &s_datin_vtbl;
    return ((iDATIN*)disk);

_failure1:
    file_close(file);
_failure2:
    mem_free(disk);
    return (NULL);
}

#if !defined(_CR_NAKED_NO_WSTRING_)

/*
=======================================
    生成磁盘文件读取接口W
=======================================
*/
CR_API iDATIN*
create_disk_inW (
  __CR_IN__ const wide_t*   name
    )
{
    fbuf_t      file;
    fsize_t     size;
    iDISKIN*    disk;

    disk = struct_new(iDISKIN);
    if (disk == NULL)
        return (NULL);

    file = file_openW(name, CR_FO_RO);
    if (file == NULL)
        goto _failure2;

    size = file_size(file);
    if (size == CR_F_ERROR)
        goto _failure1;

    if ((fdist_t)size <= 0)
        goto _failure1;
    disk->m_pos = 0;
    disk->m_fpp = file;
    disk->datin.__head__ = 0;
    disk->datin.__tail__ = size;
    disk->datin.__size__ = size;
    disk->datin.__vptr__ = &s_datin_vtbl;
    return ((iDATIN*)disk);

_failure1:
    file_close(file);
_failure2:
    mem_free(disk);
    return (NULL);
}

#endif  /* !_CR_NAKED_NO_WSTRING_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
