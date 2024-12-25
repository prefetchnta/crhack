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
/*  >>>>>>>>>>>>>>>>>>>>>>> DTM_NEXT 加密解密函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "crypto.h"
#include "encdec.h"
#include "memlib.h"
#include "morder.h"
#include "strlib.h"

/* 加密解密用常数密钥 */
static const byte_t _rom_ s_userkey1[16] =
{
    0x98, 0x05, 0x15, 0xFB, 0xC4, 0x76, 0xDC, 0x32,
    0x3D, 0xFC, 0x65, 0x10, 0x28, 0x67, 0x82, 0x99,
};

static const byte_t _rom_ s_userkeyA[16] =
{
    0xD3, 0xCF, 0x10, 0x28, 0xBF, 0x67, 0xA2, 0xAA,
    0x89, 0xD5, 0xA5, 0x65, 0x4C, 0x87, 0xEF, 0x07,
};

static const byte_t _rom_ s_userkeyB[16] =
{
    0xAD, 0x98, 0x3F, 0x43, 0xA0, 0x76, 0x23, 0x1F,
    0xD9, 0x86, 0xCE, 0xEA, 0xC4, 0x56, 0x43, 0x55,
};

/* 四川通讯及 IC 卡密钥 */
static const byte_t _rom_ s_sc_ickey[16] =
{
    0x00, 0x30, 0x1B, 0x49, 0x1B, 0x73, 0x0E, 0x86,
    0xC9, 0x78, 0x14, 0x01, 0xCE, 0xB1, 0x08, 0xED,
};

/* 几个常用的读取传输密钥 */
static const byte_t _rom_ * _rom_ s_keylist[] = {
(byte_t*)"\x24\x33\x8F\x95\xC4\xB7\x55\x75\x2B\x80\x1A\x55\x90\xCB\xEE\xFE",
(byte_t*)"\x81\x33\x11\x67\x6E\x78\xC1\x37\x87\x34\x48\xA1\x17\x5E\xC2\x73",
(byte_t*)"\xC4\xB7\x55\x24\x33\x8F\x95\x1A\x55\x90\xCB\x75\x2B\x80\xEE\xFE",
};

/*****************************************************************************/
/*                               DTM_NEW 算法                                */
/*****************************************************************************/

/*
=======================================
    数据简单混淆处理
=======================================
*/
CR_API void_t
dtmn_dat_crypto (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    int32u  aa = 63689UL;
    int32u  bb = 378551UL;

    /* 加密解密在一起的 */
    for (; size != 0; size--) {
        aa *= bb;
        *(byte_t*)data ^= (byte_t)(aa >> 16);
        data = (byte_t*)data + 1;
    }
}

/*
=======================================
    手持机普通数据加密
=======================================
*/
CR_API void_t
dtmn_luj_encode (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    /* 直接使用常数密钥加密 */
    dtmo_luj_encode(data, size, s_userkeyB, NULL);
}

/*
=======================================
    手持机普通数据解密
=======================================
*/
CR_API void_t
dtmn_luj_decode (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    /* 直接使用常数密钥解密 */
    dtmo_luj_decode(data, size, s_userkeyB, NULL);
}

/*
=======================================
    网络传输数据包加密
=======================================
*/
CR_API void_t
dtmn_net_encode (
  __CR_IO__ void_t*         data,
  __CR_IN__ leng_t          size,
  __CR_IN__ const byte_t    key[16]
    )
{
    leng_t  idx;
    int32u* ptr = (int32u*)data;

    size /= 8;
    for (idx = 0; idx < size; idx++) {
        crypto_tea_enc(ptr, (int32u*)key, 16);
        ptr += 2;
    }
}

/*
=======================================
    网络传输数据包解密
=======================================
*/
CR_API void_t
dtmn_net_decode (
  __CR_IO__ void_t*         data,
  __CR_IN__ leng_t          size,
  __CR_IN__ const byte_t    key[16]
    )
{
    leng_t  idx;
    int32u* ptr = (int32u*)data;

    size /= 8;
    for (idx = 0; idx < size; idx++) {
        crypto_tea_dec(ptr, (int32u*)key, 16);
        ptr += 2;
    }
}

/*
=======================================
    生成登录密码哈希
=======================================
*/
CR_API void_t
dtmn_hash_passw (
  __CR_OT__ byte_t          hash[16],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    uint_t  idx;
    byte_t  md4[16];
    byte_t  md5[16];

    hash_md4_total(md4, data, size);
    hash_md5_total(md5, data, size);
    for (idx = 0; idx < 16; idx++)
        hash[idx] = (md5[idx] ^ md4[15 - idx]);
}

