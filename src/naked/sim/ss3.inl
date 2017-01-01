/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-12-01  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 模拟3线 SSI 函数库模板 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/* 管脚相关配置 */
#ifndef _SS3_INIT_
    #define _SS3_INIT_
#endif
#ifndef SS3_SSEL_DIRO
    #define SS3_SSEL_DIRO
#endif
#ifndef SS3_SCLK_DIRO
    #define SS3_SCLK_DIRO
#endif
#ifndef SS3_SDIO_DIRO
    #define SS3_SDIO_DIRO
#endif
#ifndef SS3_SDIO_DIRI
    #define SS3_SDIO_DIRI   SS3_SDIO_SETB
#endif

/* 器件数据长度 */
#ifndef ss3_leng_t
    #define ss3_leng_t  ufast_t
#endif

/*
=======================================
    模拟3线 SSI 初始化
=======================================
*/
#if defined(ss3_init)
CR_API void_t
ss3_init (void_t)
{
    _SS3_INIT_
    /* ------- */
    SS3_SSEL_DIRO
    /* ------- */
    SS3_SSEL_SETB
    /* ------- */
    SS3_SDIO_DIRO
    /* ------- */
    SS3_SDIO_CLRB
    /* ------- */
    SS3_SCLK_DIRO
    /* ------- */
    SS3_SCLK_CLRB
}
#endif  /* ss3_init */

/*
=======================================
    模拟3线 SSI 发送数据到从机
=======================================
*/
#if defined(ss3_send_data)
CR_API void_t
ss3_send_data (
  __CR_IN__ const void_t*   data,
  __CR_IN__ ss3_leng_t      size
    )
{
    byte_t  val;
    ufast_t idx;

    SS3_SSEL_CLRB
    for (; size != 0; size--)
    {
        idx = 8;
        val = *(uchar*)data;
        data = (uchar*)data + 1;
        for (; idx != 0; idx--)
        {
    #ifndef SS3_MSBF
            if (val & 0x01)
                SS3_SDIO_SETB
            else
                SS3_SDIO_CLRB
            val >>= 1;
    #else
            if (val & 0x80)
                SS3_SDIO_SETB
            else
                SS3_SDIO_CLRB
            val <<= 1;
    #endif
            SS3_SCLK_DELAY12
            SS3_SCLK_SETB
            SS3_SCLK_DELAY12
            SS3_SCLK_CLRB
        }
    }
    SS3_SCLK_DELAY12
    SS3_SSEL_SETB
}
#endif  /* ss3_send_data */

