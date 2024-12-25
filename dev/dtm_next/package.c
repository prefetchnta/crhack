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
/*  >>>>>>>>>>>>>>>>>>>>>>> DTM_NEXT 程序解包函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "crypto.h"
#include "encdec.h"
#include "memlib.h"
#include "morder.h"
#include "strlib.h"
#include "package.h"

/*
=======================================
    释放解包后的程序
=======================================
*/
CR_API void_t
dtm_pack_free (
  __CR_IN__ sDTM_PRO*   outp
    )
{
    mem_free(outp->all_data);
    mem_free(outp->version);
}

/*
=======================================
    检查程序版本字符串
=======================================
*/
CR_API uint_t
dtm_check_ver (
  __CR_IN__ const ansi_t*   file,
  __CR_IO__ ansi_t**        vers
    )
{
    ansi_t  *version;
    uint_t  leng, type;

    if (file == NULL)
        return (PRO_TYPE_INV);
    leng = str_lenA(file);
    if (vers == NULL) {
        if (leng < 17 || leng > 21)
            return (PRO_TYPE_INV);
    }
    else {
        if (leng < 27 || leng > 37)
            return (PRO_TYPE_INV);
    }
    if (!is_digitA(file[14]))
        return (PRO_TYPE_INV);
    if (file[ 4] != CR_AC('_') || file[ 8] != CR_AC('_') ||
        file[12] != CR_AC('_') || file[13] != CR_AC('V'))
        return (PRO_TYPE_INV);

    /* 判断类型 */
    type = PRO_TYPE_CAR;
    if (vers != NULL) {
        if ((version = (ansi_t*)str_strA(file, "_CAR_")) != NULL)
            type = PRO_TYPE_CAR;
        else
        if ((version = (ansi_t*)str_strA(file, "_SMU_")) != NULL)
            type = PRO_TYPE_SMU;
        else
        if ((version = (ansi_t*)str_strA(file, "_THE_")) != NULL)
            type = PRO_TYPE_THE;
        else
            return (PRO_TYPE_INV);
        *vers = version + 5;
    }
    return (type);
}

/*
=======================================
    DTM_OLD 程序解包
=======================================
*/
CR_API bool_t
dtm_old_unpack (
  __CR_OT__ sDTM_PRO*       outp,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ const ansi_t*   unkey
    )
{
    sDES    des;
    leng_t  blks;
    int32u  temp;
    byte_t  sum8;
    byte_t* pntr;
    byte_t* copy;

    /* 参数过滤 */
    CR_NOUSE(unkey);
    if (size % 8 != 0 || size <= 20 * 8 || size > CR_M2B(1))
        return (FALSE);
    copy = (byte_t*)mem_dup(data, size);
    if (copy == NULL)
        return (FALSE);
    pntr = copy;

    /* 尝试用三种密钥解密 */
    crypto_des_key(&des, (byte_t*)"we||comj", TRUE);
    for (blks = 20; blks != 0; blks--) {
        crypto_des_ops(&des, pntr, pntr);
        pntr += 8;
    }
    if (str_strA((ansi_t*)copy, "_66") != NULL ||
        str_strA((ansi_t*)copy, "_80") != NULL)
        goto _dtm_old_okay;
    pntr = (byte_t*)mem_cpy(copy, data, 20 * 8);
    crypto_des_key(&des, (byte_t*)"WELLCOMY", TRUE);
    for (blks = 20; blks != 0; blks--) {
        crypto_des_ops(&des, pntr, pntr);
        pntr += 8;
    }
    if (str_strA((ansi_t*)copy, "_66") != NULL ||
        str_strA((ansi_t*)copy, "_80") != NULL)
        goto _dtm_old_okay;
    pntr = (byte_t*)mem_cpy(copy, data, 20 * 8);
    crypto_des_key(&des, (byte_t*)"NOFIGWEL", TRUE);
    for (blks = 20; blks != 0; blks--) {
        crypto_des_ops(&des, pntr, pntr);
        pntr += 8;
    }
    if (str_strA((ansi_t*)copy, "_66") == NULL &&
        str_strA((ansi_t*)copy, "_80") == NULL)
        goto _failure1;

_dtm_old_okay:  /* 继续解密 */
    blks = size / 8 - 20;
    for (; blks != 0; blks--) {
        crypto_des_ops(&des, pntr, pntr);
        pntr += 8;
    }

    /* 抽取包文件的参数信息 */
    struct_zero(outp, sDTM_PRO);
    outp->car_ver = 0xFFFFFFFFUL;
    outp->smu_ver = 0xFFFFFFFFUL;
    outp->the_ver = 0xFFFFFFFFUL;
    blks = str_lenA((ansi_t*)copy);
    if (blks > 36)
        goto _failure1;
    outp->version = str_dupA((ansi_t*)copy);
    if (outp->version == NULL)
        goto _failure1;
    pntr = copy + blks;
    temp = load_misali32(pntr + 0);
    outp->pro_size = DWORD_BE(temp);
    if (outp->pro_size < 32 ||
        outp->pro_size > CR_K2B(512) ||
        outp->pro_size > size - blks - 1 - 8)
        goto _failure2;
    temp = load_misali32(pntr + 4);
    outp->hzk_size = DWORD_BE(temp);
    if (outp->hzk_size > CR_K2B(512) ||
        outp->hzk_size > size - blks - 1 - 8)
        goto _failure2;
    size -= outp->hzk_size;
    if (blks + outp->pro_size + 1 + 8 > size)
        goto _failure2;

    /* 计算整个包文件的校验和 */
    temp = blks + 8 + outp->pro_size + outp->hzk_size;
    for (pntr = copy, sum8 = 0; temp != 0; temp--)
        sum8 = (byte_t)(sum8 + *pntr++);
    if (sum8 != pntr[0])
        goto _failure2;
    size = (leng_t)(outp->pro_size + outp->hzk_size);
    outp->all_data = (byte_t*)mem_dup(copy + blks + 8, size);
    if (outp->all_data == NULL)
        goto _failure2;
    mem_free(copy);
    return (TRUE);

_failure2:
    mem_free(outp->version);
_failure1:
    mem_free(copy);
    return (FALSE);
}

