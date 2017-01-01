/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-06-12  */
/*     #######          ###    ###      [KRNL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 硬件总线函数库模板 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_ADBUS_H__
#define __CR_ADBUS_H__

/* 器件总线类型 */
#ifndef itl_leng_t
    #define itl_leng_t  ufast_t
#endif
#ifndef mot_leng_t
    #define mot_leng_t  ufast_t
#endif
#if     defined(_INTEL_ADDR08_) || \
        defined(_MOTOL_ADDR08_)
    #define itl_addr_t  byte_t
    #define mot_addr_t  byte_t
#elif   defined(_INTEL_ADDR16_) || \
        defined(_MOTOL_ADDR16_)
    #define itl_addr_t  int16u
    #define mot_addr_t  int16u
#elif   defined(_INTEL_ADDR32_) || \
        defined(_MOTOL_ADDR32_)
    #define itl_addr_t  int32u
    #define mot_addr_t  int32u
#else
    #error "adbus.h: missing ADDR TYPE define!"
#endif
#if     defined(_INTEL_DATA08_) || \
        defined(_MOTOL_DATA08_)
    #define itl_data_t  byte_t
    #define mot_data_t  byte_t
#elif   defined(_INTEL_DATA16_) || \
        defined(_MOTOL_DATA16_)
    #define itl_data_t  int16u
    #define mot_data_t  int16u
#elif   defined(_INTEL_DATA32_) || \
        defined(_MOTOL_DATA32_)
    #define itl_data_t  int32u
    #define mot_data_t  int32u
#else
    #error "adbus.h: missing DATA TYPE define!"
#endif

/* 总线位置定义 */
#ifndef _pos_
    #define _pos_
#endif

/* 硬件 MOTO 总线函数直接映射 */
#define mbus_chip_sel       ibus_chip_sel
#define mbus_chip_out       ibus_chip_out
#define mbus_data_set       ibus_data_set
#define mbus_data_get       ibus_data_get
#define mbus_data_read      ibus_data_read
#define mbus_data_write     ibus_data_write

/* 连续读写的进入退出映射
   用于实现一些额外的硬件加速 */
#ifndef ADBUS_READ_START
    #define ADBUS_READ_START
#endif
#ifndef ADBUS_READ_CHECK
    #define ADBUS_READ_CHECK
#endif
#ifndef ADBUS_WRITE_START
    #define ADBUS_WRITE_START
#endif
#ifndef ADBUS_WRITE_CHECK
    #define ADBUS_WRITE_CHECK
#endif

#if !defined(__no_ibus_chip_sel) && \
    !defined(__no_mbus_chip_sel)
/*
=======================================
    硬件总线芯片选中
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define ibus_chip_sel()
#else
cr_inline void_t
ibus_chip_sel (void_t)
{
}
#endif  /* _CR_NO_INLINE_ */

#endif  /* !__no_ibus_chip_sel &&
           !__no_mbus_chip_sel */

#if !defined(__no_ibus_chip_out) && \
    !defined(__no_mbus_chip_out)
/*
=======================================
    硬件总线芯片脱离
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define ibus_chip_out()
#else
cr_inline void_t
ibus_chip_out (void_t)
{
}
#endif  /* _CR_NO_INLINE_ */

#endif  /* !__no_ibus_chip_out &&
           !__no_mbus_chip_out */

#if !defined(__no_ibus_data_set) && \
    !defined(__no_mbus_data_set)
/*
=======================================
    硬件总线写入数据
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define ibus_data_set(_a, _d) \
        *(volatile itl_data_t _pos_*)(_a) = (_d)
#else
cr_inline void_t
ibus_data_set (
  __CR_IN__ itl_addr_t  addr,
  __CR_IN__ itl_data_t  data
    )
{
    *(volatile itl_data_t _pos_*)(addr) = data;
}
#endif  /* _CR_NO_INLINE_ */

#endif  /* !__no_ibus_data_set &&
           !__no_mbus_data_set */

#if !defined(__no_ibus_data_write) && \
    !defined(__no_mbus_data_write)
/*
=======================================
    硬件总线连续写入数据
=======================================
*/
cr_inline void_t
ibus_data_write (
  __CR_IN__ itl_addr_t          addr,
  __CR_IN__ const itl_data_t*   data,
  __CR_IN__ itl_leng_t          size
    )
{
#if !defined(ADBUS_HARDWARE)
    for (; size != 0; size--)
        *(volatile itl_data_t _pos_*)(addr) = *data++;
#else
    itl_leng_t  idx;

    ADBUS_WRITE_START
    for (idx = 0; idx < size; idx++) {
        *(volatile itl_data_t _pos_*)(addr) = *data++;
        if (idx == ADBUS_HARDWARE - 1) {
            ADBUS_WRITE_CHECK
        }
    }
#endif  /* !ADBUS_HARDWARE */
}

#endif  /* !__no_ibus_data_write &&
           !__no_mbus_data_write */

#if !defined(__no_ibus_data_get) && \
    !defined(__no_mbus_data_get)
/*
=======================================
    硬件总线读取数据
=======================================
*/
#if defined(_CR_NO_INLINE_)
    #define ibus_data_get(_a) \
        *(volatile itl_data_t _pos_*)(_a)
#else
cr_inline itl_data_t
ibus_data_get (
  __CR_IN__ itl_addr_t  addr
    )
{
    return (*(volatile itl_data_t _pos_*)(addr));
}
#endif  /* _CR_NO_INLINE_ */

#endif  /* !__no_ibus_data_get &&
           !__no_mbus_data_get */

#if !defined(__no_ibus_data_read) && \
    !defined(__no_mbus_data_read)
/*
=======================================
    硬件总线连续读取数据
=======================================
*/
cr_inline void_t
ibus_data_read (
  __CR_IN__ itl_addr_t  addr,
  __CR_OT__ itl_data_t* data,
  __CR_IN__ itl_leng_t  size
    )
{
#if !defined(ADBUS_HARDWARE)
    for (; size != 0; size--)
        *data++ = *(volatile itl_data_t _pos_*)(addr);
#else
    itl_leng_t  idx;

    ADBUS_READ_START
    for (idx = 0; idx < size; idx++) {
        *data++ = *(volatile itl_data_t _pos_*)(addr);
        if (idx == ADBUS_HARDWARE - 1) {
            ADBUS_READ_CHECK
        }
    }
#endif  /* !ADBUS_HARDWARE */
}

#endif  /* !__no_ibus_data_read &&
           !__no_mbus_data_read */

#endif  /* !__CR_ADBUS_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
