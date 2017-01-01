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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 文件读写接口共用函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "datio.h"
#include "memlib.h"

/*****************************************************************************/
/*                            读取接口的共用函数                             */
/*****************************************************************************/

/*
---------------------------------------
    读取文件剩余部分
---------------------------------------
*/
extern byte_t*
iDatIN_get (
  __CR_IN__ iDATIN* that,
  __CR_OT__ leng_t* size,
  __CR_IN__ bool_t  rel
    )
{
    leng_t  red;
    byte_t* dat;
    fsize_t pos, fsz = that->__size__;

    /* 获取当前文件指针 */
    pos = CR_VCALL(that)->tell(that);
    if (pos == CR_F_ERROR)
        return (NULL);

    /* 检查是否有内容可读 */
    if (fsz <= pos)
        return (NULL);

    fsz -= pos;
    if (fsz > ((leng_t)-1) / 2)
        return (NULL);

    /* 分配内存并读起数据 */
    dat = (byte_t*)mem_malloc((leng_t)fsz);
    if (dat == NULL)
        return (NULL);
    red = CR_VCALL(that)->read(that, dat, (leng_t)fsz);
    if (red != (leng_t)fsz) {
        mem_free(dat);
        return (NULL);
    }

    /* 返回数据 */
    if (size != NULL)
        *size = red;
    if (rel)    /* 顺便释放接口^^ */
        CR_VCALL(that)->release(that);
    return (dat);
}

/*
---------------------------------------
    读取指针定位64
---------------------------------------
*/
extern bool_t
iDatIN_seek64 (
  __CR_IN__ iDATIN* that,
  __CR_IN__ int64s  offset,
  __CR_IN__ uint_t  whence
    )
{
#if defined(_CR_FILE64_)
    return (CR_VCALL(that)->seek(that, offset, whence));
#else
    fdist_t move;

    /* 64位有符号整数截断检查 */
    if (cut_int64_32s(&move, offset))
        return (FALSE);
    return (CR_VCALL(that)->seek(that, move, whence));
#endif
}

/*
---------------------------------------
    读取一组数据并检查
---------------------------------------
*/
extern bool_t
iDatIN_check (
  __CR_IO__ iDATIN*         that,
  __CR_OT__ void_t*         temp,
  __CR_IN__ const ansi_t*   tag,
  __CR_IN__ uint_t          size
    )
{
    if (CR_VCALL(that)->read(that, temp, size) != size)
        return (FALSE);
    return ((mem_cmp(temp, tag, size) == 0) ? TRUE : FALSE);
}

/*
---------------------------------------
    设置文件逻辑区段 (磁盘使用)
---------------------------------------
*/
extern bool_t
iDatIN_setArea (
  __CR_OT__ iDATIN* that,
  __CR_IN__ fdist_t head,
  __CR_IN__ fdist_t tail
    )
{
    if (head < 0 || tail < 0)
        return (FALSE);

    /* 只设置有效尾 */
    if (tail > that->__size__ || tail <= head)
        tail = that->__size__;

    if (head < tail)
    {
        that->__head__ = head;
        that->__tail__ = tail;
        that->__size__ = tail - head;
        return (CR_VCALL(that)->rewind(that));
    }
    return (FALSE);
}

/*****************************************************************************/
/*                            写入接口的共用函数                             */
/*****************************************************************************/

/*
---------------------------------------
    写入一个字符串A
---------------------------------------
*/
extern bool_t
iDatOT_putsA (
  __CR_IO__ iDATOT*         that,
  __CR_IN__ const ansi_t*   str,
  __CR_IN__ leng_t          len
    )
{
    if (len == 0)
        len = str_lenA(str);
    len *= sizeof(ansi_t);

    if (len != 0) {
        if (CR_VCALL(that)->write(that, str, len) != len)
            return (FALSE);
    }
    return (TRUE);
}

/*
---------------------------------------
    写入一个字符串W
---------------------------------------
*/
extern bool_t
iDatOT_putsW (
  __CR_IO__ iDATOT*         that,
  __CR_IN__ const wide_t*   str,
  __CR_IN__ leng_t          len
    )
{
    if (len == 0)
        len = str_lenW(str);
    len *= sizeof(wide_t);

    if (len != 0) {
        if (CR_VCALL(that)->write(that, str, len) != len)
            return (FALSE);
    }
    return (TRUE);
}

/*****************************************************************************/
/*                             位流输入接口包装                              */
/*****************************************************************************/

/*
=======================================
    初始化位流输入结构
=======================================
*/
CR_API void_t
bitin_init (
  __CR_OT__ sBITIN* bitin,
  __CR_IN__ iDATIN* datin,
  __CR_IN__ uint_t  max_size
    )
{
    if (max_size > 4)
        max_size = 4;
    else
    if (max_size < 1)
        max_size = 1;
    bitin->datin = datin;
    bitin->buff[0] = 0;
    bitin->buff[1] = 0;
    bitin->buff[2] = 0;
    bitin->buff[3] = 0;
    bitin->rest_bits = 0;
    bitin->cur_size = max_size;
    bitin->max_size = max_size;
}

/*
=======================================
    重置位流输入大小
=======================================
*/
CR_API void_t
bitin_reset (
  __CR_IO__ sBITIN* bitin,
  __CR_IN__ uint_t  max_size
    )
{
    if (max_size > 4)
        max_size = 4;
    else
    if (max_size < 1)
        max_size = 1;
    bitin->max_size = max_size;
}

