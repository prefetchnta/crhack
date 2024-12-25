/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-01-01  */
/*     #######          ###    ###      [UTIL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>> LLB3 通讯协议函数库 <<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"
#include "util/commlib.h"

/* 串口命令值 */
#define COMM_GET_T      0   /* 获取瞬时温度 */
#define COMM_GET_A      1   /* 获取瞬时常温 */
#define COMM_GET_P      2   /* 获取瞬时压力 */
#define COMM_GET_D      3   /* 获取瞬时差压 */
#define COMM_GET_F      4   /* 获取瞬时流量 */
#define COMM_GET_H      5   /* 获取瞬时热量 */
#define COMM_GET_SQ     6   /* 获取流量累计 */
#define COMM_GET_SH     7   /* 获取热量累计 */

/* ADC 数值 */
typedef struct
{
        int16u  value;
        int16u  range;

} sLLB3X_ADC;

/* 数据提取用 */
typedef union
{
        fp32_t      fp32;
        int32s      it32;
        byte_t      by[4];
        sLLB3X_ADC  vadc;

} sLLB3X;

/*
---------------------------------------
    LLB3 UART 交互
---------------------------------------
*/
static bool_t
llb3_sio_iorw (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr,
  __CR_IN__ byte_t  cmds,
  __CR_OT__ byte_t* value
    )
{
    uint_t  back;
    byte_t  buf[14];

    /* 发送命令 */
    buf[0] =  0xAA;
    buf[1] =  0x01;
    buf[2] =  addr;
    buf[3] = ~addr;
    buf[4] =  cmds;
    buf[5] = ~cmds;
    sio_flush(port, CR_SIO_FLU_RT);
    back = sio_write(port, buf, 6);
    if (back != 6)
        return (FALSE);

    /* 等待回包 */
    do {
        back = sio_read(port, buf, 1);
        if (back != 1)
            return (FALSE);
    } while (buf[0] != 0xAA);

    back = sio_read(port, &buf[1], 13);
    if (back != 13)
        return (FALSE);

    /* 数据校验 */
    if (buf[0] != 0xAA || buf[ 1] != 0x00 || buf[ 2] != addr ||
        buf[3] != 0x00 || buf[ 5] != 0x00 || buf[ 7] != 0x00 ||
        buf[9] != 0x00 || buf[11] != 0x00 || buf[13] != 0x00)
        return (FALSE);
    for (back = 1; back < 14; back++)
        buf[0] ^= buf[back];
    if (buf[0] != 0xFF)
        return (FALSE);
    value[0] = buf[4];
    value[1] = buf[6];
    value[2] = buf[8];
    value[3] = buf[10];
    return (TRUE);
}

/*
---------------------------------------
    LLB3 UART 获取浮点值
---------------------------------------
*/
static fp32_t
llb3_sio_get_fp32 (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr,
  __CR_IN__ byte_t  cmds
    )
{
    sLLB3X  cnvt;
    byte_t  buf[4];

    /* 发送命令 */
    if (!llb3_sio_iorw(port, addr, cmds, buf))
        return (CR_PHY_INV);

    /* 提取数据 */
#ifndef _CR_ORDER_BE_
    cnvt.by[3] = buf[0];
    cnvt.by[2] = buf[1];
    cnvt.by[1] = buf[2];
    cnvt.by[0] = buf[3];
#else
    cnvt.by[0] = buf[0];
    cnvt.by[1] = buf[1];
    cnvt.by[2] = buf[2];
    cnvt.by[3] = buf[3];
#endif
    return (cnvt.fp32);
}

/*
---------------------------------------
    LLB3 UART 获取转换值
---------------------------------------
*/
static int16u
llb3_sio_get_ad16 (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr,
  __CR_IN__ byte_t  cmds,
  __CR_OT__ int16u* maxv
    )
{
    sLLB3X  cnvt;
    byte_t  buf[4];

    /* 过滤参数 */
    *maxv = 0;
    if (cmds & 0x80)
        return (0);
    cmds |= 0x80;

    /* 发送命令 */
    if (!llb3_sio_iorw(port, addr, cmds, buf))
        return (0);

    /* 提取数据 */
#ifndef _CR_ORDER_BE_
    cnvt.by[1] = buf[0];
    cnvt.by[0] = buf[1];
    cnvt.by[3] = buf[2];
    cnvt.by[2] = buf[3];
#else
    cnvt.by[0] = buf[0];
    cnvt.by[1] = buf[1];
    cnvt.by[2] = buf[2];
    cnvt.by[3] = buf[3];
#endif
    *maxv = cnvt.vadc.range;
    return (cnvt.vadc.value);
}

/*
=======================================
    LLB3 UART 获取温度
=======================================
*/
CR_API fp32_t
llb3_sio_get_tc (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr
    )
{
    return (llb3_sio_get_fp32(port, addr, COMM_GET_T));
}

/*
=======================================
    LLB3 UART 获取常温
=======================================
*/
CR_API fp32_t
llb3_sio_get_ta (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr
    )
{
    return (llb3_sio_get_fp32(port, addr, COMM_GET_A));
}

/*
=======================================
    LLB3 UART 获取压力
=======================================
*/
CR_API fp32_t
llb3_sio_get_pc (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr
    )
{
    return (llb3_sio_get_fp32(port, addr, COMM_GET_P));
}

/*
=======================================
    LLB3 UART 获取差压
=======================================
*/
CR_API fp32_t
llb3_sio_get_dc (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr
    )
{
    return (llb3_sio_get_fp32(port, addr, COMM_GET_D));
}

/*
=======================================
    LLB3 UART 获取流量
=======================================
*/
CR_API fp32_t
llb3_sio_get_fc (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr
    )
{
    return (llb3_sio_get_fp32(port, addr, COMM_GET_F));
}

/*
=======================================
    LLB3 UART 获取热量
=======================================
*/
CR_API fp32_t
llb3_sio_get_hc (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr
    )
{
    return (llb3_sio_get_fp32(port, addr, COMM_GET_H));
}

/*
=======================================
    LLB3 UART 获取累计流量
=======================================
*/
CR_API fp32_t
llb3_sio_get_sq (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr
    )
{
    return (llb3_sio_get_fp32(port, addr, COMM_GET_SQ));
}

/*
=======================================
    LLB3 UART 获取累计热量
=======================================
*/
CR_API fp32_t
llb3_sio_get_sh (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr
    )
{
    return (llb3_sio_get_fp32(port, addr, COMM_GET_SH));
}

/*
=======================================
    LLB3 UART 获取直接取值
=======================================
*/
CR_API int16u
llb3_sio_get_ad (
  __CR_IN__ uint_t  port,
  __CR_IN__ byte_t  addr,
  __CR_IN__ byte_t  chnn,
  __CR_OT__ int16u* maxv
    )
{
    return (llb3_sio_get_ad16(port, addr, chnn, maxv));
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