/*
=======================================
    可变密钥加密算法
=======================================
*/
CR_API void_t
dtmn_adsz_encode (
  __CR_IO__ void_t*         data,
  __CR_IN__ int32u          addr,
  __CR_IN__ int32u          size,
  __CR_IN__ const byte_t*   ukey
    )
{
    uint_t  idx;
    int32u  key1, key2;
    int16u  hash1, *p16;
    int32u  hash2, *p32;
    byte_t  userkey[16];

    /* 默认的常数密钥 */
    if (ukey == NULL)
        ukey = (byte_t*)s_userkeyA;
    key1 = DWORD_LE(addr * size + addr);
    key2 = DWORD_LE(addr * size + size);
    hash1 = hash_crc16c_total(&key1, sizeof(key1));
    hash2 = hash_crc32i_total(&key2, sizeof(key2));
    for (idx = 0; idx < sizeof(userkey); idx++)
        userkey[idx] = (byte_t)((~ukey[idx]) ^ addr);
    p16 = (int16u*)(&userkey[14]);
    p32 = (int32u*)(&userkey[ 0]);
    p16[0] = WORD_LE(~hash1);
    p32[0] = DWORD_LE(hash2);
    dtmo_luj_encode(data, size, userkey, NULL);
    dtmn_net_encode(data, size, userkey);
}

/*
=======================================
    可变密钥解密算法
=======================================
*/
CR_API void_t
dtmn_adsz_decode (
  __CR_IO__ void_t*         data,
  __CR_IN__ int32u          addr,
  __CR_IN__ int32u          size,
  __CR_IN__ const byte_t*   ukey
    )
{
    uint_t  idx;
    int32u  key1, key2;
    int16u  hash1, *p16;
    int32u  hash2, *p32;
    byte_t  userkey[16];

    /* 默认的常数密钥 */
    if (ukey == NULL)
        ukey = (byte_t*)s_userkeyA;
    key1 = DWORD_LE(addr * size + addr);
    key2 = DWORD_LE(addr * size + size);
    hash1 = hash_crc16c_total(&key1, sizeof(key1));
    hash2 = hash_crc32i_total(&key2, sizeof(key2));
    for (idx = 0; idx < sizeof(userkey); idx++)
        userkey[idx] = (byte_t)((~ukey[idx]) ^ addr);
    p16 = (int16u*)(&userkey[14]);
    p32 = (int32u*)(&userkey[ 0]);
    p16[0] = WORD_LE(~hash1);
    p32[0] = DWORD_LE(hash2);
    dtmn_net_decode(data, size, userkey);
    dtmo_luj_decode(data, size, userkey, NULL);
}

/*****************************************************************************/
/*                               DTM_OLD 算法                                */
/*****************************************************************************/

/*
=======================================
    IC 卡密钥解密
=======================================
*/
CR_API void_t
dtmo_unkey_icc (
  __CR_OT__ byte_t          dst[16],
  __CR_IN__ const byte_t    src[64]
    )
{
    uint_t  idx;

    /* 直接密钥 */
    if (src[0] == 0x00) {
        mem_cpy(dst, src, 16);
        return;
    }

    /* 转换密钥 */
    for (idx = 0; idx < 16; idx++)
        dst[idx] = (byte_t)(src[1 + idx] ^ src[1 + 2 * idx]);
}

/*
=======================================
    4428 卡密码生成
=======================================
*/
CR_API void_t
dtmo_unkey_4428 (
  __CR_OT__ byte_t          dst[3],
  __CR_IN__ const byte_t    src[64]
    )
{
    uint_t  idx;

    for (idx = 0; idx < 3; idx++)
        dst[idx] = (byte_t)(src[1 + idx] + src[1 + 2 * idx]);
}