/*
---------------------------------------
    读入一位 (高位先读)
---------------------------------------
*/
static bool_t
bitin_hi_read (
  __CR_IO__ sBITIN* bitin,
  __CR_OT__ bool_t* getbit
    )
{
    uint_t  idx;
    leng_t  back;
    iDATIN* datin;

    /* 当前位全读完了 */
    if (bitin->rest_bits == 0) {
        datin = bitin->datin;
        back = CR_VCALL(datin)->read(datin, bitin->buff, bitin->max_size);
        if (back != bitin->max_size)
            return (FALSE);
        bitin->cur_size  = bitin->max_size;
        bitin->rest_bits = bitin->max_size * 8;
    }

    /* 读取一位 */
    if (bitin->buff[0] & 0x80)
        *getbit = TRUE;
    else
        *getbit = FALSE;
    bitin->rest_bits -= 1;

    /* 处理位移 */
    for (idx = 0; idx < bitin->cur_size - 1; idx++) {
        bitin->buff[idx] <<= 1;
        bitin->buff[idx] |= (bitin->buff[idx + 1] >> 7) & 1;
    }
    bitin->buff[idx] <<= 1;
    return (TRUE);
}

/*
=======================================
    高位先读调用 (推挤方式)
=======================================
*/
CR_API bool_t
bitin_hi_push (
  __CR_IO__ sBITIN* bitin,
  __CR_OT__ uint_t* value,
  __CR_IN__ ufast_t count
    )
{
    uint_t  temp;
    bool_t  getbit;

    /* 过滤非法参数 */
    if (count > bitsof(uint_t) ||
        bitin->cur_size > 4 || bitin->cur_size < 1 ||
        bitin->max_size > 4 || bitin->max_size < 1)
        return (FALSE);

    /* 每读取一位推上去一位 */
    for (temp = 0; count != 0; count--) {
        if (!bitin_hi_read(bitin, &getbit))
            return (FALSE);
        temp <<= 1;
        if (getbit)
            temp |= 1;
    }
    *value = temp;
    return (TRUE);
}

/*
=======================================
    高位先读调用 (层叠方式)
=======================================
*/
CR_API bool_t
bitin_hi_casc (
  __CR_IO__ sBITIN* bitin,
  __CR_OT__ uint_t* value,
  __CR_IN__ ufast_t count
    )
{
    bool_t  getbit;
    uint_t  temp, bits;

    /* 过滤非法参数 */
    if (count > bitsof(uint_t) ||
        bitin->cur_size > 4 || bitin->cur_size < 1 ||
        bitin->max_size > 4 || bitin->max_size < 1)
        return (FALSE);

    /* 每读取一位叠上去一位 */
    for (bits = 1, temp = 0; count != 0; count--) {
        if (!bitin_hi_read(bitin, &getbit))
            return (FALSE);
        if (getbit)
            temp |= bits;
        bits <<= 1;
    }
    *value = temp;
    return (TRUE);
}

/*
---------------------------------------
    读入一位 (低位先读)
---------------------------------------
*/
static bool_t
bitin_lo_read (
  __CR_IO__ sBITIN* bitin,
  __CR_OT__ bool_t* getbit
    )
{
    uint_t  idx;
    leng_t  back;
    iDATIN* datin;

    /* 当前位全读完了 */
    if (bitin->rest_bits == 0) {
        datin = bitin->datin;
        back = CR_VCALL(datin)->read(datin, bitin->buff, bitin->max_size);
        if (back != bitin->max_size)
            return (FALSE);
        bitin->cur_size  = bitin->max_size;
        bitin->rest_bits = bitin->max_size * 8;
    }

    /* 读取一位 */
    idx = bitin->cur_size - 1;
    if (bitin->buff[idx] & 0x01)
        *getbit = TRUE;
    else
        *getbit = FALSE;
    bitin->rest_bits -= 1;

    /* 处理位移 */
    for (; idx != 0; idx--) {
        bitin->buff[idx] >>= 1;
        bitin->buff[idx] |= (bitin->buff[idx - 1] << 7) & 0x80;
    }
    bitin->buff[0] >>= 1;
    return (TRUE);
}

/*
=======================================
    低位先读调用 (推挤方式)
=======================================
*/
CR_API bool_t
bitin_lo_push (
  __CR_IO__ sBITIN* bitin,
  __CR_OT__ uint_t* value,
  __CR_IN__ ufast_t count
    )
{
    uint_t  temp;
    bool_t  getbit;

    /* 过滤非法参数 */
    if (count > bitsof(uint_t) ||
        bitin->cur_size > 4 || bitin->cur_size < 1 ||
        bitin->max_size > 4 || bitin->max_size < 1)
        return (FALSE);

    /* 每读取一位推上去一位 */
    for (temp = 0; count != 0; count--) {
        if (!bitin_lo_read(bitin, &getbit))
            return (FALSE);
        temp <<= 1;
        if (getbit)
            temp |= 1;
    }
    *value = temp;
    return (TRUE);
}

/*
=======================================
    低位先读调用 (层叠方式)
=======================================
*/
CR_API bool_t
bitin_lo_casc (
  __CR_IO__ sBITIN* bitin,
  __CR_OT__ uint_t* value,
  __CR_IN__ ufast_t count
    )
{
    bool_t  getbit;
    uint_t  temp, bits;

    /* 过滤非法参数 */
    if (count > bitsof(uint_t) ||
        bitin->cur_size > 4 || bitin->cur_size < 1 ||
        bitin->max_size > 4 || bitin->max_size < 1)
        return (FALSE);

    /* 每读取一位叠上去一位 */
    for (bits = 1, temp = 0; count != 0; count--) {
        if (!bitin_lo_read(bitin, &getbit))
            return (FALSE);
        if (getbit)
            temp |= bits;
        bits <<= 1;
    }
    *value = temp;
    return (TRUE);
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
