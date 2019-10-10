/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-05-22  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 模拟 I2C 函数库模板 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/* 管脚相关配置 */
#ifndef _I2C_INIT_
    #define _I2C_INIT_
#endif
#ifndef I2C_SCL_DIRO
    #define I2C_SCL_DIRO
#endif
#ifndef I2C_SDA_DIRO
    #define I2C_SDA_DIRO
#endif
#ifndef I2C_SDA_DIRI
    #define I2C_SDA_DIRI    I2C_SDA_SETB
#endif

/* 器件地址类型 */
#if     defined(_I2C_256B_)
    #define i2c_addr_t  byte_t
#elif   defined(_I2C_64KB_) || \
        defined(_I2C_2048B_)
    #define i2c_addr_t  int16u
#elif   defined(_I2C_LARGE_)
    #define i2c_addr_t  int32u
#else
    #error "i2c.inl: missing I2C TYPE define!"
#endif

/* 缺省的额外 ACK 等待 */
#ifndef I2C_DELAY_ACK_EX
    #define I2C_DELAY_ACK_EX
#endif

/* 缺省的额外读位等待 */
#ifndef I2C_DELAY_GET_EX
    #define I2C_DELAY_GET_EX
#endif

/* 器件数据长度 */
#ifndef i2c_leng_t
    #define i2c_leng_t  i2c_addr_t
#endif

/*
=======================================
    模拟 I2C 初始化
=======================================
*/
#if defined(i2c_init)
CR_API void_t
i2c_init (void_t)
{
    _I2C_INIT_
    /* ------ */
    I2C_SDA_DIRI
    I2C_SCL_DIRO
    /* ------ */
    I2C_SCL_SETB
}
#endif  /* i2c_init */

/*
=======================================
    模拟 I2C 起始信号
=======================================
*/
#if defined(i2c_enter)
CR_API void_t
i2c_enter (void_t)
{
    /* ------ */
    I2C_SDA_DIRO
    /* ------ */
    I2C_SDA_SETB
    I2C_SCL_SETB
    I2C_DELAY_4_7_US
    I2C_SDA_CLRB
    I2C_DELAY_4___US
    I2C_SCL_CLRB
    I2C_DELAY_4_7_US
}
#endif  /* i2c_enter */

/*
=======================================
    模拟 I2C 结束信号
=======================================
*/
#if defined(i2c_leave)
CR_API void_t
i2c_leave (void_t)
{
    /* ------ */
    I2C_SDA_DIRO
    /* ------ */
    I2C_SDA_CLRB
    I2C_SCL_SETB
    I2C_DELAY_4___US
    I2C_SDA_SETB
    I2C_DELAY_4_7_US
    /* ------ */
    I2C_SDA_DIRI
    /* ------ */
}
#endif  /* i2c_leave */

/*
=======================================
    模拟 I2C 释放总线
=======================================
*/
#if defined(i2c_release)
CR_API void_t
i2c_release (void_t)
{
    ufast_t idx;

    I2C_SCL_CLRB
    /* ------ */
    I2C_SDA_DIRI
    /* ------ */
    I2C_DELAY_ACK_EX
    if (I2C_SDA_GETB)
        return;
    for (idx = 9; idx != 0; idx--) {
        I2C_SCL_CLRB
        I2C_DELAY_4_7_US
        I2C_SCL_SETB
        I2C_DELAY_4___US
    }
    I2C_SCL_CLRB
}
#endif  /* i2c_release */

/*
=======================================
    模拟 I2C 发送 ACK 信号
=======================================
*/
#if defined(i2c_send_ack)
CR_API void_t
i2c_send_ack (void_t)
{
    /* ------ */
    I2C_SDA_DIRO
    /* ------ */
    I2C_SDA_CLRB
    I2C_SCL_SETB
    I2C_DELAY_4___US
    I2C_SCL_CLRB
    I2C_DELAY_4_7_US
}
#endif  /* i2c_send_ack */

/*
=======================================
    模拟 I2C 发送 NACK 信号
=======================================
*/
#if defined(i2c_send_nack)
CR_API void_t
i2c_send_nack (void_t)
{
    /* ------ */
    I2C_SDA_DIRO
    /* ------ */
    I2C_SDA_SETB
    I2C_SCL_SETB
    I2C_DELAY_4___US
    I2C_SCL_CLRB
    I2C_DELAY_4_7_US
}
#endif  /* i2c_send_nack */

/*
=======================================
    模拟 I2C 检测 ACK 信号
=======================================
*/
#if defined(i2c_test_ack)
CR_API retc_t
i2c_test_ack (void_t)
{
    retc_t  ack;

    /* ------ */
    I2C_SDA_DIRI
    /* ------ */
    I2C_SCL_SETB
    I2C_DELAY_4___US
    I2C_DELAY_ACK_EX
    if (I2C_SDA_GETB)
        ack = FALSE;
    else
        ack = TRUE;
    I2C_DELAY_4_7_US
    I2C_SCL_CLRB
    I2C_DELAY_4___US
    return (ack);
}
#endif  /* i2c_test_ack */

