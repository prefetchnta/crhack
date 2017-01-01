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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 内存文件读取接口实现 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "datio.h"
#include "memlib.h"
#include "strlib.h"

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iMemIN_release (
  __CR_IN__ iDATIN* that
    )
{
    iMEMIN* real = (iMEMIN*)that;

    if (real->m_rel)
        mem_free((void_t*)real->m_mem);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iMemIN_getMore (
  __CR_IN__ iDATIN* that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iDATIN::MEM") != 0)
        return (NULL);
    return ((void_t*)that);
}

/*
---------------------------------------
    返回指针位置
---------------------------------------
*/
extern fsize_t
iMemIN_tell (
  __CR_IN__ iDATIN* that
    )
{
    iMEMIN* real;

    real = (iMEMIN*)that;
    return (real->m_pos);
}

/*
---------------------------------------
    读取指针定位
---------------------------------------
*/
extern bool_t
iMemIN_seek (
  __CR_IO__ iDATIN* that,
  __CR_IN__ fdist_t offset,
  __CR_IN__ uint_t  whence
    )
{
    dist_t  move;
    iMEMIN* real = (iMEMIN*)that;

    /* 偏移的截断检查 */
#if defined(_CR_FILE32_)
    if (cut_int32s(&move, offset))
        return (FALSE);
#else
    if (cut_int64s(&move, offset))
        return (FALSE);
#endif

    switch (whence)
    {
        case SEEK_SET:
            break;

        case SEEK_CUR:
            if (cut_adds(&move, move, real->m_pos))
                return (FALSE);
            break;

        case SEEK_END:
            if (cut_adds(&move, move, real->m_size))
                return (FALSE);
            break;

        default:
            return (FALSE);
    }

    /* 过滤偏移 */
    if (move < 0)
        move = 0;
    else
    if (move > real->m_size)
        move = real->m_size;
    real->m_pos = move;
    return (TRUE);
}

/*
---------------------------------------
    是否处于文件尾部
---------------------------------------
*/
extern bool_t
iMemIN_isEOF (
  __CR_IN__ iDATIN* that
    )
{
    iMEMIN* real = (iMEMIN*)that;

    return ((real->m_pos >= real->m_size) ? TRUE : FALSE);
}

/*
---------------------------------------
    重新定位读取指针
---------------------------------------
*/
extern bool_t
iMemIN_rewind (
  __CR_IO__ iDATIN* that
    )
{
    ((iMEMIN*)that)->m_pos = 0;
    return (TRUE);
}

/*
---------------------------------------
    读取一个字节
---------------------------------------
*/
extern bool_t
iMemIN_getb_no (
  __CR_IO__ iDATIN* that,
  __CR_OT__ byte_t* val
    )
{
    iMEMIN* real = (iMEMIN*)that;

    if (real->m_pos > real->m_size - 1) {
        real->m_pos = real->m_size;
        return (FALSE);
    }
    *val = real->m_ptr[real->m_pos];
    real->m_pos += sizeof(byte_t);
    return (TRUE);
}

/*
---------------------------------------
    读取一个单字LE
---------------------------------------
*/
extern bool_t
iMemIN_getw_le (
  __CR_IO__ iDATIN* that,
  __CR_OT__ int16u* val
    )
{
    iMEMIN* real = (iMEMIN*)that;

    if (real->m_pos > real->m_size - 2) {
        real->m_pos = real->m_size;
        return (FALSE);
    }
    *val = load_misali16(&real->m_ptr[real->m_pos]);
    *val = WORD_LE(*val);
    real->m_pos += sizeof(int16u);
    return (TRUE);
}

/*
---------------------------------------
    读取一个双字LE
---------------------------------------
*/
extern bool_t
iMemIN_getd_le (
  __CR_IO__ iDATIN* that,
  __CR_OT__ int32u* val
    )
{
    iMEMIN* real = (iMEMIN*)that;

    if (real->m_pos > real->m_size - 4) {
        real->m_pos = real->m_size;
        return (FALSE);
    }
    *val = load_misali32(&real->m_ptr[real->m_pos]);
    *val = DWORD_LE(*val);
    real->m_pos += sizeof(int32u);
    return (TRUE);
}

/*
---------------------------------------
    读取一个四字LE
---------------------------------------
*/
extern bool_t
iMemIN_getq_le (
  __CR_IO__ iDATIN* that,
  __CR_OT__ int64u* val
    )
{
    iMEMIN* real = (iMEMIN*)that;

    if (real->m_pos > real->m_size - 8) {
        real->m_pos = real->m_size;
        return (FALSE);
    }
    *val = load_misali64(&real->m_ptr[real->m_pos]);
    *val = QWORD_LE(*val);
    real->m_pos += sizeof(int64u);
    return (TRUE);
}

/*
---------------------------------------
    读取一个单字BE
---------------------------------------
*/
extern bool_t
iMemIN_getw_be (
  __CR_IO__ iDATIN* that,
  __CR_OT__ int16u* val
    )
{
    iMEMIN* real = (iMEMIN*)that;

    if (real->m_pos > real->m_size - 2) {
        real->m_pos = real->m_size;
        return (FALSE);
    }
    *val = load_misali16(&real->m_ptr[real->m_pos]);
    *val = WORD_BE(*val);
    real->m_pos += sizeof(int16u);
    return (TRUE);
}

/*
---------------------------------------
    读取一个双字BE
---------------------------------------
*/
extern bool_t
iMemIN_getd_be (
  __CR_IO__ iDATIN* that,
  __CR_OT__ int32u* val
    )
{
    iMEMIN* real = (iMEMIN*)that;

    if (real->m_pos > real->m_size - 4) {
        real->m_pos = real->m_size;
        return (FALSE);
    }
    *val = load_misali32(&real->m_ptr[real->m_pos]);
    *val = DWORD_BE(*val);
    real->m_pos += sizeof(int32u);
    return (TRUE);
}

/*
---------------------------------------
    读取一个四字BE
---------------------------------------
*/
extern bool_t
iMemIN_getq_be (
  __CR_IO__ iDATIN* that,
  __CR_OT__ int64u* val
    )
{
    iMEMIN* real = (iMEMIN*)that;

    if (real->m_pos > real->m_size - 8) {
        real->m_pos = real->m_size;
        return (FALSE);
    }
    *val = load_misali64(&real->m_ptr[real->m_pos]);
    *val = QWORD_BE(*val);
    real->m_pos += sizeof(int64u);
    return (TRUE);
}

/*
---------------------------------------
    读取一组数据
---------------------------------------
*/
extern leng_t
iMemIN_read (
  __CR_IO__ iDATIN* that,
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    iMEMIN* real = (iMEMIN*)that;

    /* 超出范围的裁减 (保证了加法不溢出) */
    if (size > (leng_t)real->m_size ||
        real->m_pos > (dist_t)(real->m_size - size))
        size = (leng_t)(real->m_size - real->m_pos);
    mem_cpy(data, &real->m_ptr[real->m_pos], size);
    real->m_pos += (dist_t)size;
    return (size);
}

/*
---------------------------------------
    设置文件逻辑区段
---------------------------------------
*/
extern bool_t
iMemIN_setArea (
  __CR_IO__ iDATIN* that,
  __CR_IN__ fdist_t head,
  __CR_IN__ fdist_t tail
    )
{
    dist_t  hd, tl;
    iMEMIN* real = (iMEMIN*)that;

    /* 数值安全检查 */
#if defined(_CR_FILE32_)
    if (cut_int32s(&hd, head))
        return (FALSE);
    if (cut_int32s(&tl, tail))
        return (FALSE);
#else
    if (cut_int64s(&hd, head))
        return (FALSE);
    if (cut_int64s(&tl, tail))
        return (FALSE);
#endif
    if (hd < 0 || tl < 0)
        return (FALSE);

    /* 只设置有效尾 */
    if (tl > real->m_size || tl <= hd)
        tl = real->m_size;

    if (hd < tl)
    {
        real->m_head = hd;
        real->m_tail = tl;
        real->m_size = tl - hd;
        real->m_ptr = real->m_mem + hd;
        that->__head__ = hd;
        that->__tail__ = tl;
        that->__size__ = real->m_size;
        iMemIN_rewind(that);
        return (TRUE);
    }
    return (FALSE);
}

/* 接口虚函数表 */
static const iDATIN_vtbl _rom_ s_datin_vtbl =
{
    iMemIN_release, iMemIN_getMore,
    iMemIN_tell, iMemIN_seek, iDatIN_seek64,
    iMemIN_isEOF, iMemIN_rewind, iMemIN_getb_no,
    iMemIN_getw_le, iMemIN_getd_le, iMemIN_getq_le,
    iMemIN_getw_be, iMemIN_getd_be, iMemIN_getq_be,
    iDatIN_get, iMemIN_read, iDatIN_check, iMemIN_setArea,
};

/*
=======================================
    生成内存文件读取接口
=======================================
*/
CR_API iDATIN*
create_buff_in (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ bool_t          rel_after_del
    )
{
    iMEMIN* memi;

    if ((dist_t)size <= 0)
        return (NULL);

    memi = struct_new(iMEMIN);
    if (memi == NULL)
        return (NULL);
    memi->m_pos = 0;
    memi->m_rel = rel_after_del;
    memi->m_mem = (byte_t*)data;
    memi->m_ptr = (byte_t*)data;
    memi->m_head = 0;
    memi->m_tail = (dist_t)size;
    memi->m_size = (dist_t)size;
    memi->datin.__head__ = 0;
    memi->datin.__tail__ = size;
    memi->datin.__size__ = size;
    memi->datin.__vptr__ = &s_datin_vtbl;
    return ((iDATIN*)memi);
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
