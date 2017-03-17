/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-16  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack AT 外设驱动函数库模板 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "strlib.h"

/* 断流的默认时间 (50ms) */
#if !defined(CUT_DOWN_TIME)
    #define CUT_DOWN_TIME   50
#endif

/*
=======================================
    命令收发
=======================================
*/
CR_API ansi_t*
at_iorw (
  __CR_OT__ ansi_t*         out,
  __CR_IN__ const ansi_t*   inp,
  __CR_IN__ uint_t          size,
  __CR_IN__ uint_t          tout
    )
{
    uint_t  count;

    uart_rx_flush();
    uart_send_str(inp);
    count = uart_wait(NULL, CUT_DOWN_TIME, tout);
    if (count == 0 || count + 1 > size)
        return (NULL);
    uart_read(out, count);
    out[count] = 0x00;
    return (out);
}

/*
=======================================
    AT
=======================================
*/
CR_API bool_t
at_check (
  __CR_IN__ uint_t  tout
    )
{
    ansi_t  buf[16];

    /* AT 命令测试 */
    if (at_iorw(buf, "AT\r", sizeof(buf), tout) == NULL)
        return (FALSE);
    if (str_strA(buf, "OK") == NULL)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    AT+CGMM
=======================================
*/
CR_API bool_t
at_cgmm (
  __CR_OT__ ansi_t* out,
  __CR_IN__ uint_t  size,
  __CR_IN__ uint_t  tout
    )
{
    /* 返回模块类型 */
    if (at_iorw(out, "AT+CGMM\r", size, tout) == NULL)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    AT+CSQ
=======================================
*/
CR_API bool_t
at_csq (
  __CR_OT__ uint_t* rssi,
  __CR_IN__ uint_t  tout
    )
{
    ansi_t  buf[32], *ptr;

    /* 返回信号质量 */
    if (at_iorw(buf, "AT+CSQ\r", sizeof(buf), tout) == NULL)
        return (FALSE);
    ptr = str_strA(buf, "+CSQ: ");
    if (ptr == NULL)
        return (FALSE);
    *rssi = str2intA(ptr + 6, NULL);
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
