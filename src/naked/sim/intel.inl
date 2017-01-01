/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-05-21  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 模拟 INTEL 总线函数库模板 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/* 管脚相关配置 */
#ifndef _INTEL_INIT_
    #define _INTEL_INIT_
#endif
#ifndef INTEL_ADR_DIRO
    #define INTEL_ADR_DIRO
#endif
#ifndef INTEL_DAT_DIRO
    #define INTEL_DAT_DIRO
#endif
#ifndef INTEL_ALE_DIRO
    #define INTEL_ALE_DIRO
#endif
#ifndef INTEL_NRD_DIRO
    #define INTEL_NRD_DIRO
#endif
#ifndef INTEL_NWR_DIRO
    #define INTEL_NWR_DIRO
#endif
#ifndef INTEL_NCS_DIRO
    #define INTEL_NCS_DIRO
#endif

/* 器件总线类型 */
#ifndef itl_leng_t
    #define itl_leng_t  ufast_t
#endif
#if     defined(_INTEL_ADDR08_)
    #define itl_addr_t  byte_t
#elif   defined(_INTEL_ADDR16_)
    #define itl_addr_t  int16u
#elif   defined(_INTEL_ADDR32_)
    #define itl_addr_t  int32u
#else
    #error "intel.inl: missing ADDR TYPE define!"
#endif
#if     defined(_INTEL_DATA08_)
    #define itl_data_t  byte_t
#elif   defined(_INTEL_DATA16_)
    #define itl_data_t  int16u
#elif   defined(_INTEL_DATA32_)
    #define itl_data_t  int32u
#else
    #error "intel.inl: missing DATA TYPE define!"
#endif

/*
=======================================
    模拟 INTEL 初始化
=======================================
*/
#if defined(ibus_init)
CR_API void_t
ibus_init (void_t)
{
    _INTEL_INIT_
    /* -------- */
    INTEL_NCS_DIRO
    /* -------- */
    INTEL_NCS_SETB
    /* -------- */
#if     defined(INTEL_AD_TGT)
    INTEL_ALE_DIRO
    /* -------- */
    INTEL_ALE_CLRB
    /* -------- */
#endif
    INTEL_NRD_DIRO
    /* -------- */
    INTEL_NRD_SETB
    /* -------- */
    INTEL_NWR_DIRO
    /* -------- */
    INTEL_NWR_SETB
    /* -------- */
    INTEL_ADR_DIRO
    INTEL_DAT_DIRO
    /* -------- */
}
#endif  /* ibus_init */

/*
=======================================
    模拟 INTEL 芯片选中
=======================================
*/
#if defined(ibus_chip_sel)
CR_API void_t
ibus_chip_sel (void_t)
{
#ifndef INTEL_AD_CSL
    INTEL_NCS_CLRB
#endif
}
#endif  /* ibus_chip_sel */

/*
=======================================
    模拟 INTEL 芯片脱离
=======================================
*/
#if defined(ibus_chip_out)
CR_API void_t
ibus_chip_out (void_t)
{
#ifndef INTEL_AD_CSL
    INTEL_NCS_SETB
#endif
}
#endif  /* ibus_chip_out */

/*
=======================================
    模拟 INTEL 写入数据
=======================================
*/
#if defined(ibus_data_set)
CR_API void_t
ibus_data_set (
  __CR_IN__ itl_addr_t  addr,
  __CR_IN__ itl_data_t  data
    )
{
    /* 地址数据复用 */
#if     defined(INTEL_AD_TGT)
    INTEL_ALE_SETB
    /* -------- */
    INTEL_ADR_DIRO
    /* -------- */
    INTEL_ADR_WR(addr);
    INTEL_DELAY_TM_ASH
    INTEL_ALE_CLRB
    INTEL_DELAY_TM_ASL

    /* 地址片选锁存 */
#elif   defined(INTEL_AD_CSL)
    INTEL_NCS_SETB
    INTEL_ADR_WR(addr);
    INTEL_DELAY_TM_ASH
    INTEL_NCS_CLRB
    INTEL_DELAY_TM_ASL
    /* -------- */
    INTEL_DAT_DIRO
    /* -------- */
#else
    INTEL_ADR_WR(addr);
    /* -------- */
    INTEL_DAT_DIRO
    /* -------- */
#endif
    INTEL_DAT_WR(data);
    INTEL_NWR_CLRB
    INTEL_DELAY_TM_WSL
    INTEL_NWR_SETB
    INTEL_DELAY_TM_WSH

    /* 地址片选锁存 */
#if     defined(INTEL_AD_CSL)
    INTEL_NCS_SETB
#endif
}
#endif  /* ibus_data_set */

