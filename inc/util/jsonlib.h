/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2023-07-31  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>>>>> JSONLIB 头文件 <<<<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_JSONLIB_H__
#define __CR_JSONLIB_H__

#include "../defs.h"
#include "cjson/cJSON.h"
#include "cjson/cJSON_Utils.h"

/* 一些简化 cJSON 代码的助手函数 */
CR_API cJSON*   cjson_object (cJSON *node, const ansi_t *name);
CR_API ansi_t*  cjson_string (cJSON *node, const ansi_t *name);
CR_API ansi_t*  cjson_str_dup (cJSON *node, const ansi_t *name);
CR_API bool_t   cjson_str_cpy (ansi_t *dest, size_t size, cJSON *node,
                               const ansi_t *name);
CR_API bool_t   cjson_array_cnt (cJSON *node, const ansi_t *name,
                                 uint_t *count);
CR_API cJSON*   cjson_array_get (cJSON *node, uint_t index);
CR_API bool_t   cjson_int (cJSON *node, const ansi_t *name, sint_t *numb);
CR_API bool_t   cjson_int32 (cJSON *node, const ansi_t *name, int32s *numb);
CR_API bool_t   cjson_int64 (cJSON *node, const ansi_t *name, int64s *numb);
CR_API bool_t   cjson_real (cJSON *node, const ansi_t *name, real_t *numb);
CR_API bool_t   cjson_fp32 (cJSON *node, const ansi_t *name, fp32_t *numb);
CR_API bool_t   cjson_fp64 (cJSON *node, const ansi_t *name, fp64_t *numb);
CR_API bool_t   cjson_hex_dat (cJSON *node, const ansi_t *name, void_t *data,
                               leng_t size);
CR_API bool_t   cjson_hex_dat2 (cJSON *node, const ansi_t *name, void_t *data,
                                leng_t *size);

#endif  /* !__CR_JSONLIB_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
