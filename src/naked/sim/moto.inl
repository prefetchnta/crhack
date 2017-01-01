/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-08-29  */
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
/*  >>>>>>>>>>>>>>>>> CrHack 模拟 MOTOROLA 总线函数库模板 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/* 管脚相关配置 */
#ifndef _MOTOL_INIT_
    #define _MOTOL_INIT_
#endif
#ifndef MOTOL_ADR_DIRO
    #define MOTOL_ADR_DIRO
#endif
#ifndef MOTOL_DAT_DIRO
    #define MOTOL_DAT_DIRO
#endif
#ifndef MOTOL_ALE_DIRO
    #define MOTOL_ALE_DIRO
#endif
#ifndef MOTOL_DLE_DIRO
    #define MOTOL_DLE_DIRO
#endif
#ifndef MOTOL_RNW_DIRO
    #define MOTOL_RNW_DIRO
#endif
#ifndef MOTOL_CSE_DIRO
    #define MOTOL_CSE_DIRO
#endif

/* 器件总线类型 */
#ifndef mot_leng_t
    #define mot_leng_t  ufast_t
#endif
#if     defined(_MOTOL_ADDR08_)
    #define mot_addr_t  byte_t
#elif   defined(_MOTOL_ADDR16_)
    #define mot_addr_t  int16u
#elif   defined(_MOTOL_ADDR32_)
    #define mot_addr_t  int32u
#else
    #error "moto.inl: missing ADDR TYPE define!"
#endif
#if     defined(_MOTOL_DATA08_)
    #define mot_data_t  byte_t
#elif   defined(_MOTOL_DATA16_)
    #define mot_data_t  int16u
#elif   defined(_MOTOL_DATA32_)
    #define mot_data_t  int32u
#else
    #error "moto.inl: missing DATA TYPE define!"
#endif

/*
=======================================
    模拟 MOTOROLA 初始化
=======================================
*/
#if defined(mbus_init)
CR_API void_t
mbus_init (void_t)
{
    _MOTOL_INIT_
    /* -------- */
    MOTOL_CSE_DIRO
    /* -------- */
    MOTOL_CSE_SETB
    /* -------- */
#if     defined(MOTOL_AD_TGT)
    MOTOL_ALE_DIRO
    /* -------- */
    MOTOL_ALE_CLRB
    /* -------- */
#endif
    MOTOL_DLE_DIRO
    /* -------- */
    MOTOL_DLE_CLRB
    /* -------- */
    MOTOL_RNW_DIRO
    /* -------- */
    MOTOL_RNW_SETB
    /* -------- */
    MOTOL_ADR_DIRO
    MOTOL_DAT_DIRO
    /* -------- */
}
#endif  /* mbus_init */

/*
=======================================
    模拟 MOTOROLA 芯片选中
=======================================
*/
#if defined(mbus_chip_sel)
CR_API void_t
mbus_chip_sel (void_t)
{
    MOTOL_CSE_CLRB
}
#endif  /* mbus_chip_sel */

/*
=======================================
    模拟 MOTOROLA 芯片脱离
=======================================
*/
#if defined(mbus_chip_out)
CR_API void_t
mbus_chip_out (void_t)
{
    MOTOL_CSE_SETB
}
#endif  /* mbus_chip_out */

/*
=======================================
    模拟 MOTOROLA 写入数据
=======================================
*/
#if defined(mbus_data_set)
CR_API void_t
mbus_data_set (
  __CR_IN__ mot_addr_t  addr,
  __CR_IN__ mot_data_t  data
    )
{
    /* 地址数据复用 */
#if     defined(MOTOL_AD_TGT)
    MOTOL_ALE_SETB
    /* -------- */
    MOTOL_ADR_DIRO
    /* -------- */
    MOTOL_ADR_WR(addr);
    MOTOL_DELAY_TM_ASH
    MOTOL_ALE_CLRB
    MOTOL_DELAY_TM_ASL
#else
    MOTOL_ADR_WR(addr);
    /* -------- */
    MOTOL_DAT_DIRO
    /* -------- */
#endif
    MOTOL_RNW_CLRB
    MOTOL_DAT_WR(data);
    MOTOL_DLE_SETB
    MOTOL_DELAY_TM_WSH
    MOTOL_DLE_CLRB
    MOTOL_DELAY_TM_WSL
}
#endif  /* mbus_data_set */

