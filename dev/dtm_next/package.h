/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-01-05  */
/*     #######          ###    ###      [SOFT]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> DTM_NEXT 程序解包头文件 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_PACKAGE_H__
#define __CR_PACKAGE_H__

#include "defs.h"

/* 解包后的程序结构 */
typedef struct
{
        ansi_t*     version;    /*  程序版本名字符串  */
        int32u      car_ver;    /*  车载机版本 X.X.X  */
        int32u      smu_ver;    /*  模拟机版本 X.X.X  */
        int32u      the_ver;    /*  理论机版本 X.X.X  */
        int32u      pro_size;   /*  主程序的字节大小  */
        int32u      hzk_size;   /*  汉字库的字节大小  */
        byte_t*     all_data;   /* 主程序和汉字库数据 */

} sDTM_PRO;

/* 程序版本类型值 */
#define PRO_TYPE_INV    0   /* 非法类型 */
#define PRO_TYPE_CAR    1   /* 车载类型 */
#define PRO_TYPE_SMU    2   /* 模拟类型 */
#define PRO_TYPE_THE    3   /* 理论类型 */

CR_API void_t   dtm_pack_free (sDTM_PRO *outp);
CR_API uint_t   dtm_check_ver (const ansi_t *file, ansi_t **vers);
CR_API bool_t   dtm_old_unpack (sDTM_PRO *outp, const void_t *data,
                                leng_t size, const ansi_t *unkey);
CR_API bool_t   dtm_new_unpack (sDTM_PRO *outp, const void_t *data,
                                leng_t size, const ansi_t *unkey);
CR_API bool_t   dtm_all_unpack (sDTM_PRO *outp, const void_t *data,
                                leng_t size, const ansi_t *unkey);

#endif  /* !__CR_PACKAGE_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
