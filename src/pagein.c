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
/*  >>>>>>>>>>>>>>>>>>> CrHack 内存映射文件读取接口实现 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "datio.h"
#include "memlib.h"
#include "strlib.h"

#ifndef _CR_NO_VPAGE_

/* 接口内部数据结构 */
typedef struct
{
        /* 通用部分 */
        iMEMIN  memin;

        /* 个性部分 */
        sVFILE  m_vfile;

} iPAGEIN;

/*
---------------------------------------
    释放接口
---------------------------------------
*/
static void_t
iPageIN_release (
  __CR_IN__ iDATIN* that
    )
{
    iPAGEIN*    real;

    real = (iPAGEIN*)that;
    file_release(&real->m_vfile);
    mem_free(that);
}

/*
---------------------------------------
    扩展接口
---------------------------------------
*/
static void_t*
iPageIN_getMore (
  __CR_IN__ iDATIN* that,
  __CR_IN__ port_t  iid
    )
{
    /* 判断一下名称 */
    if (str_cmpA(iid, "iDATIN::PAGE") != 0)
        return (NULL);
    return ((void_t*)that);
}

/* 接口虚函数表 */
static const iDATIN_vtbl _rom_ s_datin_vtbl =
{
    iPageIN_release, iPageIN_getMore,
    iMemIN_tell, iMemIN_seek, iDatIN_seek64,
    iMemIN_isEOF, iMemIN_rewind, iMemIN_getb_no,
    iMemIN_getw_le, iMemIN_getd_le, iMemIN_getq_le,
    iMemIN_getw_be, iMemIN_getd_be, iMemIN_getq_be,
    iDatIN_get, iMemIN_read, iDatIN_check, iMemIN_setArea,
};

/*
=======================================
    生成内存映射文件接口A
=======================================
*/
CR_API iDATIN*
create_page_inA (
  __CR_IN__ const ansi_t*   name
    )
{
    dist_t      size;
    byte_t*     data;
    iPAGEIN*    page;

    page = struct_new(iPAGEIN);
    if (page == NULL)
        return (NULL);

    data = (uchar*)file_mappingA(name, &page->m_vfile);
    if (data == NULL)
        goto _failure2;

    size = (dist_t)page->m_vfile.size;
    if (size <= 0)
        goto _failure1;
    page->memin.m_pos = 0;
    page->memin.m_rel = FALSE;
    page->memin.m_mem =  data;
    page->memin.m_ptr =  data;
    page->memin.m_head = 0;
    page->memin.m_tail = size;
    page->memin.m_size = size;
    page->memin.datin.__head__ = 0;
    page->memin.datin.__tail__ = size;
    page->memin.datin.__size__ = size;
    page->memin.datin.__vptr__ = &s_datin_vtbl;
    return ((iDATIN*)page);

_failure1:
    file_release(&page->m_vfile);
_failure2:
    mem_free(page);
    return (NULL);
}

#if !defined(_CR_NAKED_NO_WSTRING_)

/*
=======================================
    生成内存映射文件接口W
=======================================
*/
CR_API iDATIN*
create_page_inW (
  __CR_IN__ const wide_t*   name
    )
{
    dist_t      size;
    byte_t*     data;
    iPAGEIN*    page;

    page = struct_new(iPAGEIN);
    if (page == NULL)
        return (NULL);

    data = (uchar*)file_mappingW(name, &page->m_vfile);
    if (data == NULL)
        goto _failure2;

    size = (dist_t)page->m_vfile.size;
    if (size <= 0)
        goto _failure1;
    page->memin.m_pos = 0;
    page->memin.m_rel = FALSE;
    page->memin.m_mem =  data;
    page->memin.m_ptr =  data;
    page->memin.m_head = 0;
    page->memin.m_tail = size;
    page->memin.m_size = size;
    page->memin.datin.__head__ = 0;
    page->memin.datin.__tail__ = size;
    page->memin.datin.__size__ = size;
    page->memin.datin.__vptr__ = &s_datin_vtbl;
    return ((iDATIN*)page);

_failure1:
    file_release(&page->m_vfile);
_failure2:
    mem_free(page);
    return (NULL);
}

#endif  /* !_CR_NAKED_NO_WSTRING_ */

#endif  /* !_CR_NO_VPAGE_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
