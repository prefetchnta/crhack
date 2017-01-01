/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-07-28  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 模拟 SPI 函数库模板 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/* 管脚相关配置 */
#ifndef _SPI_INIT_
    #define _SPI_INIT_
#endif
#ifndef SPI_SSEL_DIRO
    #define SPI_SSEL_DIRO
#endif
#ifndef SPI_MOSI_DIRO
    #define SPI_MOSI_DIRO
#endif
#ifndef SPI_SCLK_DIRO
    #define SPI_SCLK_DIRO
#endif
#ifndef SPI_MISO_DIRI
    #define SPI_MISO_DIRI   SPI_MISO_SETB
#endif

/* 时钟相位选择 */
#ifndef SPI_CPOL
    #define SPI_CPOL    0
#endif
#ifndef SPI_CPHA
    #define SPI_CPHA    0
#endif

/* 器件数据长度 */
#ifndef spi_leng_t
    #define spi_leng_t  ufast_t
#endif

/*
==============================================================================
                驱动第一个数据位        驱动其他的数据位        采样数据
==============================================================================
CPOL=0,CPHA=0   第一个SCK上升沿前           SCK下降沿           SCK上升沿
CPOL=0,CPHA=1   第一个SCK上升沿             SCK上升沿           SCK下降沿
CPOL=1,CPHA=0   第一个SCK下降沿前           SCK上升沿           SCK下降沿
CPOL=1,CPHA=1   第一个SCK下降沿             SCK下降沿           SCK上升沿
==============================================================================
*/

/*
=======================================
    模拟 SPI 初始化
=======================================
*/
#if defined(spi_init)
CR_API void_t
spi_init (void_t)
{
    _SPI_INIT_
    /* ------- */
    SPI_SSEL_DIRO
    /* ------- */
    SPI_SSEL_SETB
    /* ------- */
    SPI_MOSI_DIRO
    /* ------- */
    SPI_MOSI_CLRB
    /* ------- */
    SPI_MISO_DIRI
    SPI_SCLK_DIRO
    /* ------- */
#if     (SPI_CPOL == 0)
    SPI_SCLK_CLRB
#elif   (SPI_CPOL != 0)
    SPI_SCLK_SETB
#endif
}
#endif  /* spi_init */

/*
=======================================
    模拟 SPI 接收从机的数据
=======================================
*/
#if defined(spi_recv_data)
CR_API void_t
spi_recv_data (
  __CR_OT__ void_t*     data,
  __CR_IN__ spi_leng_t  size
    )
{
    byte_t  val;
    ufast_t idx;

    SPI_SSEL_CLRB
#if     (SPI_CPHA == 0)
    SPI_SCLK_DELAY12
#endif
    for (; size != 0; size--)
    {
        val = 0;
        idx = 8;
        for (; idx != 0; idx--)
        {
#if     (SPI_CPHA == 0)
    #ifndef SPI_LSBF
            val <<= 1;
            if (SPI_MISO_GETB)
                val |= 0x01;
    #else
            val >>= 1;
            if (SPI_MISO_GETB)
                val |= 0x80;
    #endif
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12

#elif   (SPI_CPHA != 0)
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
    #ifndef SPI_LSBF
            val <<= 1;
            if (SPI_MISO_GETB)
                val |= 0x01;
    #else
            val >>= 1;
            if (SPI_MISO_GETB)
                val |= 0x80;
    #endif
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
#endif
        }
        *(uchar*)data = val;
        data = (uchar*)data + 1;
    }
    SPI_SSEL_SETB
}
#endif  /* spi_recv_data */

/*
=======================================
    模拟 SPI 发送数据到从机
=======================================
*/
#if defined(spi_send_data)
CR_API void_t
spi_send_data (
  __CR_IN__ const void_t*   data,
  __CR_IN__ spi_leng_t      size
    )
{
    byte_t  val;
    ufast_t idx;

    SPI_SSEL_CLRB
    for (; size != 0; size--)
    {
        idx = 8;
        val = *(uchar*)data;
        data = (uchar*)data + 1;
        for (; idx != 0; idx--)
        {
#if     (SPI_CPHA == 0)
    #ifndef SPI_LSBF
            if (val & 0x80)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val <<= 1;
    #else
            if (val & 0x01)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val >>= 1;
    #endif
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP

#elif   (SPI_CPHA != 0)
            SPI_SCLK_FLIP
    #ifndef SPI_LSBF
            if (val & 0x80)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val <<= 1;
    #else
            if (val & 0x01)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val >>= 1;
    #endif
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
#endif
        }
    }
#if     (SPI_CPHA == 0)
    SPI_SCLK_DELAY12
#endif
    SPI_SSEL_SETB
    SPI_MOSI_CLRB
}
#endif  /* spi_send_data */

