/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-18  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 可增长数组函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "datlib.h"

/*
=======================================
    构造函数
=======================================
*/
CR_API void_t
array_init (
  __CR_OT__ sARRAY* that
    )
{
    struct_zero(that, sARRAY);
}

/*
=======================================
    析构函数
=======================================
*/
CR_API void_t
array_free (
  __CR_IO__ sARRAY* that,
  __CR_IN__ leng_t  unit
    )
{
    if (that->__buff__ != NULL) {
        array_clear(that, unit);
        mem_free(that->__buff__);
    }
    struct_zero(that, sARRAY);
}

/*
=======================================
    清空成员
=======================================
*/
CR_API void_t
array_clear (
  __CR_IO__ sARRAY* that,
  __CR_IN__ leng_t  unit
    )
{
    leng_t  idx;

    if (that->free != NULL) {
        idx = that->__cnts__;
        while (idx-- != 0)
            that->free(array_get_unit(that, unit, idx));
    }
    that->__cnts__ = 0;
}

/*
=======================================
    固定分配成员的个数
=======================================
*/
CR_API bool_t
array_no_grow (
  __CR_IO__ sARRAY* that,
  __CR_IN__ leng_t  unit
    )
{
    void_t* temp;

    if (that->__cnts__ == that->__size__)
        return (TRUE);

    if (that->__cnts__ == 0) {
        mem_free(that->__buff__);
        that->__buff__ = NULL;
        that->__size__ = 0;
        return (TRUE);
    }
    temp = mem_realloc(that->__buff__, that->__cnts__ * unit);
    if (temp == NULL)
        return (FALSE);
    that->__buff__ = (uchar*)temp;
    that->__size__ = that->__cnts__;
    return (TRUE);
}

/*
=======================================
    保留数组大小 (只能扩大)
=======================================
*/
CR_API bool_t
array_reserve (
  __CR_IO__ sARRAY* that,
  __CR_IN__ leng_t  unit,
  __CR_IN__ leng_t  size
    )
{
    uchar*  new_data;
    leng_t  new_size;

    if (size <= that->__size__)
        return (TRUE);

    if (cut_mul(&new_size, size, unit))
        return (FALSE);

    new_data = (uchar*)mem_realloc(that->__buff__, new_size);
    if (new_data == NULL)
        return (FALSE);
    that->__size__ = size;
    that->__buff__ = new_data;
    return (TRUE);
}

/*
=======================================
    压入数组元素 (不可生长)
=======================================
*/
CR_API void_t*
array_push (
  __CR_IO__ sARRAY*         that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ const void_t*   data
    )
{
    void_t* rett;

    if (that->__cnts__ >= that->__size__)
        return (NULL);
    rett = that->__buff__ + (that->__cnts__++) * unit;
    return (mem_cpy(rett, data, unit));
}

/*
=======================================
    压入数组元素 (x2 + 1)
=======================================
*/
CR_API void_t*
array_push_grow (
  __CR_IO__ sARRAY*         that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ const void_t*   data
    )
{
    void_t* rett;

    if (that->__cnts__ >= that->__size__ &&
        !array_reserve(that, unit, that->__size__ * 2 + 1))
        return (NULL);
    rett = that->__buff__ + (that->__cnts__++) * unit;
    return (mem_cpy(rett, data, unit));
}

/*
=======================================
    返回数组最后的元素
=======================================
*/
CR_API void_t*
array_top (
  __CR_IN__ const sARRAY*   that,
  __CR_IN__ leng_t          unit,
  __CR_OT__ void_t*         data
    )
{
    void_t* rett;

    if (that->__cnts__ == 0)
        return (NULL);
    rett = that->__buff__ + (that->__cnts__-1) * unit;
    if (data != NULL)
        mem_cpy(data, rett, unit);
    return (rett);
}

/*
=======================================
    弹出数组最后的元素
=======================================
*/
CR_API bool_t
array_pop (
  __CR_IO__ sARRAY* that,
  __CR_IN__ leng_t  unit,
  __CR_OT__ void_t* data
    )
{
    uchar*  pos;

    if (that->__cnts__ == 0)
        return (FALSE);
    pos = that->__buff__ + (--that->__cnts__) * unit;
    if (data != NULL) {
        mem_cpy(data, pos, unit);
    }
    else {
        if (that->free != NULL)
            that->free(pos);
    }
    return (TRUE);
}

/*
=======================================
    删除指定数组元素
=======================================
*/
CR_API void_t
array_delete (
  __CR_IO__ sARRAY* that,
  __CR_IN__ leng_t  unit,
  __CR_IN__ leng_t  index
    )
{
    uchar*  pos;

    if (index >= that->__cnts__)
        return;
    pos = (uchar*)array_get_unit(that, unit, index);
    if (that->free != NULL)
        that->free(pos);
    that->__cnts__--;
    if (that->__cnts__ > index)
        mem_cpy(pos, pos + unit, (that->__cnts__ - index) * unit);
}

/*
=======================================
    插入一个元素到指定位置 (不可生长)
=======================================
*/
CR_API void_t*
array_insert (
  __CR_IO__ sARRAY*         that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ leng_t          index,
  __CR_IN__ const void_t*   data
    )
{
    uchar*  pos;

    if (index >= that->__cnts__)
        return (array_push(that, unit, data));

    if (that->__cnts__ >= that->__size__)
        return (NULL);
    pos = (uchar*)array_get_unit(that, unit, index);
    mem_mov(pos + unit, pos, ((that->__cnts__++) - index) * unit);
    return (mem_cpy(pos, data, unit));
}

/*
=======================================
    插入一个元素到指定位置 (x2 + 1)
=======================================
*/
CR_API void_t*
array_insert_grow (
  __CR_IO__ sARRAY*         that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ leng_t          index,
  __CR_IN__ const void_t*   data
    )
{
    uchar*  pos;

    if (index >= that->__cnts__)
        return (array_push_grow(that, unit, data));

    if (that->__cnts__ >= that->__size__ &&
        !array_reserve(that, unit, that->__size__ * 2 + 1))
        return (NULL);
    pos = (uchar*)array_get_unit(that, unit, index);
    mem_mov(pos + unit, pos, ((that->__cnts__++) - index) * unit);
    return (mem_cpy(pos, data, unit));
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
