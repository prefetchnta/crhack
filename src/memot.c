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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 内存文件写入接口实现 <<<<<<<<<<<<<<<<<<<<<  */
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
        uchar*  m_buf;      /* 内存缓冲区指针 */
        leng_t  m_pos;      /* 当前输出的位置 */
        leng_t  m_size;     /* 内存缓冲区大小 */

} iMEMOT;

/*
---------------------------------------
    内存缓冲区扩容
---------------------------------------
*/
static bool_t
iMemOT_append (
  __CR_IO__ iMEMOT* that,
  __CR_IN__ leng_t  unit_size
    )
{
    uchar*  temp;
    leng_t  size;

    /* 超出大小时重置大小 */
    if (that->m_size < that->m_pos + unit_size) {
        size = that->m_size * 2 + unit_size;
        temp = (uchar*)mem_realloc(that->m_buf, size);
        if (temp == NULL)
            return (FALSE);
        that->m_buf  = temp;
        that->m_size = size;
    }

    /* 重置文件大小 (注意有 reput 功能) */
    if (that->datot.__size__ < that->m_pos + unit_size)
        that->datot.__size__ = that->m_pos + unit_size;
    return (TRUE);
}

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iMemOT_release (
  __CR_IN__ iDATOT* that
    )
{
    iMEMOT* real;

    real = (iMEMOT*)that;
    mem_free(real->m_buf);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iMemOT_getMore (
  __CR_IN__ iDATOT* that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iDATOT::MEM") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    写一个字节
---------------------------------------
*/
static bool_t
iMemOT_putb_no (
  __CR_IO__ iDATOT* that,
  __CR_IN__ byte_t  val
    )
{
    iMEMOT* real = (iMEMOT*)that;

    if (!iMemOT_append(real, 1))
        return (FALSE);
    real->m_buf[real->m_pos] = val;
    real->m_pos += sizeof(byte_t);
    return (TRUE);
}

/*
---------------------------------------
    写一个单字LE
---------------------------------------
*/
static bool_t
iMemOT_putw_le (
  __CR_IO__ iDATOT* that,
  __CR_IN__ int16u  val
    )
{
    iMEMOT* real = (iMEMOT*)that;

    if (!iMemOT_append(real, 2))
        return (FALSE);
    save_misali16(&real->m_buf[real->m_pos], WORD_LE(val));
    real->m_pos += sizeof(int16u);
    return (TRUE);
}

/*
---------------------------------------
    写一个双字LE
---------------------------------------
*/
static bool_t
iMemOT_putd_le (
  __CR_IO__ iDATOT* that,
  __CR_IN__ int32u  val
    )
{
    iMEMOT* real = (iMEMOT*)that;

    if (!iMemOT_append(real, 4))
        return (FALSE);
    save_misali32(&real->m_buf[real->m_pos], DWORD_LE(val));
    real->m_pos += sizeof(int32u);
    return (TRUE);
}

/*
---------------------------------------
    写一个四字LE
---------------------------------------
*/
static bool_t
iMemOT_putq_le (
  __CR_IO__ iDATOT* that,
  __CR_IN__ int64u  val
    )
{
    iMEMOT* real = (iMEMOT*)that;

    if (!iMemOT_append(real, 8))
        return (FALSE);
    save_misali64(&real->m_buf[real->m_pos], QWORD_LE(val));
    real->m_pos += sizeof(int64u);
    return (TRUE);
}

/*
---------------------------------------
    写一个单字BE
---------------------------------------
*/
static bool_t
iMemOT_putw_be (
  __CR_IO__ iDATOT* that,
  __CR_IN__ int16u  val
    )
{
    iMEMOT* real = (iMEMOT*)that;

    if (!iMemOT_append(real, 2))
        return (FALSE);
    save_misali16(&real->m_buf[real->m_pos], WORD_BE(val));
    real->m_pos += sizeof(int16u);
    return (TRUE);
}

/*
---------------------------------------
    写一个双字BE
---------------------------------------
*/
static bool_t
iMemOT_putd_be (
  __CR_IO__ iDATOT* that,
  __CR_IN__ int32u  val
    )
{
    iMEMOT* real = (iMEMOT*)that;

    if (!iMemOT_append(real, 4))
        return (FALSE);
    save_misali32(&real->m_buf[real->m_pos], DWORD_BE(val));
    real->m_pos += sizeof(int32u);
    return (TRUE);
}

/*
---------------------------------------
    写一个四字BE
---------------------------------------
*/
static bool_t
iMemOT_putq_be (
  __CR_IO__ iDATOT* that,
  __CR_IN__ int64u  val
    )
{
    iMEMOT* real = (iMEMOT*)that;

    if (!iMemOT_append(real, 8))
        return (FALSE);
    save_misali64(&real->m_buf[real->m_pos], QWORD_BE(val));
    real->m_pos += sizeof(int64u);
    return (TRUE);
}

/*
---------------------------------------
    写入一组零
---------------------------------------
*/
static bool_t
iMemOT_zero (
  __CR_IO__ iDATOT* that,
  __CR_IN__ leng_t  size
    )
{
    iMEMOT* real = (iMEMOT*)that;

    if (!iMemOT_append(real, size))
        return (FALSE);
    mem_zero(&real->m_buf[real->m_pos], size);
    real->m_pos += size;
    return (TRUE);
}

/*
---------------------------------------
    写入一组数据
---------------------------------------
*/
static leng_t
iMemOT_write (
  __CR_IO__ iDATOT*         that,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    iMEMOT* real = (iMEMOT*)that;

    if (!iMemOT_append(real, size))
        return (0);
    mem_cpy(&real->m_buf[real->m_pos], data, size);
    real->m_pos += size;
    return (size);
}

/*
---------------------------------------
    返回数据指针
---------------------------------------
*/
static void_t*
iMemOT_flush (
  __CR_IN__ iDATOT* that
    )
{
    iMEMOT* real;

    real = (iMEMOT*)that;
    return (real->m_buf);
}

/*
---------------------------------------
    重置数据指针
---------------------------------------
*/
static bool_t
iMemOT_reput (
  __CR_IO__ iDATOT* that,
  __CR_IN__ fsize_t offset
    )
{
    iMEMOT* real = (iMEMOT*)that;

    /* 这里保证了不会截断 */
    if (offset > that->__size__)
        return (FALSE);
    real->m_pos = (leng_t)offset;
    return (TRUE);
}

/* 接口虚函数表 */
static const iDATOT_vtbl _rom_ s_datot_vtbl =
{
    iMemOT_release, iMemOT_getMore, iMemOT_putb_no,
    iMemOT_putw_le, iMemOT_putd_le, iMemOT_putq_le,
    iMemOT_putw_be, iMemOT_putd_be, iMemOT_putq_be,
    iMemOT_zero, iMemOT_write, iDatOT_putsA, iDatOT_putsW,
    iMemOT_flush, iMemOT_reput,
};

/*
=======================================
    生成内存文件写入接口
=======================================
*/
CR_API iDATOT*
create_buff_out (
  __CR_IN__ leng_t  init_size
    )
{
    iMEMOT* memo;
    byte_t* init_data;

    memo = struct_new(iMEMOT);
    if (memo == NULL)
        return (NULL);

    if (init_size == 0)
        init_size = 64;
    init_data = (uchar*)mem_malloc(init_size);
    if (init_data == NULL) {
        mem_free(memo);
        return (NULL);
    }
    memo->m_pos  = 0;
    memo->m_buf  = init_data;
    memo->m_size = init_size;
    memo->datot.__size__ = 0;
    memo->datot.__vptr__ = &s_datot_vtbl;
    return ((iDATOT*)memo);
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