/*
=======================================
    模拟 INTEL 连续写入数据
=======================================
*/
#if defined(ibus_data_write)
CR_API void_t
ibus_data_write (
  __CR_IN__ itl_addr_t          addr,
  __CR_IN__ const itl_data_t*   data,
  __CR_IN__ itl_leng_t          size
    )
{
    /* 参数过滤 */
    if (size == 0)
        return;

    /* 地址数据复用 */
#if     defined(INTEL_AD_TGT)
    INTEL_ALE_SETB
    /* -------- */
    INTEL_ADR_DIRO
    /* -------- */
    INTEL_ADR_WR(addr);
    INTEL_DELAY_TM_ASH
    INTEL_ALE_CLRB
    INTEL_DELAY_TM_ASL

    /* 地址片选锁存 */
#elif   defined(INTEL_AD_CSL)
    INTEL_NCS_SETB
    INTEL_ADR_WR(addr);
    INTEL_DELAY_TM_ASH
    INTEL_NCS_CLRB
    INTEL_DELAY_TM_ASL
    /* -------- */
    INTEL_DAT_DIRO
    /* -------- */
#else
    INTEL_ADR_WR(addr);
    /* -------- */
    INTEL_DAT_DIRO
    /* -------- */
#endif
    for (; size != 0; size--)
    {
        INTEL_DAT_WR(*data++);
        INTEL_NWR_CLRB
        INTEL_DELAY_TM_WSL
        INTEL_NWR_SETB
        INTEL_DELAY_TM_WSH
    }

    /* 地址片选锁存 */
#if     defined(INTEL_AD_CSL)
    INTEL_NCS_SETB
#endif
}
#endif  /* ibus_data_write */

/*
=======================================
    模拟 INTEL 读取数据
=======================================
*/
#if defined(ibus_data_get)
CR_API itl_data_t
ibus_data_get (
  __CR_IN__ itl_addr_t  addr
    )
{
    itl_data_t  data;

    /* 地址数据复用 */
#if     defined(INTEL_AD_TGT)
    INTEL_ALE_SETB
    /* -------- */
    INTEL_ADR_DIRO
    /* -------- */
    INTEL_ADR_WR(addr);
    INTEL_DELAY_TM_ASH
    INTEL_ALE_CLRB
    INTEL_DELAY_TM_ASL

    /* 地址片选锁存 */
#elif   defined(INTEL_AD_CSL)
    INTEL_NCS_SETB
    INTEL_ADR_WR(addr);
    INTEL_DELAY_TM_ASH
    INTEL_NCS_CLRB
    INTEL_DELAY_TM_ASL
#else
    INTEL_ADR_WR(addr);
#endif
    /* -------- */
    INTEL_DAT_DIRI
    /* -------- */
    INTEL_NRD_CLRB
    INTEL_DELAY_TM_RSL
    data = INTEL_DAT_RD();
    INTEL_NRD_SETB
    INTEL_DELAY_TM_RSH

    /* 地址片选锁存 */
#if     defined(INTEL_AD_CSL)
    INTEL_NCS_SETB
#endif
    return (data);
}
#endif  /* ibus_data_get */

/*
=======================================
    模拟 INTEL 连续读取数据
=======================================
*/
#if defined(ibus_data_read)
CR_API void_t
ibus_data_read (
  __CR_IN__ itl_addr_t  addr,
  __CR_OT__ itl_data_t* data,
  __CR_IN__ itl_leng_t  size
    )
{
    /* 参数过滤 */
    if (size == 0)
        return;

    /* 地址数据复用 */
#if     defined(INTEL_AD_TGT)
    INTEL_ALE_SETB
    /* -------- */
    INTEL_ADR_DIRO
    /* -------- */
    INTEL_ADR_WR(addr);
    INTEL_DELAY_TM_ASH
    INTEL_ALE_CLRB
    INTEL_DELAY_TM_ASL

    /* 地址片选锁存 */
#elif   defined(INTEL_AD_CSL)
    INTEL_NCS_SETB
    INTEL_ADR_WR(addr);
    INTEL_DELAY_TM_ASH
    INTEL_NCS_CLRB
    INTEL_DELAY_TM_ASL
#else
    INTEL_ADR_WR(addr);
#endif
    /* -------- */
    INTEL_DAT_DIRI
    /* -------- */
    for (; size != 0; size--)
    {
        INTEL_NRD_CLRB
        INTEL_DELAY_TM_RSL
        *data++ = INTEL_DAT_RD();
        INTEL_NRD_SETB
        INTEL_DELAY_TM_RSH
    }

    /* 地址片选锁存 */
#if     defined(INTEL_AD_CSL)
    INTEL_NCS_SETB
#endif
}
#endif  /* ibus_data_read */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
