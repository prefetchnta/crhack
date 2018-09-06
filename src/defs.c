/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-16  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 配置检查函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "defs.h"

/*
=======================================
    检查内嵌汇编类型
=======================================
*/
CR_API int conf_check_asmx (void)
{
#if     defined(_CR_ASM_ATnT_)
    __asm__("nop");
#elif   defined(_CR_ASM_INTEL_)
    __asm {  nop  }
#endif
    return (123);
}

/*
=======================================
    检查系统位数配置
=======================================
*/
CR_API int conf_check_bits (void)
{
    int tt = 0;

#if     defined(_CR_SMALL_)
    tt += 123 / ((sizeof(uchar*) == 2) ? 1 : 0);
    tt += 123 / ((sizeof(dist_t) == 2) ? 1 : 0);
    tt += 123 / ((sizeof(leng_t) == 2) ? 1 : 0);
    tt += 123 / ((sizeof(size_t) == 2) ? 1 : 0);
#elif   defined(_CR_SYS16_) || \
        defined(_CR_SYS32_)
    tt += 123 / ((sizeof(uchar*) == 4) ? 1 : 0);
    tt += 123 / ((sizeof(dist_t) == 4) ? 1 : 0);
    tt += 123 / ((sizeof(leng_t) == 4) ? 1 : 0);
    #if defined(_CR_SYS16_)
    tt += 123 / ((sizeof(size_t) == 2) ? 1 : 0);
    #else   /* (_CR_SYS32_) */
    tt += 123 / ((sizeof(size_t) == 4) ? 1 : 0);
    #endif
#elif   defined(_CR_SYS64_)
    tt += 123 / ((sizeof(uchar*) == 8) ? 1 : 0);
    tt += 123 / ((sizeof(dist_t) == 8) ? 1 : 0);
    tt += 123 / ((sizeof(leng_t) == 8) ? 1 : 0);
    tt += 123 / ((sizeof(size_t) == 8) ? 1 : 0);
#else
    #error "defs.c: missing _CR_SYS**_ define!"
#endif
    return (tt / 4);
}

/*
=======================================
    检查文件位数配置
=======================================
*/
CR_API int conf_check_file (void)
{
    int tt = 0;

#if     defined(_CR_FILE32_)
    tt += 123 / ((sizeof(fdist_t) == 4) ? 1 : 0);
    tt += 123 / ((sizeof(fsize_t) == 4) ? 1 : 0);
#elif   defined(_CR_FILE64_)
    tt += 123 / ((sizeof(fdist_t) == 8) ? 1 : 0);
    tt += 123 / ((sizeof(fsize_t) == 8) ? 1 : 0);
#else
    #error "defs.c: missing _CR_FILE**_ define!"
#endif
    return (tt / 2);
}

/*
=======================================
    检查内存容量配置
=======================================
*/
CR_API int conf_check_mems (void)
{
    int tt = 0;

#if     defined(_CR_MSIZE32_)
    tt += 123 / ((sizeof(mdist_t) == 4) ? 1 : 0);
    tt += 123 / ((sizeof(msize_t) == 4) ? 1 : 0);
#elif   defined(_CR_MSIZE64_)
    tt += 123 / ((sizeof(mdist_t) == 8) ? 1 : 0);
    tt += 123 / ((sizeof(msize_t) == 8) ? 1 : 0);
#else
    #error "defs.c: missing _CR_MSIZE**_ define!"
#endif
    return (tt / 2);
}

/*
=======================================
    检查字节顺序配置
=======================================
*/
CR_API int conf_check_order (void)
{
    int xx = 1;

#if     defined(_CR_ORDER_LE_)
    return (123 / ((*(char*)(&xx) == 1) ? 1 : 0));
#elif   defined(_CR_ORDER_BE_)
    return (123 / ((*(char*)(&xx) == 0) ? 1 : 0));
#else
    #error "defs.c: missing _CR_ORDER_*E_ define!"
#endif
}

/*
=======================================
    检查基本类型配置
=======================================
*/
CR_API int conf_check_type (void)
{
    char ch = CR_AC('A') - CR_AC('B');
    int tt = 123 / ((ch <= 0) ? 1 : 0);

#ifndef _CR_NO_WIDE_
    tt += 123 / ((sizeof( L'A' ) == 2) ? 1 : 0);
#else
    tt += 123;
#endif
    tt += 123 / ((sizeof(fp32_t) == 4) ? 1 : 0);
    tt += 123 / ((sizeof(fp64_t) == 8) ? 1 : 0);
    tt += 123 / ((sizeof(byte_t) == 1) ? 1 : 0);
    tt += 123 / ((sizeof(int16u) == 2) ? 1 : 0);
    tt += 123 / ((sizeof(int32u) == 4) ? 1 : 0);
    tt += 123 / ((sizeof(int64u) == 8) ? 1 : 0);
    return (tt / 8);
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