/*
=======================================
    模拟 SPI 发送后接收数据
=======================================
*/
#if defined(spi_send_recv)
CR_API void_t
spi_send_recv (
  __CR_OT__ void_t*         recv_data,
  __CR_IN__ spi_leng_t      recv_size,
  __CR_IN__ const void_t*   send_data,
  __CR_IN__ spi_leng_t      send_size
    )
{
    byte_t  val;
    ufast_t idx;

    SPI_SSEL_CLRB   /* 发送命令 */
    for (; send_size != 0; send_size--)
    {
        val = *(uchar*)send_data;
        send_data = (uchar*)send_data + 1;
        for (idx = 8; idx != 0; idx--)
        {
#if     (SPI_CPHA == 0)
    #ifndef SPI_LSBF
            if (val & 0x80)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val <<= 1;
    #else
            if (val & 0x01)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val >>= 1;
    #endif
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP

#elif   (SPI_CPHA != 0)
            SPI_SCLK_FLIP
    #ifndef SPI_LSBF
            if (val & 0x80)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val <<= 1;
    #else
            if (val & 0x01)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val >>= 1;
    #endif
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
#endif
        }
    }
#if     (SPI_CPHA == 0)
    SPI_SCLK_DELAY12
#endif
    SPI_MOSI_CLRB   /* 接收数据 */
    for (; recv_size != 0; recv_size--)
    {
        for (val = 0, idx = 8; idx != 0; idx--)
        {
#if     (SPI_CPHA == 0)
    #ifndef SPI_LSBF
            val <<= 1;
            if (SPI_MISO_GETB)
                val |= 0x01;
    #else
            val >>= 1;
            if (SPI_MISO_GETB)
                val |= 0x80;
    #endif
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12

#elif   (SPI_CPHA != 0)
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
    #ifndef SPI_LSBF
            val <<= 1;
            if (SPI_MISO_GETB)
                val |= 0x01;
    #else
            val >>= 1;
            if (SPI_MISO_GETB)
                val |= 0x80;
    #endif
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
#endif
        }
        *(uchar*)recv_data = val;
        recv_data = (uchar*)recv_data + 1;
    }
    SPI_SSEL_SETB
}
#endif  /* spi_send_recv */

/*
=======================================
    模拟 SPI 发送数据到从机
=======================================
*/
#if defined(spi_send_send)
CR_API void_t
spi_send_send (
  __CR_IN__ const void_t*   send1_data,
  __CR_IN__ spi_leng_t      send1_size,
  __CR_IN__ const void_t*   send2_data,
  __CR_IN__ spi_leng_t      send2_size
    )
{
    byte_t  val;
    ufast_t idx;

    SPI_SSEL_CLRB
    for (; send1_size != 0; send1_size--)
    {
        idx = 8;
        val = *(uchar*)send1_data;
        send1_data = (uchar*)send1_data + 1;
        for (; idx != 0; idx--)
        {
#if     (SPI_CPHA == 0)
    #ifndef SPI_LSBF
            if (val & 0x80)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val <<= 1;
    #else
            if (val & 0x01)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val >>= 1;
    #endif
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP

#elif   (SPI_CPHA != 0)
            SPI_SCLK_FLIP
    #ifndef SPI_LSBF
            if (val & 0x80)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val <<= 1;
    #else
            if (val & 0x01)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val >>= 1;
    #endif
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
#endif
        }
    }
#if     (SPI_CPHA == 0)
    SPI_SCLK_DELAY12
#endif
    for (; send2_size != 0; send2_size--)
    {
        idx = 8;
        val = *(uchar*)send2_data;
        send2_data = (uchar*)send2_data + 1;
        for (; idx != 0; idx--)
        {
#if     (SPI_CPHA == 0)
    #ifndef SPI_LSBF
            if (val & 0x80)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val <<= 1;
    #else
            if (val & 0x01)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val >>= 1;
    #endif
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP

#elif   (SPI_CPHA != 0)
            SPI_SCLK_FLIP
    #ifndef SPI_LSBF
            if (val & 0x80)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val <<= 1;
    #else
            if (val & 0x01)
                SPI_MOSI_SETB
            else
                SPI_MOSI_CLRB
            val >>= 1;
    #endif
            SPI_SCLK_DELAY12
            SPI_SCLK_FLIP
            SPI_SCLK_DELAY12
#endif
        }
    }
#if     (SPI_CPHA == 0)
    SPI_SCLK_DELAY12
#endif
    SPI_SSEL_SETB
    SPI_MOSI_CLRB
}
#endif  /* spi_send_send */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
