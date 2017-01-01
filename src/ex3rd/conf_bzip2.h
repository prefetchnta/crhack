/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-14  */
/*     #######          ###    ###      [PORT]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack BZip2 压缩库的移植头文件 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_CONF_BZIP2_H__
#define __CR_CONF_BZIP2_H__

#include "applib.h"
#include "memlib.h"

/*
=======================================
    BZ_NO_STDIO 需要这么一个函数
=======================================
*/
cr_inline void_t
bz_internal_error (
  __CR_IN__ int errcode
    )
{
    CR_NOUSE(errcode);
    msg_stopA("bzip2 internal error", "crhack");
}

#define free    mem_free
#define calloc  mem_calloc
#define malloc  mem_malloc
#define BZ_NO_STDIO
#define BZ2_crc32Table  g_crc32a

/* [port] 关闭警告 */
#if     defined(_CR_CC_BCC_)
    #pragma warn -8004
    #pragma warn -8008
    #pragma warn -8057
    #pragma warn -8066

#elif   defined(_CR_CC_MSC_)
    #pragma warning (disable: 4100)
    #pragma warning (disable: 4127)
    #pragma warning (disable: 4244)

#elif   defined(_CR_CC_VDSP_)
    #pragma diag (suppress: 111)

#elif   defined(_CR_CC_ARMCC_NEW_)
    #pragma diag_suppress 111
    #pragma diag_suppress 550

#endif  /* CC TYPE predefines */

#endif  /* !__CR_CONF_BZIP2_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