/*
=======================================
    动态库用加密算法
=======================================
*/
CR_API void_t
dtmo_api_encode (
  __CR_IO__ void_t*         data,
  __CR_IN__ leng_t          size,
  __CR_IN__ const byte_t    key[16]
    )
{
    leng_t  ii, jj, kk;
    byte_t  byte1, byte2, byte3, byte4;
    byte_t  byte[9], tmp, *buf = (byte_t*)data;

    /* 变反 - 高四位和低四位交换 */
    for (jj = 0, ii = 0; ii < size; ii++) {
        byte1 = ~buf[ii];
        byte2 =  byte1;
        byte1 &= 0xF0;
        byte1 =  byte1 >> 4;
        byte2 &= 0x0F;
        byte2 =  byte2 << 4;
        buf[ii] = (byte1 | byte2) ^ s_userkeyA[jj++];
        if (jj >= 16)
            jj = 0;
    }

    /* 对相邻的两个字节进行重新组合 */
    /* 组合的方法为 11112222 33334444 -> 44441111 22223333 */
    /*                                   11112222 33334444 */
    for (jj = 0, ii = 0; ii < size; ii += 2) {
        if (ii + 1 >= size)
            break;
        byte1 = buf[ii + 0] & 0xF0;
        byte2 = buf[ii + 0] & 0x0F;
        byte3 = buf[ii + 1] & 0xF0;
        byte4 = buf[ii + 1] & 0x0F;
        buf[ii + 0] = byte4 | byte1;
        buf[ii + 1] = byte2 | byte3;
        buf[ii + 0] ^= s_userkeyA[jj++];
        buf[ii + 1] ^= s_userkeyA[jj++];
        if (jj >= 16)
            jj = 0;
    }

    /* 对单个字节进行位变换 */
    /* 变换的方法为 12345678 -> 53671824 */
    /*                          12345678 */
    for (jj = 0, ii = 0; ii < size; ii++) {
        tmp = 0x80;
        for (kk = 1; kk <= 8; kk++) {
            byte[kk] = (buf[ii] & tmp);
            tmp = tmp >> 1;
        }
        byte[1] >>= 4;
        byte[2] >>= 5;
        byte[3] <<= 1;
        byte[4] >>= 4;
        byte[5] <<= 4;
        byte[6] <<= 3;
        byte[7] <<= 3;
        byte[8] <<= 2;

        buf[ii] = byte[1];
        for (kk = 2; kk <= 8; kk++)
            buf[ii] |= byte[kk];
        buf[ii] ^= key[jj];
        buf[ii] ^= s_userkeyB[jj++];
        if (jj >= 16)
            jj = 0;
    }

    /* 进行关联变换 */
    tmp = 0xD5;
    for (ii = 0; ii < size; ii++) {
        buf[ii] ^= tmp;
        tmp = buf[ii];
    }
}

/*
=======================================
    动态库用解密算法
=======================================
*/
CR_API void_t
dtmo_api_decode (
  __CR_IO__ void_t*         data,
  __CR_IN__ leng_t          size,
  __CR_IN__ const byte_t    key[16]
    )
{
    leng_t  ii, jj, kk;
    byte_t  byte1, byte2, byte3, byte4;
    byte_t  byte[9], tmp, *buf = (byte_t*)data;

    /* 进行关联变换 - 反变换 */
    tmp = 0xD5;
    for (ii = size - 1; ii >= 1; ii--)
        buf[ii] ^= buf[ii - 1];
    buf[0] ^= tmp;

    /* 对单个字节进行位变换 */
    /* 变换的方法为 12345678 -> 53671824 */
    /*                          12345678 */
    for (jj = 0, ii = 0; ii < size; ii++) {
        buf[ii] ^= s_userkeyB[jj];
        buf[ii] ^= key[jj++];
        if (jj >= 16)
            jj = 0;

        tmp = 0x80;
        for (kk = 1; kk <= 8; kk++) {
            byte[kk] = (buf[ii] & tmp);
            tmp = tmp >> 1;
        }
        byte[1] >>= 4;
        byte[2] >>= 1;
        byte[3] >>= 3;
        byte[4] >>= 3;
        byte[5] <<= 4;
        byte[6] >>= 2;
        byte[7] <<= 5;
        byte[8] <<= 4;

        buf[ii] = byte[1];
        for (kk = 2; kk <= 8; kk++)
            buf[ii] |= byte[kk];
    }

    /* 对相邻的两个字节进行重新组合 */
    /* 组合的方法为 11112222 33334444 -> 44441111 22223333 */
    /*                                   11112222 33334444 */
    for (jj = 0, ii = 0; ii < size; ii += 2) {
        if (ii + 1 >= size)
            break;
        buf[ii + 0] ^= s_userkeyA[jj++];
        buf[ii + 1] ^= s_userkeyA[jj++];
        if (jj >= 16)
            jj = 0;
        byte1 = buf[ii + 0] & 0xF0;
        byte2 = buf[ii + 0] & 0x0F;
        byte3 = buf[ii + 1] & 0xF0;
        byte4 = buf[ii + 1] & 0x0F;
        buf[ii + 0] = byte4 | byte1;
        buf[ii + 1] = byte2 | byte3;
    }

    /* 变反 - 高四位和低四位交换 */
    for (jj = 0, ii = 0; ii < size; ii++) {
        buf[ii] ^= s_userkeyA[jj++];
        if (jj >= 16)
            jj = 0;
        byte1 = buf[ii];
        byte2 = byte1;
        byte1 &= 0xF0;
        byte1 = byte1 >> 4;
        byte2 &= 0x0F;
        byte2 = byte2 << 4;
        buf[ii] = (byte1 | byte2);
        buf[ii] = ~buf[ii];
    }
}

