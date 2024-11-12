/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-28  */
/*     #######          ###    ###      [GFX2]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 软件 2D 内部函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "bltint.h"
#include "memlib.h"
#include "pixels.h"

/*****************************************************************************/
/*                             标准颜色转换函数                              */
/*****************************************************************************/

/*
---------------------------------------
    标准32位色转IDX1
---------------------------------------
*/
extern cpix_t
pixel_cnvt01 (
  __CR_IN__ const int32u*   pal,
  __CR_IN__ cl32_t          color
    )
{
    cpix_t  cnvt;
    byte_t* lerp;

    cnvt.val = 0UL;
    lerp = (byte_t*)(&color);
    if (mem_cmp(lerp, &pal[1], 3) == 0) {
        cnvt.c08.idx = 0x01;
        cnvt.c08.lrp = lerp[3];
    }
    else
    if (mem_cmp(lerp, &pal[0], 3) == 0) {
        cnvt.c08.lrp = lerp[3];
    }
    return (cnvt);
}

/*
---------------------------------------
    标准32位色转IDX2
---------------------------------------
*/
extern cpix_t
pixel_cnvt02 (
  __CR_IN__ const int32u*   pal,
  __CR_IN__ cl32_t          color
    )
{
    uint_t  idx;
    cpix_t  cnvt;
    byte_t* lerp;

    cnvt.val = 0UL;
    lerp = (byte_t*)(&color);
    for (idx = 0; idx < 4; idx++) {
        if (mem_cmp(lerp, &pal[idx], 3) == 0) {
            cnvt.c08.idx = (byte_t)idx;
            cnvt.c08.lrp = lerp[3];
            break;
        }
    }
    return (cnvt);
}

/*
---------------------------------------
    标准32位色转IDX4
---------------------------------------
*/
extern cpix_t
pixel_cnvt04 (
  __CR_IN__ const int32u*   pal,
  __CR_IN__ cl32_t          color
    )
{
    uint_t  idx;
    cpix_t  cnvt;
    byte_t* lerp;

    cnvt.val = 0UL;
    lerp = (byte_t*)(&color);
    for (idx = 0; idx < 16; idx++) {
        if (mem_cmp(lerp, &pal[idx], 3) == 0) {
            cnvt.c08.idx = (byte_t)idx;
            cnvt.c08.lrp = lerp[3];
            break;
        }
    }
    return (cnvt);
}

/*
---------------------------------------
    标准32位色转IDX8
---------------------------------------
*/
extern cpix_t
pixel_cnvt08 (
  __CR_IN__ const int32u*   pal,
  __CR_IN__ cl32_t          color
    )
{
    uint_t  idx;
    cpix_t  cnvt;
    byte_t* lerp;

    cnvt.val = 0UL;
    lerp = (byte_t*)(&color);
    for (idx = 0; idx < 256; idx++) {
        if (mem_cmp(lerp, &pal[idx], 3) == 0) {
            cnvt.c08.idx = (byte_t)idx;
            cnvt.c08.lrp = lerp[3];
            break;
        }
    }
    return (cnvt);
}

/*
---------------------------------------
    标准32位色转4444
---------------------------------------
*/
extern cpix_t
pixel_cnvt12 (
  __CR_IN__ const int32u*   pal,
  __CR_IN__ cl32_t          color
    )
{
    int16u  temp;
    cpix_t  cnvt;
    byte_t* lerp;

    CR_NOUSE(pal);
    cnvt.val = 0UL;
    lerp = (byte_t*)(&color);
    cnvt.c16.clr = pixel32to4444(&temp, color);
    cnvt.c16.lrp = lerp[3] >> 4;
    return (cnvt);
}

/*
---------------------------------------
    标准32位色转X555
---------------------------------------
*/
extern cpix_t
pixel_cnvt15 (
  __CR_IN__ const int32u*   pal,
  __CR_IN__ cl32_t          color
    )
{
    int16u  temp;
    cpix_t  cnvt;
    byte_t* lerp;

    CR_NOUSE(pal);
    cnvt.val = 0UL;
    lerp = (byte_t*)(&color);
    cnvt.c16.clr = pixel32toX555(&temp, color);
    cnvt.c16.lrp = lerp[3] >> 3;
    return (cnvt);
}

/*
---------------------------------------
    标准32位色转1555
---------------------------------------
*/
extern cpix_t
pixel_cnvt17 (
  __CR_IN__ const int32u*   pal,
  __CR_IN__ cl32_t          color
    )
{
    int16u  temp;
    cpix_t  cnvt;
    byte_t* lerp;

    CR_NOUSE(pal);
    cnvt.val = 0UL;
    lerp = (byte_t*)(&color);
    cnvt.c16.clr = pixel32to1555(&temp, color);
    cnvt.c16.lrp = lerp[3] >> 3;
    return (cnvt);
}