/*
=======================================
    模拟3线 SSI 发送后接收数据
=======================================
*/
#if defined(ss3_send_recv)
CR_API void_t
ss3_send_recv (
  __CR_OT__ void_t*         recv_data,
  __CR_IN__ ss3_leng_t      recv_size,
  __CR_IN__ const void_t*   send_data,
  __CR_IN__ ss3_leng_t      send_size
    )
{
    byte_t  val;
    ufast_t idx;

    send_size--;
    SS3_SSEL_CLRB   /* 发送命令 */
    for (; send_size != 0; send_size--)
    {
        val = *(uchar*)send_data;
        send_data = (uchar*)send_data + 1;
        for (idx = 8; idx != 0; idx--)
        {
    #ifndef SS3_MSBF
            if (val & 0x01)
                SS3_SDIO_SETB
            else
                SS3_SDIO_CLRB
            val >>= 1;
    #else
            if (val & 0x80)
                SS3_SDIO_SETB
            else
                SS3_SDIO_CLRB
            val <<= 1;
    #endif
            SS3_SCLK_DELAY12
            SS3_SCLK_SETB
            SS3_SCLK_DELAY12
            SS3_SCLK_CLRB
        }
    }

    /* 最后一字节特殊处理 */
    val = *(uchar*)send_data;
    for (idx = 7; idx != 0; idx--)
    {
#ifndef SS3_MSBF
        if (val & 0x01)
            SS3_SDIO_SETB
        else
            SS3_SDIO_CLRB
        val >>= 1;
#else
        if (val & 0x80)
            SS3_SDIO_SETB
        else
            SS3_SDIO_CLRB
        val <<= 1;
#endif
        SS3_SCLK_DELAY12
        SS3_SCLK_SETB
        SS3_SCLK_DELAY12
        SS3_SCLK_CLRB
    }

    /* 最后一位下降沿前切换方向 */
#ifndef SS3_MSBF
    if (val & 0x01)
        SS3_SDIO_SETB
    else
        SS3_SDIO_CLRB
#else
    if (val & 0x80)
        SS3_SDIO_SETB
    else
        SS3_SDIO_CLRB
#endif
    SS3_SCLK_DELAY12
    SS3_SCLK_SETB
    SS3_SCLK_DELAY12
    /* ------- */
    SS3_SDIO_DIRI   /* 切换方向 */
    /* ------- */
    SS3_SCLK_DELAY12
    SS3_SCLK_CLRB
    SS3_SCLK_DELAY12
    for (; recv_size != 0; recv_size--)
    {
        for (val = 0, idx = 8; idx != 0; idx--)
        {
    #ifndef SS3_MSBF
            val >>= 1;
            if (SS3_SDIO_GETB)
                val |= 0x80;
    #else
            val <<= 1;
            if (SS3_SDIO_GETB)
                val |= 0x01;
    #endif
            SS3_SCLK_SETB
            SS3_SCLK_DELAY12
            SS3_SCLK_CLRB
            SS3_SCLK_DELAY12
        }
        *(uchar*)recv_data = val;
        recv_data = (uchar*)recv_data + 1;
    }
    SS3_SSEL_SETB
    /* ------- */
    SS3_SDIO_DIRO   /* 默认输出 */
    /* ------- */
}
#endif  /* ss3_send_recv */

/*
=======================================
    模拟3线 SSI 发送数据到从机
=======================================
*/
#if defined(ss3_send_send)
CR_API void_t
ss3_send_send (
  __CR_IN__ const void_t*   send1_data,
  __CR_IN__ ss3_leng_t      send1_size,
  __CR_IN__ const void_t*   send2_data,
  __CR_IN__ ss3_leng_t      send2_size
    )
{
    byte_t  val;
    ufast_t idx;

    SS3_SSEL_CLRB
    for (; send1_size != 0; send1_size--)
    {
        idx = 8;
        val = *(uchar*)send1_data;
        send1_data = (uchar*)send1_data + 1;
        for (; idx != 0; idx--)
        {
    #ifndef SS3_MSBF
            if (val & 0x01)
                SS3_SDIO_SETB
            else
                SS3_SDIO_CLRB
            val >>= 1;
    #else
            if (val & 0x80)
                SS3_SDIO_SETB
            else
                SS3_SDIO_CLRB
            val <<= 1;
    #endif
            SS3_SCLK_DELAY12
            SS3_SCLK_SETB
            SS3_SCLK_DELAY12
            SS3_SCLK_CLRB
        }
    }
    SS3_SCLK_DELAY12
    for (; send2_size != 0; send2_size--)
    {
        idx = 8;
        val = *(uchar*)send2_data;
        send2_data = (uchar*)send2_data + 1;
        for (; idx != 0; idx--)
        {
    #ifndef SS3_MSBF
            if (val & 0x01)
                SS3_SDIO_SETB
            else
                SS3_SDIO_CLRB
            val >>= 1;
    #else
            if (val & 0x80)
                SS3_SDIO_SETB
            else
                SS3_SDIO_CLRB
            val <<= 1;
    #endif
            SS3_SCLK_DELAY12
            SS3_SCLK_SETB
            SS3_SCLK_DELAY12
            SS3_SCLK_CLRB
        }
    }
    SS3_SCLK_DELAY12
    SS3_SSEL_SETB
}
#endif  /* ss3_send_send */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