/* DTM_NEW 封包文件头 (LE) */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif
CR_TYPEDEF struct
{
/*000*/ byte_t  magic[8];       /* "\x89PACKAGE" */
/*008*/ ansi_t  bin_ver[22];    /* "ZJNB_VHF_V42_Vnnn.nnn" (22) */
/*030*/ int32u  car_ver;        /* 车载机版本 "nnn.nnn.nnn" (12) */
/*034*/ int32u  smu_ver;        /* 模拟机版本 "nnn.nnn.nnn" (12) */
/*038*/ int32u  the_ver;        /* 理论机版本 "nnn.nnn.nnn" (12) */
/*042*/ int32u  dat_size[2];    /* 保存的两块数据大小 (程序+字库) */
/*050*/ byte_t  padded56[2];    /* 保证使结构大小为56字节 */
/*052*/ int32u  checksum32;     /* 数据的 CRC-32/IEEE 校验 */
/*056*/
} CR_PACKED sDTM_NEW_HDR;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    DTM_NEW 程序解包
=======================================
*/
CR_API bool_t
dtm_new_unpack (
  __CR_OT__ sDTM_PRO*       outp,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ const ansi_t*   unkey
    )
{
    sDES            des;
    uint_t          type;
    leng_t          blks;
    ansi_t*         vers;
    byte_t*         pntr;
    byte_t*         copy;
    int32u          key[2];
    sDTM_NEW_HDR*   header;

    /* 参数过滤 */
    if (size % 8 != 0 || size > CR_M2B(1) || size <= sizeof(sDTM_NEW_HDR))
        return (FALSE);
    if (mem_cmp(data, "\x89PACKAGE", 8) != 0)
        return (FALSE);
    vers = NULL;
    type = dtm_check_ver(unkey, &vers);
    if (type == PRO_TYPE_INV || vers == NULL)
        return (FALSE);
    copy = (byte_t*)mem_dup(data, size);
    if (copy == NULL)
        return (FALSE);

    /* 封包数据解密 */
    pntr = copy + 8;
    blks = size / 8 - 1;
    key[1] = hash_crc32i_total(unkey,  7);
    key[0] = hash_crc32i_total(unkey, 14);
    crypto_des_key(&des, (byte_t*)key, TRUE);
    for (; blks != 0; blks--) {
        crypto_des_ops(&des, pntr, pntr);
        pntr += 8;
    }
    dtmn_dat_crypto(copy + 8, size - 8);

    /* 校验包文件数据的有效性 */
    header = (sDTM_NEW_HDR*)copy;
    key[1] = header->checksum32;
    if (mem_cmp(header->bin_ver, unkey, 14) != 0)
        goto _failure1;
    pntr = (byte_t*)(&header[1]);
    key[0] = hash_crc32i_total(pntr, size - sizeof(sDTM_NEW_HDR));
    if (key[0] != key[1])
        goto _failure1;

    /* 抽取包文件的参数信息 */
    struct_zero(outp, sDTM_PRO);
    outp->car_ver = DWORD_LE(header->car_ver);
    outp->smu_ver = DWORD_LE(header->smu_ver);
    outp->the_ver = DWORD_LE(header->the_ver);
    blks = str_lenA((ansi_t*)copy + 8);
    if (blks >= 22)
        goto _failure1;
    outp->version = str_dupA((ansi_t*)copy + 8);
    if (outp->version == NULL)
        goto _failure1;
    outp->pro_size = DWORD_LE(header->dat_size[0]);
    if (outp->pro_size < 32 ||
        outp->pro_size > CR_K2B(512) ||
        outp->pro_size > size - sizeof(sDTM_NEW_HDR))
        goto _failure2;
    outp->hzk_size = DWORD_LE(header->dat_size[1]);
    if (outp->hzk_size > CR_K2B(512) ||
        outp->hzk_size > size - sizeof(sDTM_NEW_HDR))
        goto _failure2;
    size -= outp->hzk_size;
    if (outp->pro_size + sizeof(sDTM_NEW_HDR) > size)
        goto _failure2;
    size = (leng_t)(outp->pro_size + outp->hzk_size);
    outp->all_data = (byte_t*)mem_dup(pntr, size);
    if (outp->all_data == NULL)
        goto _failure2;
    mem_free(copy);
    return (TRUE);

_failure2:
    mem_free(outp->version);
_failure1:
    mem_free(copy);
    return (FALSE);
}

/*
=======================================
    自动识别程序解包
=======================================
*/
CR_API bool_t
dtm_all_unpack (
  __CR_OT__ sDTM_PRO*       outp,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ const ansi_t*   unkey
    )
{
    /* 新封包格式优先检查 */
    if (dtm_new_unpack(outp, data, size, unkey))
        return (TRUE);
    if (dtm_old_unpack(outp, data, size, unkey))
        return (TRUE);
    return (FALSE);
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
