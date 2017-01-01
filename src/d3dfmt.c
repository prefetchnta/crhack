/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-09-03  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack D3D 像素处理函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#define _CR_USE_PIXELCVT_
#include "img2d.h"
#include "memlib.h"
#include "pixels.h"
#include "_fmtz_/d3dfmt.h"

/*
=======================================
    CrHack 转 D3DFMT 格式
=======================================
*/
CR_API int32u
image_crh_to_d3d (
  __CR_IN__ uint_t  crh_fmt
    )
{
    switch (crh_fmt)
    {
        default:
            break;

        case CR_DXT1:
            return (D3DFMT_DXT1);

        case CR_DXT2:
            return (D3DFMT_DXT2);

        case CR_DXT3:
            return (D3DFMT_DXT3);

        case CR_DXT4:
            return (D3DFMT_DXT4);

        case CR_DXT5:
            return (D3DFMT_DXT5);

        case CR_INDEX8:
            return (D3DFMT_P8);

        case CR_ARGB565:
            return (D3DFMT_R5G6B5);

        case CR_ARGB888:
            return (D3DFMT_R8G8B8);

        case CR_ARGB4444:
            return (D3DFMT_A4R4G4B4);

        case CR_ARGBX555:
            return (D3DFMT_X1R5G5B5);

        case CR_ARGB1555:
            return (D3DFMT_A1R5G5B5);

        case CR_ARGB8888:
            return (D3DFMT_A8R8G8B8);
    }
    return (D3DFMT_UNKNOWN);
}

/*
=======================================
    D3DFMT 转 CrHack 格式
=======================================
*/
CR_API uint_t
image_d3d_to_crh (
  __CR_IN__ int32u  d3d_fmt
    )
{
    switch (d3d_fmt)
    {
        default:
            break;

        case D3DFMT_DXT1:
            return (CR_DXT1);

        case D3DFMT_DXT2:
            return (CR_DXT2);

        case D3DFMT_DXT3:
            return (CR_DXT3);

        case D3DFMT_DXT4:
            return (CR_DXT4);

        case D3DFMT_DXT5:
            return (CR_DXT5);

        case D3DFMT_P8:
        case D3DFMT_L8:
            return (CR_INDEX8);

        case D3DFMT_R5G6B5:
            return (CR_ARGB565);

        case D3DFMT_R8G8B8:
            return (CR_ARGB888);

        case D3DFMT_A4R4G4B4:
            return (CR_ARGB4444);

        case D3DFMT_X1R5G5B5:
            return (CR_ARGBX555);

        case D3DFMT_A1R5G5B5:
            return (CR_ARGB1555);

        case D3DFMT_A8R8G8B8:
            return (CR_ARGB8888);
    }
    return (CR_UNKNOWN);
}

