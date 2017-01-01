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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 磁盘文件写入接口实现 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "datio.h"
#include "memlib.h"
#include "strlib.h"

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iDATOT  datot;

        /* 个性部分 */
        file_t  m_fpp;  /* 文件句柄 */
        fsize_t m_pos;  /* 指针位置 */

} iDISKOT;

#if defined(_CR_ORDER_LE_)
    #define iDiskOT_putq_no     iDiskOT_putq_le
#else   /* (_CR_ORDER_BE_) */
    #define iDiskOT_putq_no     iDiskOT_putq_be
#endif

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iDiskOT_release (
  __CR_IN__ iDATOT* that
    )
{
    iDISKOT*    real;

    real = (iDISKOT*)that;
    file_close(real->m_fpp);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iDiskOT_getMore (
  __CR_IN__ iDATOT* that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iDATOT::DISK") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    写一个字节
---------------------------------------
*/
static bool_t
iDiskOT_putb_no (
  __CR_IO__ iDATOT* that,
  __CR_IN__ byte_t  val
    )
{
    leng_t      wrt;
    bool_t      ret = TRUE;
    iDISKOT*    real = (iDISKOT*)that;

    wrt = file_write(&val, 1, real->m_fpp);
    if (wrt != 1) ret = FALSE;
    real->m_pos += wrt;
    if (that->__size__ < real->m_pos)
        that->__size__ = real->m_pos;
    return (ret);
}

/*
---------------------------------------
    写一个单字LE
---------------------------------------
*/
static bool_t
iDiskOT_putw_le (
  __CR_IO__ iDATOT* that,
  __CR_IN__ int16u  val
    )
{
    leng_t      wrt;
    bool_t      ret = TRUE;
    iDISKOT*    real = (iDISKOT*)that;

    val = WORD_LE(val);
    wrt = file_write(&val, 2, real->m_fpp);
    if (wrt != 2) ret = FALSE;
    real->m_pos += wrt;
    if (that->__size__ < real->m_pos)
        that->__size__ = real->m_pos;
    return (ret);
}

/*
---------------------------------------
    写一个双字LE
---------------------------------------
*/
static bool_t
iDiskOT_putd_le (
  __CR_IO__ iDATOT* that,
  __CR_IN__ int32u  val
    )
{
    leng_t      wrt;
    bool_t      ret = TRUE;
    iDISKOT*    real = (iDISKOT*)that;

    val = DWORD_LE(val);
    wrt = file_write(&val, 4, real->m_fpp);
    if (wrt != 4) ret = FALSE;
    real->m_pos += wrt;
    if (that->__size__ < real->m_pos)
        that->__size__ = real->m_pos;
    return (ret);
}

/*
---------------------------------------
    写一个四字LE
---------------------------------------
*/
static bool_t
iDiskOT_putq_le (
  __CR_IO__ iDATOT* that,
  __CR_IN__ int64u  val
    )
{
    leng_t      wrt;
    bool_t      ret = TRUE;
    iDISKOT*    real = (iDISKOT*)that;

    val = QWORD_LE(val);
    wrt = file_write(&val, 8, real->m_fpp);
    if (wrt != 8) ret = FALSE;
    real->m_pos += wrt;
    if (that->__size__ < real->m_pos)
        that->__size__ = real->m_pos;
    return (ret);
}

/*
---------------------------------------
    写一个单字BE
---------------------------------------
*/
static bool_t
iDiskOT_putw_be (
  __CR_IO__ iDATOT* that,
  __CR_IN__ int16u  val
    )
{
    leng_t      wrt;
    bool_t      ret = TRUE;
    iDISKOT*    real = (iDISKOT*)that;

    val = WORD_BE(val);
    wrt = file_write(&val, 2, real->m_fpp);
    if (wrt != 2) ret = FALSE;
    real->m_pos += wrt;
    if (that->__size__ < real->m_pos)
        that->__size__ = real->m_pos;
    return (ret);
}

/*
---------------------------------------
    写一个双字BE
---------------------------------------
*/
static bool_t
iDiskOT_putd_be (
  __CR_IO__ iDATOT* that,
  __CR_IN__ int32u  val
    )
{
    leng_t      wrt;
    bool_t      ret = TRUE;
    iDISKOT*    real = (iDISKOT*)that;

    val = DWORD_BE(val);
    wrt = file_write(&val, 4, real->m_fpp);
    if (wrt != 4) ret = FALSE;
    real->m_pos += wrt;
    if (that->__size__ < real->m_pos)
        that->__size__ = real->m_pos;
    return (ret);
}

/*
---------------------------------------
    写一个四字BE
---------------------------------------
*/
static bool_t
iDiskOT_putq_be (
  __CR_IO__ iDATOT* that,
  __CR_IN__ int64u  val
    )
{
    leng_t      wrt;
    bool_t      ret = TRUE;
    iDISKOT*    real = (iDISKOT*)that;

    val = QWORD_BE(val);
    wrt = file_write(&val, 8, real->m_fpp);
    if (wrt != 8) ret = FALSE;
    real->m_pos += wrt;
    if (that->__size__ < real->m_pos)
        that->__size__ = real->m_pos;
    return (ret);
}

/*
---------------------------------------
    写入一组零
---------------------------------------
*/
static bool_t
iDiskOT_zero (
  __CR_IO__ iDATOT* that,
  __CR_IN__ leng_t  size
    )
{
    leng_t  blk = (leng_t)(size / sizeof(int64u));
    uint_t  rst = (uint_t)(size % sizeof(int64u));

    for (; blk != 0; blk--) {
        if (!iDiskOT_putq_no(that, 0))
            return (FALSE);
    }
    for (; rst != 0; rst--) {
        if (!iDiskOT_putb_no(that, 0))
            return (FALSE);
    }
    return (TRUE);
}

/*
---------------------------------------
    写入一组数据
---------------------------------------
*/
static leng_t
iDiskOT_write (
  __CR_IO__ iDATOT*         that,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    leng_t      temp;
    iDISKOT*    real = (iDISKOT*)that;

    temp = file_write(data, size, real->m_fpp);
    real->m_pos += temp;
    if (that->__size__ < real->m_pos)
        that->__size__ = real->m_pos;
    return (temp);
}

/*
---------------------------------------
    刷新文件输出缓存
---------------------------------------
*/
static void_t*
iDiskOT_flush (
  __CR_IN__ iDATOT* that
    )
{
    iDISKOT*    real = (iDISKOT*)that;

    if (!file_flush(real->m_fpp))
        return (NULL);
    return (real->m_fpp);
}

/*
---------------------------------------
    重置数据指针
---------------------------------------
*/
static bool_t
iDiskOT_reput (
  __CR_IO__ iDATOT* that,
  __CR_IN__ fsize_t offset
    )
{
    iDISKOT*    real = (iDISKOT*)that;

    if (offset > that->__size__)
        return (FALSE);
    if (!file_seek(real->m_fpp, offset, SEEK_SET))
        return (FALSE);
    real->m_pos = offset;
    return (TRUE);
}

/* 接口虚函数表 */
static const iDATOT_vtbl _rom_ s_datot_vtbl =
{
    iDiskOT_release, iDiskOT_getMore, iDiskOT_putb_no,
    iDiskOT_putw_le, iDiskOT_putd_le, iDiskOT_putq_le,
    iDiskOT_putw_be, iDiskOT_putd_be, iDiskOT_putq_be,
    iDiskOT_zero, iDiskOT_write, iDatOT_putsA, iDatOT_putsW,
    iDiskOT_flush, iDiskOT_reput,
};

/*
=======================================
    生成磁盘文件写入接口A
=======================================
*/
CR_API iDATOT*
create_disk_outA (
  __CR_IN__ const ansi_t*   name
    )
{
    fbuf_t      file;
    iDISKOT*    disk;

    disk = struct_new(iDISKOT);
    if (disk == NULL)
        return (NULL);

    file = file_openA(name, CR_FO_WO);
    if (file == NULL) {
        mem_free(disk);
        return (NULL);
    }
    disk->m_pos = 0;
    disk->m_fpp = file;
    disk->datot.__size__ = 0;
    disk->datot.__vptr__ = &s_datot_vtbl;
    return ((iDATOT*)disk);
}

/*
=======================================
    生成磁盘文件写入接口W
=======================================
*/
CR_API iDATOT*
create_disk_outW (
  __CR_IN__ const wide_t*   name
    )
{
    fbuf_t      file;
    iDISKOT*    disk;

    disk = struct_new(iDISKOT);
    if (disk == NULL)
        return (NULL);

    file = file_openW(name, CR_FO_WO);
    if (file == NULL) {
        mem_free(disk);
        return (NULL);
    }
    disk->m_pos = 0;
    disk->m_fpp = file;
    disk->datot.__size__ = 0;
    disk->datot.__vptr__ = &s_datot_vtbl;
    return ((iDATOT*)disk);
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