/*
=======================================
    查找动态库用加密算法密钥
=======================================
*/
CR_API bool_t
dtmo_api_search (
  __CR_OT__ byte_t  key[16],
  __CR_IO__ void_t* version,
  __CR_IN__ leng_t  size_w_sum
    )
{
    uint_t  idx;
    void_t* temp;

    if (size_w_sum <= 12)
        return (FALSE);

    /* 无需解密版本字符串 */
    if (mem_cmp(version, "Wellcom_DTM-", 12) == 0) {
        mem_zero(key, 16);
        return (TRUE);
    }
    size_w_sum -= 1;
    temp = (void_t*)mem_malloc(size_w_sum);
    if (temp == NULL)
        return (FALSE);

    /* 用几个常用密钥尝试解密 */
    for (idx = 0; idx < cntsof(s_keylist); idx++) {
        mem_cpy(temp, version, size_w_sum);
        dtmo_api_decode(temp, size_w_sum, s_keylist[idx]);
        if (mem_cmp(temp, "Wellcom_DTM-", 12) == 0) {
            mem_cpy(version, temp, size_w_sum);
            ((byte_t*)version)[size_w_sum] = 0;
            break;
        }
    }
    mem_free(temp);
    if (idx >= cntsof(s_keylist))
        return (FALSE);
    mem_cpy(key, s_keylist[idx], 16);
    return (TRUE);
}

/*
=======================================
    手持机用加密算法
=======================================
*/
CR_API void_t
dtmo_luj_encode (
  __CR_IO__ void_t*         data,
  __CR_IN__ leng_t          size,
  __CR_IN__ const byte_t    key[16],
  __CR_IN__ const byte_t*   userkey
    )
{
    leng_t  ii, jj, kk;
    byte_t  byte1, byte2, byte3, byte4;
    byte_t  byte[9], tmp, *buf = (byte_t*)data;

    /* 默认的常数密钥 */
    if (userkey == NULL)
        userkey = (byte_t*)s_userkey1;

    /* 变反 - 高四位和低四位交换 */
    for (jj = 0, ii = 0; ii < size; ii++) {
        byte1 = ~buf[ii];
        byte2 =  byte1;
        byte1 &= 0xF0;
        byte1 =  byte1 >> 4;
        byte2 &= 0x0F;
        byte2 =  byte2 << 4;
        buf[ii] = (byte1 | byte2) ^ userkey[jj++];
        if (jj >= 16)
            jj = 0;
    }

    /* 对相邻的两个字节进行重新组合 */
    /* 组合的方法为 11112222 33334444 -> 44441111 22223333 */
    /*                                   11112222 33334444 */
    for (jj = 0, ii = 0; ii < size; ii += 2) {
        if (ii + 1 >= size)
            break;
        byte1 = buf[ii + 0] & 0xF0;
        byte2 = buf[ii + 0] & 0x0F;
        byte3 = buf[ii + 1] & 0xF0;
        byte4 = buf[ii + 1] & 0x0F;
        buf[ii + 0] = byte4 | byte1;
        buf[ii + 1] = byte2 | byte3;
        buf[ii + 0] ^= userkey[jj++];
        buf[ii + 1] ^= userkey[jj++];
        if (jj >= 16)
            jj = 0;
    }

    /* 对单个字节进行位变换 */
    /* 变换的方法为 12345678 -> 53671824 */
    /*                          12345678 */
    for (jj = 0, ii = 0; ii < size; ii++) {
        tmp = 0x80;
        for (kk = 1; kk <= 8; kk++) {
            byte[kk] = (buf[ii] & tmp);
            tmp = tmp >> 1;
        }
        byte[1] >>= 4;
        byte[2] >>= 5;
        byte[3] <<= 1;
        byte[4] >>= 4;
        byte[5] <<= 4;
        byte[6] <<= 3;
        byte[7] <<= 3;
        byte[8] <<= 2;

        buf[ii] = byte[1];
        for (kk = 2; kk <= 8; kk++)
            buf[ii] |= byte[kk];
        buf[ii] ^= key[jj++];
        if (jj >= 16)
            jj = 0;
    }

    /* 进行关联变换 */
    tmp = 0xD5;
    for (ii = 0; ii < size; ii++) {
        buf[ii] ^= tmp;
        tmp = buf[ii];
    }
}