/*
=======================================
    从 D3DFMT 转出到8888
=======================================
*/
CR_API sIMAGE*
img_d3dx_to_32 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const void_t*   pixel,
  __CR_IN__ leng_t          size,
  __CR_IN__ uint_t          width,
  __CR_IN__ uint_t          height,
  __CR_IN__ const void_t*   palette,
  __CR_IN__ int32u          d3dfmt,
  __CR_IN__ uint_t          align
    )
{
    int32u  clr;
    leng_t  idx;
    uint_t  x, y;
    sIMAGE  temp;
    dist_t  dbpl;
    int32u* dtmp;
    byte_t* stmp;
    byte_t* dptr;
    byte_t* sptr;

    switch (d3dfmt)
    {
        default:
            break;

        case D3DFMT_R8G8B8:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB888, FALSE, align))
                return (NULL);
            return (img_888_to_32(dst, dx, dy, &temp));

        case D3DFMT_A8R8G8B8:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB8888, FALSE, align))
                return (NULL);
            return (img_auto_to_32(dst, dx, dy, &temp));

        case D3DFMT_X8R8G8B8:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB8888, FALSE, align))
                return (NULL);

            dst = img_auto_to_32(dst, dx, dy, &temp);
            if (dst == NULL)
                return (NULL);

            /* 取消 Alpha 通道 */
            for (idx = 0; idx < dst->size; idx += 4)
                dst->data[idx + 3] = 0xFF;
            return ((sIMAGE*)dst);

        case D3DFMT_R5G6B5:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB565, FALSE, align))
                return (NULL);
            return (img_565_to_32(dst, dx, dy, &temp));

        case D3DFMT_X1R5G5B5:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGBX555, FALSE, align))
                return (NULL);
            return (img_X555_to_32(dst, dx, dy, &temp));

        case D3DFMT_A1R5G5B5:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB1555, FALSE, align))
                return (NULL);
            return (img_1555_to_32(dst, dx, dy, &temp));

        case D3DFMT_A4R4G4B4:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB4444, FALSE, align))
                return (NULL);
            return (img_4444_to_32(dst, dx, dy, &temp));

        case D3DFMT_R3G3B2:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_INDEX8, FALSE, align))
                return (NULL);

            dst = safe_convert(dst, CR_ARGB8888, &temp, CR_INDEX8,
                               &dx, &dy, &width, &height);
            if (dst == NULL)
                return (NULL);

            stmp = (byte_t*)pixel;
            CR_MAKE_DST(dx, dy, 4);

            for (y = height; y != 0; y--) {
                for (x = 0; x < width; x++) {
                    pixel332to32(&dtmp[x], stmp[x]);
                    dtmp[x] |= CDWORD_LE(0xFF000000UL);
                }
                dtmp = (int32u*)((uchar*)dtmp + dbpl);
                stmp = (byte_t*)((uchar*)stmp + temp.bpl);
            }
            return ((sIMAGE*)dst);

        case D3DFMT_A8:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_INDEX8, FALSE, align))
                return (NULL);

            dst = safe_convert(dst, CR_ARGB8888, &temp, CR_INDEX8,
                               &dx, &dy, &width, &height);
            if (dst == NULL)
                return (NULL);

            stmp = (byte_t*)pixel;
            CR_MAKE_DST(dx, dy, 4);

            for (y = height; y != 0; y--) {
                dptr = (byte_t*)dtmp;
                for (x = 0; x < width; x++) {
                    dtmp[x] = CDWORD_LE(0x00FFFFFFUL);
                    dptr[3] = stmp[x];
                    dptr += 4;
                }
                dtmp = (int32u*)((uchar*)dtmp + dbpl);
                stmp = (byte_t*)((uchar*)stmp + temp.bpl);
            }
            return ((sIMAGE*)dst);

        case D3DFMT_A8R3G3B2:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB565, FALSE, align))
                return (NULL);

            dst = safe_convert(dst, CR_ARGB8888, &temp, CR_ARGB565,
                               &dx, &dy, &width, &height);
            if (dst == NULL)
                return (NULL);

            stmp = (byte_t*)pixel;
            CR_MAKE_DST(dx, dy, 4);

            for (y = height; y != 0; y--) {
                dptr = (byte_t*)dtmp;
                sptr = (byte_t*)stmp;
                for (x = 0; x < width; x++) {
                    pixel332to32(&dtmp[x], sptr[0]);
                    dptr[3] = sptr[1];
                    dptr += 4;
                    sptr += 2;
                }
                dtmp = (int32u*)((uchar*)dtmp + dbpl);
                stmp = (byte_t*)((uchar*)stmp + temp.bpl);
            }
            return ((sIMAGE*)dst);

        case D3DFMT_X4R4G4B4:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB4444, FALSE, align))
                return (NULL);

            dst = img_4444_to_32(dst, dx, dy, &temp);
            if (dst == NULL)
                return (NULL);

            /* 取消 Alpha 通道 */
            for (idx = 0; idx < dst->size; idx += 4)
                dst->data[idx + 3] = 0xFF;
            return ((sIMAGE*)dst);

        case D3DFMT_A2B10G10R10:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB8888, FALSE, align))
                return (NULL);

            dst = safe_convert(dst, CR_ARGB8888, &temp, CR_ARGB8888,
                               &dx, &dy, &width, &height);
            if (dst == NULL)
                return (NULL);

            stmp = (byte_t*)pixel;
            CR_MAKE_DST(dx, dy, 4);

            for (y = height; y != 0; y--) {
                dptr = (byte_t*)dtmp;
                for (x = 0; x < width; x++) {
                    clr = DWORD_LE(((int32u*)stmp)[x]);
                    dptr[2] = (byte_t)(clr >>  2);
                    dptr[1] = (byte_t)(clr >> 12);
                    dptr[0] = (byte_t)(clr >> 22);
                    dptr[3] = g_by04to256[clr >> 30];
                    dptr += 4;
                }
                dtmp = (int32u*)((uchar*)dtmp + dbpl);
                stmp = (byte_t*)((uchar*)stmp + temp.bpl);
            }
            return ((sIMAGE*)dst);

        case D3DFMT_A8B8G8R8:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB8888, FALSE, align))
                return (NULL);

            dst = img_auto_to_32(dst, dx, dy, &temp);
            if (dst == NULL)
                return (NULL);

            /* 交换 RB 通道 */
            for (idx = 0; idx < dst->size; idx += 4)
                swap_rb32(&dst->data[idx]);
            return ((sIMAGE*)dst);

        case D3DFMT_X8B8G8R8:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB8888, FALSE, align))
                return (NULL);

            dst = img_auto_to_32(dst, dx, dy, &temp);
            if (dst == NULL)
                return (NULL);

            /* 交换 RB 通道, 取消 Alpha 通道 */
            for (idx = 0; idx < dst->size; idx += 4)
            {
                swap_rb32(&dst->data[idx]);
                dst->data[idx + 3] = 0xFF;
            }
            return ((sIMAGE*)dst);

        case D3DFMT_G16R16:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB8888, FALSE, align))
                return (NULL);

            dst = safe_convert(dst, CR_ARGB8888, &temp, CR_ARGB8888,
                               &dx, &dy, &width, &height);
            if (dst == NULL)
                return (NULL);

            stmp = (byte_t*)pixel;
            CR_MAKE_DST(dx, dy, 4);

            for (y = height; y != 0; y--) {
                dptr = (byte_t*)dtmp;
                for (x = 0; x < width; x++) {
                    clr = DWORD_LE(((int32u*)stmp)[x]);
                    dptr[2] = (byte_t)(clr >>  8);
                    dptr[1] = (byte_t)(clr >> 24);
                    dptr[0] = 0x00;
                    dptr[3] = 0xFF;
                    dptr += 4;
                }
                dtmp = (int32u*)((uchar*)dtmp + dbpl);
                stmp = (byte_t*)((uchar*)stmp + temp.bpl);
            }
            return ((sIMAGE*)dst);

        case D3DFMT_A2R10G10B10:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB8888, FALSE, align))
                return (NULL);

            dst = safe_convert(dst, CR_ARGB8888, &temp, CR_ARGB8888,
                               &dx, &dy, &width, &height);
            if (dst == NULL)
                return (NULL);

            stmp = (byte_t*)pixel;
            CR_MAKE_DST(dx, dy, 4);

            for (y = height; y != 0; y--) {
                dptr = (byte_t*)dtmp;
                for (x = 0; x < width; x++) {
                    clr = DWORD_LE(((int32u*)stmp)[x]);
                    dptr[0] = (byte_t)(clr >>  2);
                    dptr[1] = (byte_t)(clr >> 12);
                    dptr[2] = (byte_t)(clr >> 22);
                    dptr[3] = g_by04to256[clr >> 30];
                    dptr += 4;
                }
                dtmp = (int32u*)((uchar*)dtmp + dbpl);
                stmp = (byte_t*)((uchar*)stmp + temp.bpl);
            }
            return ((sIMAGE*)dst);

        case D3DFMT_A16B16G16R16:
            if (!image_set(&temp, pixel, size, 0, 0, width * 2,
                           height, CR_ARGB8888, FALSE, align))
                return (NULL);

            temp.position.ww /= 2;  /* BPL 加倍后宽再改回来 */
            dst = safe_convert(dst, CR_ARGB8888, &temp, CR_ARGB8888,
                               &dx, &dy, &width, &height);
            if (dst == NULL)
                return (NULL);

            stmp = (byte_t*)pixel;
            CR_MAKE_DST(dx, dy, 4);

            for (y = height; y != 0; y--) {
                dptr = (byte_t*)dtmp;
                sptr = (byte_t*)stmp;
                for (x = 0; x < width; x++) {
                    dptr[2] = sptr[1];
                    dptr[1] = sptr[3];
                    dptr[0] = sptr[5];
                    dptr[3] = sptr[7];
                    dptr += 4;
                    sptr += 8;
                }
                dtmp = (int32u*)((uchar*)dtmp + dbpl);
                stmp = (byte_t*)((uchar*)stmp + temp.bpl);
            }
            return ((sIMAGE*)dst);

        case D3DFMT_A8L8:
            palette = NULL;     /* 只用亮度 */

        case D3DFMT_A8P8:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB565, FALSE, align))
                return (NULL);

            dst = safe_convert(dst, CR_ARGB8888, &temp, CR_ARGB565,
                               &dx, &dy, &width, &height);
            if (dst == NULL)
                return (NULL);

            if (palette == NULL)
                pal_set_gray8(temp.pal, 256);
            else
                mem_cpy(temp.pal, palette, 1024);

            stmp = (byte_t*)pixel;
            CR_MAKE_DST(dx, dy, 4);

            for (y = height; y != 0; y--) {
                dptr = (byte_t*)dtmp;
                sptr = (byte_t*)stmp;
                for (x = 0; x < width; x++) {
                    dtmp[x] = temp.pal[sptr[0]];
                    dptr[3] = sptr[1];
                    dptr += 4;
                    sptr += 2;
                }
                dtmp = (int32u*)((uchar*)dtmp + dbpl);
                stmp = (byte_t*)((uchar*)stmp + temp.bpl);
            }
            return ((sIMAGE*)dst);

        case D3DFMT_L8:
            palette = NULL;     /* 只用亮度 */

        case D3DFMT_P8:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_INDEX8, FALSE, align))
                return (NULL);

            if (palette == NULL)
                pal_set_gray8(temp.pal, 256);
            else
                mem_cpy(temp.pal, palette, 1024);
            return (img_idx8_to_32(dst, dx, dy, &temp));

        case D3DFMT_L16:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_ARGB565, FALSE, align))
                return (NULL);

            dst = safe_convert(dst, CR_ARGB8888, &temp, CR_ARGB565,
                               &dx, &dy, &width, &height);
            if (dst == NULL)
                return (NULL);

            stmp = (byte_t*)pixel;
            CR_MAKE_DST(dx, dy, 4);

            for (y = height; y != 0; y--) {
                dptr = (byte_t*)dtmp;
                sptr = (byte_t*)stmp;
                for (x = 0; x < width; x++) {
                    dptr[0] = sptr[1];
                    dptr[1] = sptr[1];
                    dptr[2] = sptr[1];
                    dptr[3] = 0xFF;
                    dptr += 4;
                    sptr += 2;
                }
                dtmp = (int32u*)((uchar*)dtmp + dbpl);
                stmp = (byte_t*)((uchar*)stmp + temp.bpl);
            }
            return ((sIMAGE*)dst);

        case D3DFMT_A4L4:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_INDEX8, FALSE, align))
                return (NULL);

            dst = safe_convert(dst, CR_ARGB8888, &temp, CR_INDEX8,
                               &dx, &dy, &width, &height);
            if (dst == NULL)
                return (NULL);

            stmp = (byte_t*)pixel;
            CR_MAKE_DST(dx, dy, 4);

            for (y = height; y != 0; y--) {
                dptr = (byte_t*)dtmp;
                for (x = 0; x < width; x++) {
                    dptr[0] = g_by16to256[stmp[x] & 0x0F];
                    dptr[1] = dptr[0];
                    dptr[2] = dptr[0];
                    dptr[3] = g_by16to256[stmp[x] >> 0x4];
                    dptr += 4;
                }
                dtmp = (int32u*)((uchar*)dtmp + dbpl);
                stmp = (byte_t*)((uchar*)stmp + temp.bpl);
            }
            return ((sIMAGE*)dst);

        case D3DFMT_DXT1:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_DXT1, FALSE, align))
                return (NULL);
            return (img_dxtc_to_32(dst, dx, dy, &temp));

        case D3DFMT_DXT2:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_DXT2, FALSE, align))
                return (NULL);
            return (img_dxtc_to_32(dst, dx, dy, &temp));

        case D3DFMT_DXT3:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_DXT3, FALSE, align))
                return (NULL);
            return (img_dxtc_to_32(dst, dx, dy, &temp));

        case D3DFMT_DXT4:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_DXT4, FALSE, align))
                return (NULL);
            return (img_dxtc_to_32(dst, dx, dy, &temp));

        case D3DFMT_DXT5:
            if (!image_set(&temp, pixel, size, 0, 0, width,
                           height, CR_DXT5, FALSE, align))
                return (NULL);
            return (img_dxtc_to_32(dst, dx, dy, &temp));
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
