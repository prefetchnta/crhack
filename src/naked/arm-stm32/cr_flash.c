/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-11  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack FLASH 函数库 for STM32 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"
#include "memlib.h"
#include "stm32cpu.h"

#if defined(_CR_STM32F10X_)

/*
=======================================
    读取 FLASH 数据
=======================================
*/
CR_API void_t
flash_read (
  __CR_IN__ int32u  addr,
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  size
    )
{
    mem_cpy(data, (void_t*)addr, size);
}

/*
=======================================
    写入 FLASH 数据
=======================================
*/
CR_API bool_t
flash_prog (
  __CR_IN__ int32u          addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    int32u  adr, tmp;
    uint_t  blk, rst, idx;
    /* ---------------- */
    const void_t*   ptr;

    /* 只能按页写 */
    if (addr % STM32FLASH_PAGE != 0)
        return (FALSE);
    adr = addr;
    ptr = data;

    /* 按四字节写 */
    blk = size / sizeof(int32u);
    rst = size % sizeof(int32u);
    FLASH_Unlock();
    for (idx = 0; idx < blk; idx++) {
        if (adr % STM32FLASH_PAGE == 0)
            FLASH_ErasePage(adr);
        mem_cpy(&tmp, ptr, sizeof(int32u));
        ptr = (int32u*)ptr + 1;
        FLASH_ProgramWord(adr, tmp);
        adr += sizeof(int32u);
    }
    if (rst != 0) {
        if (adr % STM32FLASH_PAGE == 0)
            FLASH_ErasePage(adr);
        tmp = 0;
        mem_cpy(&tmp, ptr, rst);
        FLASH_ProgramWord(adr, tmp);
    }
    FLASH_Lock();

    /* 回读比较 */
    if (mem_cmp((void_t*)addr, data, size) != 0)
        return (FALSE);
    return (TRUE);
}

#endif  /* _CR_STM32F10X_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