/*
=======================================
    手持机用解密算法
=======================================
*/
CR_API void_t
dtmo_luj_decode (
  __CR_IO__ void_t*         data,
  __CR_IN__ leng_t          size,
  __CR_IN__ const byte_t    key[16],
  __CR_IN__ const byte_t*   userkey
    )
{
    leng_t  ii, jj, kk;
    byte_t  byte1, byte2, byte3, byte4;
    byte_t  byte[9], tmp, *buf = (byte_t*)data;

    /* 默认的常数密钥 */
    if (userkey == NULL)
        userkey = (byte_t*)s_userkey1;

    /* 进行关联变换 - 反变换 */
    tmp = 0xD5;
    for (ii = size - 1; ii >= 1; ii--)
        buf[ii] ^= buf[ii - 1];
    buf[0] ^= tmp;

    /* 对单个字节进行位变换 */
    /* 变换的方法为 12345678 -> 53671824 */
    /*                          12345678 */
    for (jj = 0, ii = 0; ii < size; ii++) {
        buf[ii] ^= key[jj++];
        if (jj >= 16)
            jj = 0;

        tmp = 0x80;
        for (kk = 1; kk <= 8; kk++) {
            byte[kk] = (buf[ii] & tmp);
            tmp = tmp >> 1;
        }
        byte[1] >>= 4;
        byte[2] >>= 1;
        byte[3] >>= 3;
        byte[4] >>= 3;
        byte[5] <<= 4;
        byte[6] >>= 2;
        byte[7] <<= 5;
        byte[8] <<= 4;

        buf[ii] = byte[1];
        for (kk = 2; kk <= 8; kk++)
            buf[ii] |= byte[kk];
    }

    /* 对相邻的两个字节进行重新组合 */
    /* 组合的方法为 11112222 33334444 -> 44441111 22223333 */
    /*                                   11112222 33334444 */
    for (jj = 0, ii = 0; ii < size; ii += 2) {
        if (ii + 1 >= size)
            break;
        buf[ii + 0] ^= userkey[jj++];
        buf[ii + 1] ^= userkey[jj++];
        if (jj >= 16)
            jj = 0;
        byte1 = buf[ii + 0] & 0xF0;
        byte2 = buf[ii + 0] & 0x0F;
        byte3 = buf[ii + 1] & 0xF0;
        byte4 = buf[ii + 1] & 0x0F;
        buf[ii + 0] = byte4 | byte1;
        buf[ii + 1] = byte2 | byte3;
    }

    /* 变反 - 高四位和低四位交换 */
    for (jj = 0, ii = 0; ii < size; ii++) {
        buf[ii] ^= userkey[jj++];
        if (jj >= 16)
            jj = 0;
        byte1 = buf[ii];
        byte2 = byte1;
        byte1 &= 0xF0;
        byte1 = byte1 >> 4;
        byte2 &= 0x0F;
        byte2 = byte2 << 4;
        buf[ii] = (byte1 | byte2);
        buf[ii] = ~buf[ii];
    }
}

/*
---------------------------------------
    密钥简单变换算法
---------------------------------------
*/
static void_t
dtmo_jjm_invchge (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    byte_t  tmp, *jj;
    leng_t  mm = (size - 1) / 2;
    byte_t* ii = (byte_t*)data;
    byte_t* pp = (byte_t*)data + mm;

    if (size == 1) {
        jj = (byte_t*)data + 7;
        tmp = *ii;
        *ii = *jj;
        *jj = tmp;
    }
    else {
        jj = (byte_t*)data + size - 1;
        for (; ii <= pp; ii++, jj--) {
            tmp = *ii;
            *ii = *jj;
            *jj = tmp;
        }
    }
}

