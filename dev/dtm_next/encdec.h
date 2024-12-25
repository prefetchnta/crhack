/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-11-15  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> DTM_NEXT 加密解密头文件 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_ENCDEC_H__
#define __CR_ENCDEC_H__

#include "defs.h"

/*****************************************************************************/
/*                               DTM_NEW 算法                                */
/*****************************************************************************/

CR_API void_t   dtmn_dat_crypto (void_t *data, leng_t size);
CR_API void_t   dtmn_luj_encode (void_t *data, leng_t size);
CR_API void_t   dtmn_luj_decode (void_t *data, leng_t size);
CR_API void_t   dtmn_net_encode (void_t *data, leng_t size,
                                 const byte_t key[16]);
CR_API void_t   dtmn_net_decode (void_t *data, leng_t size,
                                 const byte_t key[16]);
CR_API void_t   dtmn_hash_passw (byte_t hash[16], const void_t *data,
                                 leng_t size);
CR_API void_t   dtmn_adsz_encode (void_t *data, int32u addr, int32u size,
                                  const byte_t *ukey CR_DEFAULT(NULL));
CR_API void_t   dtmn_adsz_decode (void_t *data, int32u addr, int32u size,
                                  const byte_t *ukey CR_DEFAULT(NULL));

/*****************************************************************************/
/*                               DTM_OLD 算法                                */
/*****************************************************************************/

CR_API void_t   dtmo_unkey_icc (byte_t dst[16], const byte_t src[64]);
CR_API void_t   dtmo_unkey_4428 (byte_t dst[3], const byte_t src[64]);
CR_API void_t   dtmo_api_encode (void_t *data, leng_t size,
                                 const byte_t key[16]);
CR_API void_t   dtmo_api_decode (void_t *data, leng_t size,
                                 const byte_t key[16]);
CR_API bool_t   dtmo_api_search (byte_t key[16], void_t *version,
                                 leng_t size_w_sum);
CR_API void_t   dtmo_luj_encode (void_t *data, leng_t size,
                                 const byte_t key[16],
                                 const byte_t *userkey CR_DEFAULT(NULL));
CR_API void_t   dtmo_luj_decode (void_t *data, leng_t size,
                                 const byte_t key[16],
                                 const byte_t *userkey CR_DEFAULT(NULL));
CR_API void_t   dtmo_jjm_crypto (void_t *dst, const void_t *src, leng_t size,
                                 const void_t *key, leng_t keylen);
CR_API void_t   dtmo_sc232_encode (void_t *data, leng_t size);
CR_API void_t   dtmo_sc232_decode (void_t *data, leng_t size);
CR_API bool_t   wlfid_str_encode (ansi_t *dst, const ansi_t *src,
                                  const ansi_t *key);
CR_API bool_t   wlfid_str_decode (ansi_t *dst, const ansi_t *src,
                                  const ansi_t *key);

#endif  /* !__CR_ENCDEC_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