/*
---------------------------------------
    标准32位色转565
---------------------------------------
*/
extern cpix_t
pixel_cnvt16 (
  __CR_IN__ const int32u*   pal,
  __CR_IN__ cl32_t          color
    )
{
    int16u  temp;
    cpix_t  cnvt;
    byte_t* lerp;

    CR_NOUSE(pal);
    cnvt.val = 0UL;
    lerp = (byte_t*)(&color);
    cnvt.c16.clr = pixel32to565(&temp, color);
    cnvt.c16.lrp = lerp[3] >> 2;
    return (cnvt);
}

/*
---------------------------------------
    标准32位色转888
---------------------------------------
*/
extern cpix_t
pixel_cnvt24 (
  __CR_IN__ const int32u*   pal,
  __CR_IN__ cl32_t          color
    )
{
    cpix_t  cnvt;

    CR_NOUSE(pal);
    cnvt.val = color;
    return (cnvt);
}

/*
---------------------------------------
    标准32位色转8888
---------------------------------------
*/
extern cpix_t
pixel_cnvt32 (
  __CR_IN__ const int32u*   pal,
  __CR_IN__ cl32_t          color
    )
{
    cpix_t  cnvt;

    CR_NOUSE(pal);
    cnvt.val = color;
    return (cnvt);
}

/*****************************************************************************/
/*                           像素转换绘制函数查找                            */
/*****************************************************************************/

/* 标准颜色转换查找表 */
static const pixcnvt_t _rom_ s_pixel_cnvt[] =
{
    NULL, NULL, NULL, NULL, NULL, NULL, pixel_cnvt01,
    pixel_cnvt04, pixel_cnvt08, pixel_cnvt02, NULL, NULL,
    pixel_cnvt12, NULL, NULL, pixel_cnvt15, pixel_cnvt16,
    pixel_cnvt17, NULL, NULL, NULL, NULL, NULL, NULL,
    pixel_cnvt24, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    pixel_cnvt32,
};

/*
---------------------------------------
    根据像素格式查找转换函数
---------------------------------------
*/
extern pixcnvt_t
pixel_find_cnvt (
  __CR_IN__ uint_t  fcrh
    )
{
    if (fcrh >= cntsof(s_pixel_cnvt))
        return (NULL);
    return (s_pixel_cnvt[fcrh]);
}

/* 像素绘制函数查找表 */
static const pixdraw_t _rom_ s_pixel_draw1[] =
{
    /* 正/反向绘制 */
    pixel_set01z, pixel_set02z, pixel_set04z,
    pixel_set08z, pixel_set16z, pixel_set24z, pixel_set32z,
    pixel_set01n, pixel_set02n, pixel_set04n,
    pixel_set08n, pixel_set16n, pixel_set24n, pixel_set32n,
    /*---------------------------------------------------*/
    /* 正/反向与绘制 */
    pixel_set01z, pixel_set02z, pixel_set04z,
    pixel_and08z, pixel_and16z, pixel_and24z, pixel_and32z,
    pixel_set01n, pixel_set02n, pixel_set04n,
    pixel_and08n, pixel_and16n, pixel_and24n, pixel_and32n,
    /*---------------------------------------------------*/
    /* 正/反向或绘制 */
    pixel_set01z, pixel_set02z, pixel_set04z,
    pixel_orr08z, pixel_orr16z, pixel_orr24z, pixel_orr32z,
    pixel_set01n, pixel_set02n, pixel_set04n,
    pixel_orr08n, pixel_orr16n, pixel_orr24n, pixel_orr32n,
    /*---------------------------------------------------*/
    /* 正/反向非绘制 */
    pixel_set01z, pixel_set02z, pixel_set04z,
    pixel_not08z, pixel_not16z, pixel_not24z, pixel_not32z,
    pixel_set01n, pixel_set02n, pixel_set04n,
    pixel_not08n, pixel_not16n, pixel_not24n, pixel_not32n,
    /*---------------------------------------------------*/
    /* 正/反向异或绘制 */
    pixel_set01z, pixel_set02z, pixel_set04z,
    pixel_xor08z, pixel_xor16z, pixel_xor24z, pixel_xor32z,
    pixel_set01n, pixel_set02n, pixel_set04n,
    pixel_xor08n, pixel_xor16n, pixel_xor24n, pixel_xor32n,
    /*---------------------------------------------------*/
};

