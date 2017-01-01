/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-02-06  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 电驴哈希函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

/*
=======================================
    ED2K 初始化
=======================================
*/
CR_API void_t
hash_ed2k_init (
  __CR_OT__ sED2K*  ctx
    )
{
    ctx->counts = 0UL;
    ctx->blocks = 9728000UL;
    hash_md4_init(&ctx->md4dat);
    hash_md4_init(&ctx->md4lst);
    hash_md4_init(&ctx->md4ctx);
}

/*
=======================================
    ED2K 计算数据块
=======================================
*/
CR_API void_t
hash_ed2k_update (
  __CR_IO__ sED2K*          ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    leng_t  blk;
    int32u  rst;
    byte_t  md4[16];

    /* 这里用来计算小于一块大小的哈希 */
    if (ctx->counts == 0 && size <= ctx->blocks)
        hash_md4_update(&ctx->md4ctx, data, (int32u)size);

    /* 前面已经哈希过一部分数据 */
    if (ctx->blocks < 9728000UL)
    {
        /* 计算满块的数据哈希 */
        if (size < ctx->blocks)
        {
            /* 本次的大小仍然不足满块 (这里不可能截断) */
            hash_md4_update(&ctx->md4dat, data, (int32u)size);
            ctx->blocks -= (int32u)size;
            return;
        }

        /* 本次大小刚好或超出满块 */
        hash_md4_update(&ctx->md4dat, data, ctx->blocks);
        hash_md4_finish(md4, &ctx->md4dat);
        blk = (leng_t)ctx->blocks;  /* 16位系统下必定不会截断 */
        size -= blk;

        /* 计算新出的 MD4 的 MD4 */
        hash_md4_update(&ctx->md4lst, md4, 16);

        /* 重新复位数据块的 MD4 计算 */
        ctx->counts++;
        ctx->blocks = 9728000UL;
        hash_md4_init(&ctx->md4dat);
        if (size == 0)
            return;
        data = (uchar*)data + blk;
    }

    /* 这里的计算已经保证了初始状态 */
#ifndef _CR_SYS16_
    blk = (leng_t)(size / 9728000UL);
    rst = (int32u)(size % 9728000UL);

    /* 16位系统下 blk 必定为0 */
    for (; blk != 0; blk--, ctx->counts++) {
        hash_md4_total(md4, data, 9728000UL);
        hash_md4_update(&ctx->md4lst, md4, 16);
        data = (uchar*)data + 9728000UL;
    }
#else
    rst = (int32u)size; /* 剩余的大小直接拿来计算 MD4 */
#endif
    /* 余下的数据可能会有后续数据 */
    if (rst != 0) {
        hash_md4_update(&ctx->md4dat, data, rst);
        ctx->blocks -= rst;
    }
}

/*
=======================================
    ED2K 获取结果 (添加空块)
=======================================
*/
CR_API void_t
hash_ed2k_finish1 (
  __CR_OT__ byte_t  hash[16],
  __CR_IO__ sED2K*  ctx
    )
{
    byte_t  md4[16];

    /* 不满一块, 返回 MD4 */
    if (ctx->counts == 0) {
        hash_md4_finish(hash, &ctx->md4ctx);
        return;
    }

    /* 总大小为块大小的整数倍, 添加一个空块 */
    if (ctx->blocks == 9728000UL)
        hash_md4_total(md4, NULL, 0);
    else
        hash_md4_finish(md4, &ctx->md4dat);
    hash_md4_update(&ctx->md4lst, md4, 16);
    hash_md4_finish(hash, &ctx->md4lst);
}

/*
=======================================
    ED2K 获取结果 (不添加空块)
=======================================
*/
CR_API void_t
hash_ed2k_finish2 (
  __CR_OT__ byte_t  hash[16],
  __CR_IO__ sED2K*  ctx
    )
{
    byte_t  md4[16];

    /* 不满或刚满一块, 返回 MD4 */
    if ((ctx->counts == 0) ||
       ((ctx->counts == 1) &&
        (ctx->blocks == 9728000UL))) {
        hash_md4_finish(hash, &ctx->md4ctx);
        return;
    }

    /* 处理未完的数据 */
    if (ctx->blocks < 9728000UL) {
        hash_md4_finish(md4, &ctx->md4dat);
        hash_md4_update(&ctx->md4lst, md4, 16);
    }
    hash_md4_finish(hash, &ctx->md4lst);
}

/*
=======================================
    ED2K 一次性完成 (添加空块)
=======================================
*/
CR_API void_t
hash_ed2k_total1 (
  __CR_OT__ byte_t          hash[16],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    sED2K   ctx;

    hash_ed2k_init(&ctx);
    hash_ed2k_update(&ctx, data, size);
    hash_ed2k_finish1(hash, &ctx);
}

/*
=======================================
    ED2K 一次性完成 (不添加空块)
=======================================
*/
CR_API void_t
hash_ed2k_total2 (
  __CR_OT__ byte_t          hash[16],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    sED2K   ctx;

    hash_ed2k_init(&ctx);
    hash_ed2k_update(&ctx, data, size);
    hash_ed2k_finish2(hash, &ctx);
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