/*
=======================================
    模拟 I2C 接收一个字节
=======================================
*/
#if defined(i2c_recv_byte)
CR_API byte_t
i2c_recv_byte (void_t)
{
    byte_t  val = 0;
    ufast_t idx = 8;

    /* ------ */
    I2C_SDA_DIRI
    /* ------ */
    for (; idx != 0; idx--)
    {
        I2C_SCL_CLRB
        I2C_DELAY_4_7_US
        I2C_SCL_SETB
        I2C_DELAY_GET_EX
        val <<= 1;
        if (I2C_SDA_GETB)
            val |= 1;
        I2C_DELAY_4___US
    }
    I2C_SCL_CLRB
    I2C_DELAY_4_7_US
    return (val);
}
#endif  /* i2c_recv_byte */

/*
=======================================
    模拟 I2C 发送一个字节
=======================================
*/
#if defined(i2c_send_byte)
CR_API retc_t
i2c_send_byte (
  __CR_IN__ byte_t  val
    )
{
    ufast_t idx = 8;

    /* ------ */
    I2C_SDA_DIRO
    /* ------ */
    for (; idx != 0; idx--)
    {
        if (val & 0x80)
            I2C_SDA_SETB
        else
            I2C_SDA_CLRB
        val <<= 1;
        I2C_SCL_SETB
        I2C_DELAY_4___US
        I2C_SCL_CLRB
        I2C_DELAY_4_7_US
    }
    return (i2c_test_ack());
}
#endif  /* i2c_send_byte */

/*
=======================================
    模拟 I2C 发送器件地址
=======================================
*/
#if defined(i2c_send_slave)
CR_API retc_t
i2c_send_slave (
  __CR_IO__ byte_t*     devs,
  __CR_IN__ i2c_addr_t  addr
    )
{
    byte_t  temp = *devs;

    /* <= 256B 的器件 */
#if     defined(_I2C_256B_)

    CR_NOUSE(addr);

    /* <= 2048B 的器件 */
#elif   defined(_I2C_2048B_)

    /* 地址过滤 */
    if (addr >= 2048)
        return (FALSE);
    temp |= (byte_t)(addr >> 7) & 0x0E;
    *devs = temp;

#elif   !defined(_I2C_64KB_)

    /* 地址过滤 */
    if (addr >= 524288UL)
        return (FALSE);
    temp |= (byte_t)(addr >> 15) & 0x0E;
    *devs = temp;

#endif  /* device size predefines */

    /* 发送起始位 */
    i2c_enter();

    /* 输出器件地址 */
    return (i2c_send_byte(temp));
}
#endif  /* i2c_send_slave */

/*
=======================================
    模拟 I2C 发送数据地址
=======================================
*/
#if defined(i2c_send_address)
CR_API retc_t
i2c_send_address (
  __CR_IN__ i2c_addr_t  addr
    )
{
    /* <= 2048B 的器件 */
#if defined(_I2C_256B_) || \
    defined(_I2C_2048B_)

    /* 输出8位地址 */
    if (!i2c_send_byte((byte_t)addr))
        return (FALSE);

    /* > 2048B 的器件 */
#else
    byte_t  hi = (byte_t)(addr >> 8);

    /* 输出16位地址 */
    if (!i2c_send_byte((byte_t)(hi)))
        return (FALSE);
    if (!i2c_send_byte((byte_t)addr))
        return (FALSE);

#endif  /* device size predefines */

    return (TRUE);
}
#endif  /* i2c_send_address */

/*
=======================================
    模拟 I2C 读取器件地址数据
=======================================
*/
#if defined(i2c_read_device)
CR_API i2c_leng_t
i2c_read_device (
  __CR_IN__ byte_t      devs,
  __CR_IN__ i2c_addr_t  addr,
  __CR_OT__ void_t*     data,
  __CR_IN__ i2c_leng_t  size
    )
{
    i2c_leng_t  ptr;

    /* 过滤长度 */
    if (size == 0)
        return (0);
    ptr = 0;
    devs &= 0xFE;

    /* 指定读取的地址 */
    if (!i2c_send_slave(&devs, addr))
        goto _func_out;
    if (!i2c_send_address(addr))
        goto _func_out;

    /* 发送起始位 */
    i2c_enter();

    /* 输出器件地址 */
    if (!i2c_send_byte(devs | 0x01))
        goto _func_out;
    size--;

    /* 开始连续读取数据 */
    for (; ptr < size; ptr++) {
        *(uchar*)data = i2c_recv_byte();
        i2c_send_ack();
        data = (uchar*)data + 1;
    }

    /* 最后一个字节发 NACK */
    *(uchar*)data = i2c_recv_byte();
    i2c_send_nack();
    ptr++;

_func_out:
    i2c_leave();
    return (ptr);
}
#endif  /* i2c_read_device */

/*
=======================================
    模拟 I2C 写入器件地址数据
=======================================
*/
#if defined(i2c_write_device)
CR_API i2c_leng_t
i2c_write_device (
  __CR_IN__ byte_t          devs,
  __CR_IN__ i2c_addr_t      addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ i2c_leng_t      size
    )
{
    i2c_leng_t  ptr;

    /* 过滤长度 */
    if (size == 0)
        return (0);
    ptr = 0;
    devs &= 0xFE;

    /* 指定写入的地址 */
    if (!i2c_send_slave(&devs, addr))
        goto _func_out;
    if (!i2c_send_address(addr))
        goto _func_out;

    /* 开始连续写入数据 */
    for (; ptr < size; ptr++) {
        if (!i2c_send_byte(*(uchar*)data))
            break;
        data = (uchar*)data + 1;
    }

_func_out:
    i2c_leave();
    return (ptr);
}
#endif  /* i2c_write_device */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
