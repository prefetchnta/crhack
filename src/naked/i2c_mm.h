/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-08-29  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack I2C 存储器件公用头文件 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_I2C_MM_H__
#define __CR_I2C_MM_H__

/* 器件地址大小 */
#if     defined(_I2C_SIZE_)
    #if     (_I2C_SIZE_ - 1 <= 255)
        #define i2c_addr_t  byte_t
    #elif   (_I2C_SIZE_ - 1 <= 65535)
        #define i2c_addr_t  int16u
    #else
        #define i2c_addr_t  int32u
    #endif
#else
    #error "i2c_mm.h: missing I2C SIZE define!"
#endif

/* 器件分页大小 */
#if     defined(_I2C_PAGE_)
    #if     (_I2C_PAGE_ <= 255)
        #define i2c_page_t  byte_t
    #elif   (_I2C_PAGE_ <= 65535)
        #define i2c_page_t  int16u
    #else
        #define i2c_page_t  int32u
    #endif
    #if     (_I2C_SIZE_ / _I2C_PAGE_ <= 255)
        #define i2c_blks_t  ufast_t
    #elif   (_I2C_SIZE_ / _I2C_PAGE_ <= 65535)
        #define i2c_blks_t  uint_t
    #else
        #define i2c_blks_t  int32u
    #endif
#endif

/* 器件数据长度 */
#ifndef i2c_leng_t
    #define i2c_leng_t  i2c_addr_t
#endif

#endif  /* !__CR_I2C_MM_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
