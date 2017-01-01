/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-05-31  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 模拟 1WIRE 函数库模板 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/* 管脚相关配置 */
#ifndef _WIRE1_INIT_
    #define _WIRE1_INIT_
#endif
#ifndef WIRE1_DQ_DIRO
    #define WIRE1_DQ_DIRO
#endif
#ifndef WIRE1_DQ_DIRI
    #define WIRE1_DQ_DIRI   WIRE1_DQ_SETB
#endif

/*
=======================================
    模拟 1WIRE 初始化
=======================================
*/
#if defined(wire1_init)
CR_API void_t
wire1_init (void_t)
{
    _WIRE1_INIT_
    /* ------- */
    WIRE1_DQ_DIRI
    /* ------- */
}
#endif  /* wire1_init */

/*
=======================================
    模拟 1WIRE 复位
=======================================
*/
#if defined(wire1_reset)
CR_API retc_t
wire1_reset (void_t)
{
    retc_t  okay;

    /* ------- */
    WIRE1_DQ_DIRO
    /* ------- */
    WIRE1_DQ_CLRB
    WIRE1_DELAY_480_US
    /* ------- */
    WIRE1_DQ_DIRI
    /* ------- */
    WIRE1_DELAY__60_US
    if (WIRE1_DQ_GETB)
        okay = FALSE;
    else
        okay = TRUE;
    WIRE1_DELAY_240_US
    return (okay);
}
#endif  /* wire1_reset */

/*
=======================================
    模拟 1WIRE 接收一位
=======================================
*/
#if defined(wire1_recv_bit)
CR_API retc_t
wire1_recv_bit (void_t)
{
    retc_t  val;

    WIRE1_DELAY___1_US
    /* ------- */
    WIRE1_DQ_DIRO
    /* ------- */
    WIRE1_DQ_CLRB
    WIRE1_DELAY___1_US
    /* ------- */
    WIRE1_DQ_DIRI
    /* ------- */
    WIRE1_DELAY___1_US
    if (WIRE1_DQ_GETB)
        val = 1;
    else
        val = 0;
    WIRE1_DELAY__60_US
    return (val);
}
#endif  /* wire1_recv_bit */

/*
=======================================
    模拟 1WIRE 接收一个字节
=======================================
*/
#if defined(wire1_recv_byte)
CR_API byte_t
wire1_recv_byte (void_t)
{
    byte_t  val = 0;
    ufast_t idx = 8;

    for (; idx != 0; idx--)
    {
        val >>= 1;
        WIRE1_DELAY___1_US
        /* ------- */
        WIRE1_DQ_DIRO
        /* ------- */
        WIRE1_DQ_CLRB
        WIRE1_DELAY___1_US
        /* ------- */
        WIRE1_DQ_DIRI
        /* ------- */
        WIRE1_DELAY___1_US
        if (WIRE1_DQ_GETB)
            val |= 0x80;
        WIRE1_DELAY__60_US
    }
    return (val);
}
#endif  /* wire1_recv_byte */

/*
=======================================
    模拟 1WIRE 发送一位
=======================================
*/
#if defined(wire1_send_bit)
CR_API void_t
wire1_send_bit (
  __CR_IN__ retc_t  val
    )
{
    WIRE1_DELAY___1_US
    /* ------- */
    WIRE1_DQ_DIRO
    /* ------- */
    WIRE1_DQ_CLRB
    WIRE1_DELAY___1_US
    if (val)
        WIRE1_DQ_DIRI
    WIRE1_DELAY__60_US
    /* ------- */
    WIRE1_DQ_DIRI
    /* ------- */
}
#endif  /* wire1_send_bit */

/*
=======================================
    模拟 1WIRE 发送一个字节
=======================================
*/
#if defined(wire1_send_byte)
CR_API void_t
wire1_send_byte (
  __CR_IN__ byte_t  val
    )
{
    ufast_t idx = 8;

    for (; idx != 0; idx--)
    {
        WIRE1_DELAY___1_US
        /* ------- */
        WIRE1_DQ_DIRO
        /* ------- */
        WIRE1_DQ_CLRB
        WIRE1_DELAY___1_US
        if (val & 1)
            WIRE1_DQ_DIRI
        WIRE1_DELAY__60_US
        /* ------- */
        WIRE1_DQ_DIRI
        /* ------- */
        val >>= 1;
    }
}
#endif  /* wire1_send_byte */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