/*
=======================================
    电源盒用变换算法
=======================================
*/
CR_API void_t
dtmo_jjm_crypto (
  __CR_OT__ void_t*         dst,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          size,
  __CR_IN__ const void_t*   key,
  __CR_IN__ leng_t          keylen
    )
{
    leng_t  ii, jj;
    byte_t  srcnum;
    byte_t  mi_num[8];
    byte_t  mm_key = 0;

    /* 解析密钥 */
    size   &= 0xFF;
    keylen &= 0xFF;
    for (ii = 0; ii < keylen; ii++)
        mm_key ^= ((byte_t*)key)[ii];
    mm_key = (mm_key % 8) + 1;

    for (ii = 0; ii < size; ii++)
    {
        /* 分解需解密的数据 */
        jj = 7;
        mem_zero(mi_num, sizeof(mi_num));
        srcnum = ((byte_t*)src)[ii];
        while (srcnum != 0) {
            mi_num[jj--] = srcnum & 1;
            srcnum = srcnum >> 1;
        }

        if (mm_key == 1)
            dtmo_jjm_invchge(&mi_num[0], mm_key);
        else
            dtmo_jjm_invchge(&mi_num[8 - mm_key], mm_key);

        jj = 0;
        srcnum = 0;
        while (jj < 8) {
            if (mi_num[jj] != 0)
                srcnum = (byte_t)((mi_num[jj] << (8 - jj - 1)) + srcnum);
            jj++;
        }
        ((byte_t*)dst)[ii] = srcnum;
    }
}

/*
=======================================
    四川串口通讯加密
=======================================
*/
CR_API void_t
dtmo_sc232_encode (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    if (*(byte_t*)data != 0xB0) {
        dtmo_jjm_crypto(data, data, size, s_sc_ickey, 16);
        dtmo_luj_encode(data, size, s_sc_ickey, NULL);
    }
}

/*
=======================================
    四川串口通讯解密
=======================================
*/
CR_API void_t
dtmo_sc232_decode (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    if (*(byte_t*)data != 0xB0) {
        dtmo_luj_decode(data, size, s_sc_ickey, NULL);
        dtmo_jjm_crypto(data, data, size, s_sc_ickey, 16);
    }
}

/*
=======================================
    WLFID 字符串加密
=======================================
*/
CR_API bool_t
wlfid_str_encode (
  __CR_OT__ ansi_t*         dst,
  __CR_IN__ const ansi_t*   src,
  __CR_IN__ const ansi_t*   key
    )
{
    sDES    des;
    leng_t  leng;
    leng_t  blks;
    byte_t* copy;
    byte_t* pntr;
    byte_t  tp[8];

    leng = str_lenA(src);
    copy = (byte_t*)mem_malloc(leng + 16);
    if (copy == NULL)
        return (FALSE);
    blks = str_lenA(key);
    if (blks < 8)
        mem_zero(tp, 8);
    else
        blks = 8;
    mem_cpy(tp, key, blks);
    crypto_des_key(&des, tp, FALSE);
    pntr = copy;
    blks = leng / 8 + 1;
    mem_zero(copy, leng + 16);
    str_cpyA((ansi_t*)copy, src);
    for (; blks != 0; blks--) {
        crypto_des_ops(&des, pntr, pntr);
        pntr += 8;
    }
    leng = leng / 8 + 1;
    hex2strA(dst, copy, leng * 8);
    mem_free(copy);
    return (TRUE);
}

/*
=======================================
    WLFID 字符串解密
=======================================
*/
CR_API bool_t
wlfid_str_decode (
  __CR_OT__ ansi_t*         dst,
  __CR_IN__ const ansi_t*   src,
  __CR_IN__ const ansi_t*   key
    )
{
    sDES    des;
    leng_t  leng;
    leng_t  blks;
    byte_t* copy;
    byte_t* pntr;
    byte_t  tp[8];

    leng = str_lenA(src);
    if (leng == 0 || leng % 16 != 0)
        return (FALSE);
    leng /= 2;
    copy = (byte_t*)mem_malloc(leng);
    if (copy == NULL)
        return (FALSE);
    blks = leng;
    str2datA(copy, &blks, src);
    if (leng != blks) {
        mem_free(copy);
        return (FALSE);
    }
    blks = str_lenA(key);
    if (blks < 8)
        mem_zero(tp, 8);
    else
        blks = 8;
    mem_cpy(tp, key, blks);
    crypto_des_key(&des, tp, TRUE);
    pntr = copy;
    blks = leng / 8;
    for (; blks != 0; blks--) {
        crypto_des_ops(&des, pntr, pntr);
        pntr += 8;
    }
    mem_cpy(dst, copy, leng);
    dst[leng] = CR_AC(NIL);
    mem_free(copy);
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