/*
=======================================
    模拟 MOTOROLA 连续写入数据
=======================================
*/
#if defined(mbus_data_write)
CR_API void_t
mbus_data_write (
  __CR_IN__ mot_addr_t          addr,
  __CR_IN__ const mot_data_t*   data,
  __CR_IN__ mot_leng_t          size
    )
{
    /* 参数过滤 */
    if (size == 0)
        return;

    /* 地址数据复用 */
#if     defined(MOTOL_AD_TGT)
    MOTOL_ALE_SETB
    /* -------- */
    MOTOL_ADR_DIRO
    /* -------- */
    MOTOL_ADR_WR(addr);
    MOTOL_DELAY_TM_ASH
    MOTOL_ALE_CLRB
    MOTOL_DELAY_TM_ASL
#else
    MOTOL_ADR_WR(addr);
    /* -------- */
    MOTOL_DAT_DIRO
    /* -------- */
#endif
    MOTOL_RNW_CLRB
    for (; size != 0; size--)
    {
        MOTOL_DAT_WR(*data++);
        MOTOL_DLE_SETB
        MOTOL_DELAY_TM_WSH
        MOTOL_DLE_CLRB
        MOTOL_DELAY_TM_WSL
    }
}
#endif  /* mbus_data_write */

/*
=======================================
    模拟 MOTOROLA 读取数据
=======================================
*/
#if defined(mbus_data_get)
CR_API mot_data_t
mbus_data_get (
  __CR_IN__ mot_addr_t  addr
    )
{
    mot_data_t  data;

    /* 地址数据复用 */
#if     defined(MOTOL_AD_TGT)
    MOTOL_ALE_SETB
    /* -------- */
    MOTOL_ADR_DIRO
    /* -------- */
    MOTOL_ADR_WR(addr);
    MOTOL_DELAY_TM_ASH
    MOTOL_ALE_CLRB
    MOTOL_DELAY_TM_ASL
#else
    MOTOL_ADR_WR(addr);
#endif
    MOTOL_RNW_SETB
    /* -------- */
    MOTOL_DAT_DIRI
    /* -------- */
    MOTOL_DLE_SETB
    MOTOL_DELAY_TM_RSH
    data = MOTOL_DAT_RD();
    MOTOL_DLE_CLRB
    MOTOL_DELAY_TM_RSL
    return (data);
}
#endif  /* mbus_data_get */

/*
=======================================
    模拟 MOTOROLA 连续读取数据
=======================================
*/
#if defined(mbus_data_read)
CR_API void_t
mbus_data_read (
  __CR_IN__ mot_addr_t  addr,
  __CR_OT__ mot_data_t* data,
  __CR_IN__ mot_leng_t  size
    )
{
    /* 参数过滤 */
    if (size == 0)
        return;

    /* 地址数据复用 */
#if     defined(MOTOL_AD_TGT)
    MOTOL_ALE_SETB
    /* -------- */
    MOTOL_ADR_DIRO
    /* -------- */
    MOTOL_ADR_WR(addr);
    MOTOL_DELAY_TM_ASH
    MOTOL_ALE_CLRB
    MOTOL_DELAY_TM_ASL
#else
    MOTOL_ADR_WR(addr);
#endif
    MOTOL_RNW_SETB
    /* -------- */
    MOTOL_DAT_DIRI
    /* -------- */
    for (; size != 0; size--)
    {
        MOTOL_DLE_SETB
        MOTOL_DELAY_TM_RSH
        *data++ = MOTOL_DAT_RD();
        MOTOL_DLE_CLRB
        MOTOL_DELAY_TM_RSL
    }
}
#endif  /* mbus_data_read */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