static const pixdraw_t _rom_ s_pixel_draw2[] =
{
    /* 正/反向饱和加法 */
    NULL, NULL, NULL, NULL, NULL, NULL, pixel_set01z,
    pixel_set04z, pixel_add08z, pixel_set02z, NULL, NULL,
    pixel_add12z, NULL, NULL, pixel_add15z, pixel_add16z,
    pixel_add15z, NULL, NULL, NULL, NULL, NULL, NULL,
    pixel_add24z, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, pixel_add32z,
    /*---------------------------------------------*/
    NULL, NULL, NULL, NULL, NULL, NULL, pixel_set01n,
    pixel_set04n, pixel_add08n, pixel_set02n, NULL, NULL,
    pixel_add12n, NULL, NULL, pixel_add15n, pixel_add16n,
    pixel_add15n, NULL, NULL, NULL, NULL, NULL, NULL,
    pixel_add24n, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, pixel_add32n,
    /*---------------------------------------------*/
    /* 正/反向饱和减法 */
    NULL, NULL, NULL, NULL, NULL, NULL, pixel_set01z,
    pixel_set04z, pixel_sub08z, pixel_set02z, NULL, NULL,
    pixel_sub12z, NULL, NULL, pixel_sub15z, pixel_sub16z,
    pixel_sub15z, NULL, NULL, NULL, NULL, NULL, NULL,
    pixel_sub24z, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, pixel_sub32z,
    /*---------------------------------------------*/
    NULL, NULL, NULL, NULL, NULL, NULL, pixel_set01n,
    pixel_set04n, pixel_sub08n, pixel_set02n, NULL, NULL,
    pixel_sub12n, NULL, NULL, pixel_sub15n, pixel_sub16n,
    pixel_sub15n, NULL, NULL, NULL, NULL, NULL, NULL,
    pixel_sub24n, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, pixel_sub32n,
    /*---------------------------------------------*/
    /* 正/反向透明混合 */
    NULL, NULL, NULL, NULL, NULL, NULL, pixel_set01z,
    pixel_set04z, pixel_lrp08z, pixel_set02z, NULL, NULL,
    pixel_lrp12z, NULL, NULL, pixel_lrp15z, pixel_lrp16z,
    pixel_lrp15z, NULL, NULL, NULL, NULL, NULL, NULL,
    pixel_lrp24z, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, pixel_lrp32z,
    /*---------------------------------------------*/
    NULL, NULL, NULL, NULL, NULL, NULL, pixel_set01n,
    pixel_set04n, pixel_lrp08n, pixel_set02n, NULL, NULL,
    pixel_lrp12n, NULL, NULL, pixel_lrp15n, pixel_lrp16n,
    pixel_lrp15n, NULL, NULL, NULL, NULL, NULL, NULL,
    pixel_lrp24n, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, pixel_lrp32n,
    /*---------------------------------------------*/
};

/*
---------------------------------------
    根据像素格式和模式查找绘制函数
---------------------------------------
*/
extern pixdraw_t
pixel_find_draw (
  __CR_IN__ uint_t  fcrh,
  __CR_IN__ bool_t  flip,
  __CR_IN__ uint_t  mode
    )

{
    if (mode <= CR_BLT_XOR)
    {
        switch (fcrh)
        {
            default:
                return (NULL);

            case CR_INDEX1: fcrh = 0; break;
            case CR_INDEX2: fcrh = 1; break;
            case CR_INDEX4: fcrh = 2; break;
            case CR_INDEX8: fcrh = 3; break;

            case CR_ARGB4444:
            case CR_ARGBX555:
            case CR_ARGB565:
            case CR_ARGB1555: fcrh = 4; break;
            case CR_ARGB888:  fcrh = 5; break;
            case CR_ARGB8888: fcrh = 6; break;
        }
        if (flip)
            fcrh += 7;
        fcrh += mode * 14;
        return (s_pixel_draw1[fcrh]);
    }
    if (mode <= CR_BLT_MAX)
    {
        switch (fcrh)
        {
            default:
                return (NULL);

            case CR_INDEX1:
            case CR_INDEX2:
            case CR_INDEX4:
            case CR_INDEX8:
            case CR_ARGB4444:
            case CR_ARGBX555:
            case CR_ARGB565:
            case CR_ARGB1555:
            case CR_ARGB888:
            case CR_ARGB8888:
                break;
        }
        if (flip)
            fcrh += 33;
        fcrh += (mode - CR_BLT_ADD) * 66;
        return (s_pixel_draw2[fcrh]);
    }
    return (NULL);
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
